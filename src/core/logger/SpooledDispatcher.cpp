/*******************************************************************************
 * SpooledDispatcher.cpp
 *
 * Copyright (c) 2014, Varuna L Amachi. All rights reserved.
 *
 * This program/library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program/library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 ******************************************************************************/

#include "SpooledDispatcher.h"
#include "../coreutils/Convenience.h"
#include <QDebug>


namespace Tanyatu { namespace Logger {

SpooledDispatcher::SpooledDispatcher( QObject *parent )
    : QThread( parent )
    , m_stop( false )
{
//    this->start();
}


SpooledDispatcher::~SpooledDispatcher()
{
    m_shouldStop = true;
    this->wait();
    foreach( LogMessage *msg, m_logQueue ) {
        writeToTargets( msg );
    }
    flush();
    for( auto it = m_targets.begin(); it != m_targets.end(); ++ it ) {
        TargetInfo *info = it.value();
        delete info;
    }
    for( auto fit = m_allFilters.begin(); fit != m_allFilters.end(); ++ fit ) {
        FilterInfo *finfo = fit.value();
        delete finfo;
    }
}


bool SpooledDispatcher::addTarget( AbstractLogTarget *target )
{
    bool result = false;
    if( target != nullptr && ! m_targets.contains( target->uniqueId() )) {
        SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
        m_targets.insert( target->uniqueId(), new TargetInfo( target, true ));
        result = true;
    }
    return result;
}

AbstractLogTarget *SpooledDispatcher::target(QString targetId)
{
    TargetInfo *info = m_targets.value( targetId );
    return info != nullptr ? info->m_target : nullptr;
}


bool SpooledDispatcher::setTargetEnabledState( const QString &target,
                                               bool value )
{
    bool result = false;
    TargetInfo *info = m_targets.value( target );
    if( info != nullptr ) {
        SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
        info->m_enabled = value;
        result = true;
    }
    return result;
}


bool SpooledDispatcher::removeTarget( const QString &targetId )
{
    bool result = false;
    TargetInfo *info = m_targets.value( targetId );
    if( info != nullptr ) {
        foreach( ILogFilter *filter, info->m_targetFilters ) {
            if( m_allFilters.value(
                        filter->filterId() )->m_refs == 1 ) {
                SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
                m_allFilters.remove( filter->filterId() );
                delete filter;
            }
        }
        SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
        m_targets.remove( targetId );
        delete info;
        result = true;
    }
    return result;
}


bool SpooledDispatcher::installFilter( ILogFilter *filter,
                                       const QString &targetId )
{
    bool result = false;
    if( filter ) {
        auto getFilterInfo = [ = ]() -> FilterInfo *
        {
            FilterInfo *fltInfo = m_allFilters.value( filter->filterId() );
            if( fltInfo == nullptr ) {
                fltInfo = new FilterInfo( filter );
                m_allFilters.insert( filter->filterId(), fltInfo );
            }
            else if( filter != fltInfo->m_filter ){
                /** We are registering two different filter instances with same
                 * filter id. In this case the correct way is to get the filter
                 * instance for the given id from the dispatcher and install
                 * the same instance again.
                 */
                fltInfo = 0;
            }
            return fltInfo;
        };

        if( targetId.isEmpty() ) {
            SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
            FilterInfo *finfo = getFilterInfo();
            if( finfo != nullptr ) {
                for( auto it = m_targets.begin();
                     it != m_targets.end();
                     ++ it ) {
                    TargetInfo *sinfo = it.value();
                    if( ! sinfo->m_targetFilters.contains( filter )) {
                        sinfo->m_targetFilters.append( filter );
                        ++ finfo->m_refs;
                    }
                }
                result = true;
            }
        }
        else if( m_targets.contains( targetId )){
            TargetInfo *sinfo = m_targets.value( targetId );
            if( sinfo != nullptr ) {
                SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
                FilterInfo *finfo = getFilterInfo();
                if( finfo != nullptr ) {
                    if( ! sinfo->m_targetFilters.contains( filter )) {
                        sinfo->m_targetFilters.append( filter );
                        ++ finfo->m_refs;
                    }
                    result = true;
                }
            }
        }
    }
    return result;
}


bool SpooledDispatcher::uninstallFilter( const QString &filterId,
                                         const QString &targetId )
{
    bool result = false;
    FilterInfo *finfo = m_allFilters.value( filterId );
    if( finfo != nullptr ) {
        if( targetId.isEmpty() ) {
            int numRemoved = 0;
            SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
            for( auto sit = m_targets.begin(); sit != m_targets.end(); ++ sit ){
                TargetInfo *sinfo = sit.value();
                if( sinfo->m_targetFilters.removeOne( finfo->m_filter )) {
                    ++ numRemoved;
                }
            }
            result = numRemoved != 0;
            delete finfo;
        }
        else {
            TargetInfo *sinfo = m_targets.value( targetId );
            if( sinfo != nullptr ) {
                SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
                result = true;
                if( sinfo->m_targetFilters.removeOne( finfo->m_filter )) {
                    -- finfo->m_refs;
                    if( finfo->m_refs == 0 ) {
                        m_allFilters.remove( filterId );
                        delete finfo;
                    }
                }

            }
        }
    }
    return result;
}


void SpooledDispatcher::flush()
{
    SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
    for( auto sit = m_targets.begin(); sit != m_targets.end(); ++ sit ) {
        TargetInfo *sinfo = sit.value();
        sinfo->m_target->flush();
    }
}


void SpooledDispatcher::stopDispatch()
{
    m_stop = true;
}


void SpooledDispatcher::write(LogMessage *message)
{
    if( ! this->isRunning() ) {
        this->start();
    }
    SCOPE_LIMIT( m_logIoLock.lockForWrite(), m_logIoLock.unlock() );
    m_logQueue.append( message );
}


void SpooledDispatcher::run()
{
    while( ! m_stop ) {
        m_logIoLock.lockForRead();
        bool hasMsgs = ! m_logQueue.isEmpty();
        m_logIoLock.unlock();
        if( hasMsgs ) {
            m_logIoLock.lockForWrite();
            LogMessage *msg = m_logQueue.takeLast();
            AT_SCOPE_EXIT( delete msg );
            m_logIoLock.unlock();
            writeToTargets( msg );
        }
    }
}


void SpooledDispatcher::writeToTargets( LogMessage *msg )
{
    SCOPE_LIMIT( m_lock.lockForRead(), m_lock.unlock() );
    for( auto sit = m_targets.begin(); sit != m_targets.end(); ++ sit ) {
        TargetInfo *sinfo = sit.value();
        if( sinfo->m_enabled ) {
            bool filteredOut = false;
            foreach( ILogFilter *filter, sinfo->m_targetFilters ) {
                FilterInfo *finfo = m_allFilters.value( filter->filterId() );
                if( finfo->m_enabled ) {
                    filteredOut = filter->filterOut( msg );
                    if( filteredOut ) {
                        break;
                    }
                }
            }
            if( ! filteredOut ) {
                sinfo->m_target->write( msg );
            }
        }
    }
}


} } // end of namespaces










