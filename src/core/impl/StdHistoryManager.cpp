/*******************************************************************************
 * StdHistoryManager.cpp
 *
 * Copyright (c) 2012, Varuna L Amachi. All rights reserved.
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

#include "StdHistoryManager.h"
#include "../T.h"

#include <QSize>
#include <QFont>

namespace Tanyatu { namespace Impl {

QString StdHistoryManager::COMPONENT_NAME =
        QString( "Tanyatu::Core::StdHistoryManager" );

QString StdHistoryManager::COMPONENT_MODULE = QString( "Tanyatu::Core" );


StdHistoryManager::StdHistoryManager( QObject *parent )
    : IHistoryManager( parent )
{

}


bool StdHistoryManager::init()
{
    emit aboutToChangeHistoryManager();
    //Here we clear all the history erlier than 10 days before
    QDateTime from = QDateTime::currentDateTime().addDays( -10 );
    DATA_UPDATER()->clearHistory( from );

    //Get the stuff from last 10 days
    bool result = DATA_RETRIEVER()->getHistory( m_history );
    emit historyManagerChanged();
    return result;
}


void StdHistoryManager::onItemPlayed( Tanyatu::Data::MediaItem *item )
{
    if( item->type() == Tanyatu::Data::Media_StoredAudio
            || item->type() == Data::Media_StoredVideo ) {
        QDateTime currentTime = QDateTime::currentDateTime();
        DATA_UPDATER()->addHistory( item->trackId(),
                                             currentTime );
        QPair< QString, QDateTime > *history =
                new QPair< QString, QDateTime >( item->trackId(), currentTime );
        emit aboutToChangeHistoryManager();
        m_history.append( history );
        emit historyManagerChanged();
    }
}


void StdHistoryManager::clear()
{
    emit aboutToChangeHistoryManager();
    DATA_UPDATER()->clearHistory();
    clearStruct();
    emit historyManagerChanged();
}


void StdHistoryManager::getHistoryItemIds( QList<QString> &ids ) const
{
    QPair< QString, QDateTime > *history = 0;
    foreach( history, m_history ) {
        ids.append( history->first );
    }
}


void StdHistoryManager::clearStruct()
{
    QPair< QString, QDateTime > *history = 0;
    foreach( history, m_history ) {
        delete history;
    }
    m_history.clear();
}


void StdHistoryManager::onItemRemovalFromStorage( QString trackId )
{
    emit aboutToChangeHistoryManager();
    for( int i = m_history.size() - 1; i >= 0; -- i ) {
        QString itemId = m_history[ i ]->first;
        if( itemId == trackId ) {
            m_history.removeAt( i );
            DATA_UPDATER()->removeHistoryItem( trackId );
            return;
        }
    }
    emit historyManagerChanged();
}


QString StdHistoryManager::uniqueName() const
{
    return COMPONENT_NAME;
}


QString StdHistoryManager::module() const
{
    return COMPONENT_MODULE;
}


QString StdHistoryManager::displayName() const
{
    return "History";
}


} } //end of namespaces

