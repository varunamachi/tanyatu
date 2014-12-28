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
#include <QSize>
#include <QFont>
#include <QAbstractItemModel>
#include <QTime>
#include <QtAlgorithms>

#include "StdPlayQueue.h"
#include "../data/AudioTrack.h"
#include "../data/VideoTrack.h"
#include "../data/MediaItem.h"
#include "../coreutils/Utils.h"
#include "../interfaces/IComponent.h"

#define RANDOM_STOP_PERCENTAGE 80
#define PREV_LIST_REMOVE_THRESHOLD 100

namespace Tanyatu { namespace Impl {

StdPlayQueue::StdPlayQueue( QObject *parent )
    : IPlayQueue( parent )
    , m_currentIndex( -1 )
    , m_prevListIndex( -1 )
    , m_random( false )
    , m_repeatType( IPlayQueue::RepeatType_RepeatAll )
{

}


void StdPlayQueue::addItem( const Tanyatu::Data::MediaItem *c_item,
                            bool selected )
{
    Data::MediaItem *item = const_cast< Data::MediaItem *>( c_item );
    if( item ) {
        emit aboutToChangePlayQueue();
        m_items.append( item );
        m_unplayedIds.insert( item->trackId() );
        emit playQueueChanged();
        emit itemAdded( item );
        if( selected ) {
            selectIndex( m_items.size() - 1 );
        }
        emit playQueueChanged();
    }
}


void StdPlayQueue::addItems(
        const QList< Tanyatu::Data::MediaItem *> &items )
{
    if( ! items.empty() ) {
        emit aboutToChangePlayQueue();
        for( int i = 0; i < items.size(); ++i ) {
            Data::MediaItem *item = items.at( i );
            m_items.append( item );
            m_unplayedIds.insert( item->trackId() );
            emit itemAdded( item );
        }
        emit playQueueChanged();
    }
}


void StdPlayQueue::removeItem( const QString itemId )
{
   QList< int > indices;
   getAllIndicesOf( itemId, indices );
   if( ! indices.isEmpty() ) {
       removeItems( indices );
   }
}


void StdPlayQueue::removeItem( const Tanyatu::Data::MediaItem *item )
{
    QList< int > indices;
    getAllIndicesOf( item->trackId(), indices );
    if( ! indices.isEmpty() ) {
        removeItems( indices );
    }
}


void StdPlayQueue::removeItem( int index )
{
    QList< int > solo;
    solo << index;
    removeItems( solo );

}


void StdPlayQueue::removeItems(
        const QList< Tanyatu::Data::MediaItem * > &items )
{
    QList< int > indices;
    foreach( Data::MediaItem *item, items ) {
        getAllIndicesOf( item->trackId(), indices );
    }
    if( ! indices.isEmpty() ) {
        removeItems( indices );
    }
}


void StdPlayQueue::removeItems( int fromIndex, int toIndex )
{
    if( fromIndex > toIndex ) {
        QList< int > indices;
        for( int i = fromIndex; i < toIndex; ++i ) {
            indices.append( i );
        }
        removeItems( indices );
    }
}


void StdPlayQueue::removeItems( QList< int > &indices )
{
    /* We have to delete the items which are being deleted from the playlist
     * which are persisted items */
    QList< Data::MediaItem * > deletionCandidates;
    bool currentIndexChanged = false;
//    bool plChanged = false;
    qSort( indices );
    emit aboutToChangePlayQueue();
    for( int i = indices.size() - 1; i >= 0; -- i ) {
        int index = indices.at( i );
        if( isValidIndex( index ) ) {
            //Notify the removal to the view
            Data::MediaItem *delItem = m_items.at( index );
            if( m_items.size() == 1 ) {
                //If the there is only one item, then clear the list
                emit clearingPlayQueue();
                emit removingSelectedItem();
                emit removingItem( delItem, index );
                deletionCandidates.append( delItem );
                m_items.removeAt( index );
                emit removedItem( delItem, index);
                m_currentIndex = -1;
                break;
                currentIndexChanged = true;
//                plChanged = true;
            }
            else if( index > m_currentIndex ) {
                /* If the index for removal is more than the current selected
                 * index then we are relieved since the removal wont affect the
                 * currently selected index
                 */
                emit removingItem( delItem, index );
                deletionCandidates.append( delItem );
                m_items.removeAt( index );
                emit removedItem( delItem, index);
//                plChanged = true;
            }
            else if ( index == m_currentIndex ) {
                int oldCurIndex = m_currentIndex;
                /* If we are removing the current item, we should notify it,
                 * since player may be using it and we hope it will handle this
                 * situaltion properly.
                 */
                emit removingSelectedItem();
                emit removingItem( delItem, index );
                /* Since current item is no more, we set it to invalid. Later we
                 * will select the next one
                 */
                m_currentIndex = -1;
                emit indexOfCurrentChanged( m_currentIndex, oldCurIndex );
                m_items.removeAt( index );
                deletionCandidates.append( delItem );
                emit removedItem( delItem, index );
                currentIndexChanged = true;
//                plChanged = true;
            }
            else if( index < m_currentIndex ) {

                /* If we are removing an item before the current one, the index
                 * of the current item needs to be adjusted - decremented.
                 */
                -- m_currentIndex;
                emit indexOfCurrentChanged( m_currentIndex, m_currentIndex +1 );
                emit removingItem( delItem, index );
                deletionCandidates.append( delItem );
                m_items.removeAt( index );
                emit removedItem( delItem, index );
//                plChanged = true;
                currentIndexChanged = true;
            }
        }
    }

    if( currentIndexChanged ) {
        if( m_currentIndex == -1 ) {
            selectNext();
        }
    }
    /*
     * Update the internal structures: clear the previously played items and
     * if the player is currently in random play mode then remove the track
     * from unplayed ids.
     * Deallocate the items for which playlist is the owner. The
     * PersistedItems are owned by the media library and shall not be
     * deallocated here
     */
    m_prevIndicesList.clear();
    for( int i = 0; i < deletionCandidates.size(); ++i) {
        Data::MediaItem *item = deletionCandidates.at( i );
        if( m_random ) {
            m_unplayedIds.remove( item->trackId() );
        }
        if( item->type() != Data::Media_StoredAudio
                && item->type() != Data::Media_StoredVideo) {
            delete item;
        }
    }
//    if( plChanged ) {
    emit playQueueChanged();
//    }
}


void StdPlayQueue::clearAllStoredItems()
{
    bool atleastOneRemoved = false;
    emit aboutToChangePlayQueue();
    for( int i = m_items.size() - 1; i >= 0; --i ) {
        Data::MediaItem *item = m_items.at( i );
        if( item->type() == Data::Media_StoredAudio
            || item->type() == Data::Media_StoredVideo ) {
            if( ! atleastOneRemoved ) {
                atleastOneRemoved = true;
                emit clearingPlayQueue();
            }
            m_items.removeAt( i );
            m_unplayedIds.remove( item->trackId() );
        }
    }
    if( atleastOneRemoved ) {
        m_prevIndicesList.clear();
        if( m_items.isEmpty() ) {
            m_currentIndex = -1;
            m_prevListIndex = -1;
            emit playQueueCleared();
        }
        else {
        }
    }
    emit playQueueChanged();
}


void StdPlayQueue::clear()
{
    emit clearingPlayQueue();
    emit aboutToChangePlayQueue();
    for(int i = 0; i < m_items.size(); ++ i) {
        Data::MediaItem *item = m_items.at( i );
        if( ! ( item->type() == Data::Media_StoredAudio
                || item->type() == Data::Media_StoredVideo )) {
            //Delete item which is not owned by 'this'
            delete item;
        }
    }
    m_items.clear();
    m_unplayedIds.clear();
    m_prevIndicesList.clear();
    m_currentIndex = -1;
    m_prevListIndex = -1;
    emit playQueueChanged();
    emit playQueueCleared();
}


void StdPlayQueue::selectNext()
{
    int selectedIndex = 0;
    if( ! m_prevIndicesList.isEmpty()
            && m_prevListIndex >= 0
            && m_prevListIndex < m_prevIndicesList.size() - 1 ) {
        m_prevListIndex = m_prevListIndex > m_prevIndicesList.size() - 1
                                ? m_prevIndicesList.size() - 1
                                : m_prevListIndex;
        /**
          * If we are down the previously played track and we are trieng to
          * select the next track, this track will the next index present in
          * the previously played indices list and not a new track.
          */
        selectedIndex = m_prevIndicesList.at( m_prevListIndex );
        ++ m_prevListIndex;
    }
    else if( m_random ) {
        selectedIndex = getRandomIndex();
    }
    /* If random is not selected then we check if next is available if yes we
     * have few conditions to check for, if not availabel notify that playlist
     * finished
     */
    else if( hasNext() ) {
        if( m_currentIndex == m_items.size() - 1
                && m_repeatType == IPlayQueue::RepeatType_RepeatAll) {
            //Start all over again
            selectedIndex = 0;
        }
        else if( m_repeatType == IPlayQueue::RepeatType_RepeatOne ) {
            selectedIndex = m_currentIndex;
        }
        else {
            selectedIndex = m_currentIndex + 1;
        }
    }
    else {
        emit playQueueConfigChanged();
        emit finished();
        return;
    }
    //And if it is selected, it's no longer unplayed
    m_unplayedIds.remove( m_items.at( selectedIndex )->trackId() );
    addToPrevList( selectedIndex );
    m_currentIndex = selectedIndex;
    emit itemSelected( m_items.at( m_currentIndex ), m_currentIndex );
    /*
     * Although playlist contents themself didn't change, the state of the
     * playlist has changed
     */
    emit playQueueConfigChanged();
}


void StdPlayQueue::selectPrevious()
{
    if( m_items.isEmpty() )
    {
        m_currentIndex = -1;
        return;
    }
    int selectedIndex = 0;
    if( ! m_prevIndicesList.isEmpty() && m_prevListIndex != 0 ) {
        -- m_prevListIndex;
        m_prevListIndex = m_prevListIndex > m_prevIndicesList.size() - 1
                                ? m_prevIndicesList.size() - 1
                                : m_prevListIndex;
        selectedIndex = m_prevIndicesList.at( m_prevListIndex );
        m_unplayedIds.remove( m_items.at( selectedIndex )->trackId() );
    }
    else if( m_prevIndicesList.isEmpty() )
    {
        selectedIndex = m_currentIndex == 0 ? m_items.size() - 1
                                            : m_currentIndex - 1;
    }
    else {
        if( m_random ) {
            //If its random doesn't matter if it is next or previous
            selectedIndex = getRandomIndex();
            m_unplayedIds.remove( m_items.at( selectedIndex )->trackId() );
            addToPrevList( selectedIndex );
        }
        else if( m_repeatType == IPlayQueue::RepeatType_RepeatAll ) {
            selectedIndex =  m_currentIndex == 0 ? ( m_items.size() - 1 )
                                                 : ( m_currentIndex - 1 );
        }
        else if( m_repeatType == IPlayQueue::RepeatType_RepeatOne ) {
            //Play it again and again
            selectedIndex = m_currentIndex;
        }
        else if( m_currentIndex != 0 ) {
            //Decrement to go behind
            selectedIndex = m_currentIndex - 1;
        }
        else {
            //Ok done walking backwords
            emit playQueueConfigChanged();
            emit finished();
            return;
        }
    }
    m_currentIndex = selectedIndex;
    emit itemSelected( m_items.at( m_currentIndex ), m_currentIndex );
    /*
     * Although playlist contents themself didn't change, the state of the
     * playlist has changed
     */
    emit playQueueConfigChanged();
}


void StdPlayQueue::selectIndex( int index )
{
    if( isValidIndex( index ) ) {
        Data::MediaItem *item = m_items.at( index );
        m_unplayedIds.remove( item->trackId() );
        m_prevIndicesList.append( index );
        ++ m_prevListIndex;
        m_currentIndex = index;
        emit itemSelected( item, m_currentIndex );
        emit playQueueConfigChanged();
    }
}


void StdPlayQueue::insert(
        int index,
        const QList< Tanyatu::Data::MediaItem * > &items )
{
    emit aboutToChangePlayQueue();
    foreach( Data::MediaItem *item, items ) {
        m_items.insert( index, item );
        m_unplayedIds.insert( item->trackId() );
    }
    //Clear the prev played list since updating it costly
    m_prevIndicesList.clear();
    emit playQueueChanged();
}


void StdPlayQueue::moveItem( int origIndex, int newIndex, int numItems )
{
    if( origIndex >= 0 && origIndex < m_items.size()
            && newIndex >=0 && newIndex < m_items.size()
            && newIndex != origIndex )
    {
        emit aboutToChangePlayQueue();
        for( int index = origIndex;
             index < ( origIndex +numItems );
             ++ index ) {
            Data::MediaItem *item = m_items.at( index );
            m_items.move( index, newIndex );
            ++ newIndex;
            emit itemMoved( item, index, newIndex );
            if( index == m_currentIndex ) {
                emit indexOfCurrentChanged( newIndex, index );
            }
        }
        m_prevIndicesList.clear();
        emit playQueueChanged();
    }
}


void StdPlayQueue::setRandom( bool value )
{
    m_random = value;
    m_unplayedIds.clear();
    m_prevIndicesList.clear();
    m_prevListIndex = -1;
    emit playQueueConfigChanged();
}


void StdPlayQueue::setRepeat(
        Tanyatu::IPlayQueue::RepeatType repeatType)
{
    m_repeatType = repeatType;
    emit playQueueConfigChanged();
}


Data::MediaItem* StdPlayQueue::current() const
{
    return m_items.at( m_currentIndex );
}


int StdPlayQueue::currentIndex() const
{
    return m_currentIndex;
}


bool StdPlayQueue::hasNext() const
{
    if( m_items.isEmpty() || m_currentIndex == -1 ) {
        return false;
    }
    else if( m_repeatType == IPlayQueue::RepeatType_NoRepeat
             && m_currentIndex == m_items.size() - 1)
    {
        return false;
    }
    return true;
}


bool StdPlayQueue::hasPrev() const
{
    bool result = true;
    if( m_items.isEmpty() || m_currentIndex == -1 || m_prevListIndex == -1) {
        result = false;
    }
    else if( m_random && m_prevIndicesList.isEmpty() )
    {
        result = false;
    }
    return result;
}


int StdPlayQueue::numberOfItems() const
{
    return m_items.size();
}


bool StdPlayQueue::isRandom() const
{
    return m_random;
}


IPlayQueue::RepeatType StdPlayQueue::repeatType() const
{
    return m_repeatType;
}


const QList<Data::MediaItem *> *StdPlayQueue::getAllItemsInOrder() const
{
    return &m_items;
}


void StdPlayQueue::addToPrevList( int index )
{
    m_prevIndicesList.append( index );
    //Always point to the most recently played
    m_prevListIndex = m_prevIndicesList.size() - 1;
    if( m_prevIndicesList.size() > PREV_LIST_REMOVE_THRESHOLD ) {
        m_prevIndicesList.removeFirst();
    }
}


int StdPlayQueue::getRandomIndex()
{
    int selectedIndex = 0;
    if( m_unplayedIds.empty() ) {
        //We dont have nothing unplayed now... replenish
        for( int i = 0; i < m_items.size(); ++ i ) {
            m_unplayedIds.insert( m_items.at( i )->trackId() );
        }
        return selectedIndex;
    }
    //findout the percentage of tracks that are already played
    int usedPc = static_cast< int >(
                ( m_items.size() - m_unplayedIds.size() )
                / m_items.size()
                * 100);
    if( usedPc > RANDOM_STOP_PERCENTAGE ) {
        /**
          * If 80pc of the tracks are already played, there is no point in
          * generating random numbers. So we iterate through the item list
          * and play the first available unplayed track
          */
        for( int index = 0; index < m_items.size(); ++ index) {
            Data::MediaItem *item = m_items.at( index );
            if( m_unplayedIds.contains( item->trackId() )) {
                selectedIndex = index;
                break;
            }
        }
    }
    else {
        /**
          * If we have got more than 20pc tracks unplayed, we generate
          * random number and see the generated index is unplayed, if yes
          * then select it, otherwise generate a new random number
          */
        int indexCandidate = qrand() % m_items.size();
        while( ! m_unplayedIds.contains(
                   m_items.at( indexCandidate )->trackId() )) {
            indexCandidate = qrand() % m_items.size();
        }
        selectedIndex = indexCandidate;
    }
    return selectedIndex;
}


void StdPlayQueue::getAllIndicesOf( QString itemId,
                                        QList<int> &indices ) const
{
    for( int index = 0; index < m_items.size(); ++ index ) {
        if ( m_items[ index ]->trackId() == itemId ) {
            indices.append( index );
        }
    }
}


QString StdPlayQueue::uniqueName() const
{
    return "StdPlayQueue";
}


QString StdPlayQueue::module() const
{
    return "Tanyatu::Core::StandrdComponents";
}

QString StdPlayQueue::displayName() const
{
    return "Playlist";
}

bool StdPlayQueue::init()
{
    /**
     * @todo May be we can load the tracks loaded in the previous session here
     */
    return true;
}

bool StdPlayQueue::isValidIndex(int index) const
{
    return index >= 0 && index < m_items.size();
}

} } //end of ns
