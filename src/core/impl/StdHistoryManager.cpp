/*******************************************************************************
 * Copyright (c) 2014 Varuna L Amachi. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

