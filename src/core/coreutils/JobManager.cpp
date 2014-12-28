/*******************************************************************************
 * JobManager.cpp
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
#include <QEvent>
#include <QCoreApplication>

#include "JobManager.h"



namespace Tanyatu {

namespace {
class StdJob : public IJob
{
public:
    StdJob( QString name,
            JobFunc job,
            bool guiDispatch,
            QString category )
        : m_name( name )
        , m_jobFunc( job )
        , m_isGuiDispatch( guiDispatch )
        , m_category( category )
    {

    }

    const QString & name() const
    {
        return m_name;
    }

    const QString & category() const
    {
        return m_category;
    }

    bool shouldPostResponse() const
    {
        return m_isGuiDispatch;
    }

    RespFunc execute()
    {
        return m_jobFunc();
    }

private:
    QString m_name;

    JobFunc m_jobFunc;

    bool m_isGuiDispatch;

    QString m_category;
};


class CallbackEvent : public QEvent
{
public:
    CallbackEvent( RespFunc callback )
        : QEvent( CallbackEvent::type() )
        , m_callback( callback )
    {

    }

    void invokeCallback()
    {
        m_callback();
    }

    static QEvent::Type type()
    {
        QEvent::registerEventType( CallbackEvent::EVENT_TYPE );
        return CallbackEvent::EVENT_TYPE;
    }

    static const QEvent::Type EVENT_TYPE;

private:
    RespFunc m_callback;
};


} //end of anonymous namespace

const QEvent::Type CallbackEvent::EVENT_TYPE
                                          = static_cast< QEvent::Type >( 2334 );


JobManager::JobManager( QObject *parent )
    : QThread( parent )
{

}


bool JobManager::hasPendingJobs()
{
    SCOPE_LIMIT( m_lock.lockForRead(), m_lock.unlock() );
    return ! m_jobs.empty();
}



void JobManager::addJob( IJob *job )
{
    SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
    m_jobs.append( job );
}


void JobManager::addJob( QString name,
                         JobFunc job,
                         QString category,
                         bool postResponseToEventQueue )
{
    SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
    m_jobs.append( new StdJob( name,
                               job,
                               postResponseToEventQueue,
                               category ));
}


bool JobManager::cancelJob( QString name )
{
    SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
    return m_cancelledJobs.insert( name ) != m_cancelledJobs.end();
}


bool JobManager::cancelJobCategory( QString category )
{
    SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
    return m_cancelledCategories.insert( category )
                                                != m_cancelledCategories.end();
}


void JobManager::cancelAll()
{
    m_lock.lockForWrite();
    m_stopProcessing = true;
    m_lock.unlock();
    this->wait();
    SCOPE_LIMIT( m_lock.lockForWrite(), m_lock.unlock() );
    foreach( IJob *job, m_jobs ) {
        delete job;
    }
    m_cancelledCategories.clear();
    m_cancelledJobs.clear();
}


void JobManager::run()
{
    bool shouldExit = false;
    bool hasPendingTasks = true;
    IJob *job = nullptr;

    while( ! shouldExit ){
        m_lock.lockForRead();
        shouldExit = m_stopProcessing;
        hasPendingTasks = m_jobs.size();
        m_lock.unlock();
        if( hasPendingTasks ) {
            m_lock.lockForWrite();
            job = m_jobs.last();
            m_jobs.removeLast();
            m_lock.unlock();

            m_lock.lockForWrite();
            bool isCancelled = m_cancelledJobs.remove( job->name() );
            m_lock.unlock();

            if( ! isCancelled ) {
                emit executionStarted( job->name(), job->category() );
                RespFunc func = job->execute();
                emit executionFinished( job->name(), job->category() );
                if( job->shouldPostResponse() ) {
                    QCoreApplication::postEvent(
                                this,
                                new CallbackEvent( func ),
                                100 );
                }
                else {
                    func();
                }
            }
            else {
                emit jobDescarded( job->name(), job->category() );
            }
            delete job;
        }
    }


}

bool JobManager::event( QEvent *event )
{
    if( event->type() == CallbackEvent::EVENT_TYPE ) {
        CallbackEvent *callbackEvt = static_cast< CallbackEvent *>( event );
        callbackEvt->invokeCallback();
    }
    return QThread::event( event );
}

} //end of namespace
