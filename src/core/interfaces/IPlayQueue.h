/*******************************************************************************
 * IPlayQueue.h
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
#pragma once

#include <QList>
#include <QStringList>
#include <QObject>

#include "../data/MediaItem.h"
#include "../TanyatuCoreGlobal.h"
#include "IComponent.h"


namespace Tanyatu {

/**
  * @class IPlayQueue this class should be inherited/implemented by any
  * class that presents itself as an implementation of playlist. This class
  * provides an interface for a playlist implementation.
  * The class inherits this class must implement the normal pure virtual
  * functions and override desired methods from @class QAbstractItemModel
  */
class TANYATU_CORE_EXPORT IPlayQueue : public QObject,
                                       public IComponent
{

    Q_OBJECT
public:
    enum RepeatType
    {
        RepeatType_RepeatAll,
        RepeatType_RepeatOne,
        RepeatType_NoRepeat
    };

signals:
    /**
      * @brief Emmited when an media item is added to the playlist
      * @param item the item that has been added
      */
    void itemAdded( Tanyatu::Data::MediaItem *item );

    /**
      * @brief Emmited when an item is selected in the playlist
      * @param item item that is selected
      * @param index of the item selected
      */
    void itemSelected( Tanyatu::Data::MediaItem *item, int index );


    /**
      * @brief Emmited when the index of the selected item changes, but the
      * selected item itself remains the same.
      * @param changedIndex index of the selected item after it has changed
      * @param oldIndex the index of the selected item before the change
      */
    void indexOfCurrentChanged( int changedIndex, int oldIndex );

    /**
      * @brief Emmited before an item is removed from the playlist
      * @param item item that is to be removed
      * @param index of the item to be removed
      */
    void removingItem( Tanyatu::Data::MediaItem *item, int index );


    /**
      * @brief emitted when the currently selected item is removed
      */
    void removingSelectedItem();

    /**
     * @brief Emmited after an item is removed from the playlist
     * @param item item that is removed
     * @param index of the item removed
     */
    void removedItem( Tanyatu::Data::MediaItem *item, int index );

    /**
      * @brief Emmited when an item is from one index to other in the playlist
      * @param item item that is moved
      * @param oldIndex index of the item before moving
      * @param newIndex index of the item after moving
      */
    void itemMoved( Tanyatu::Data::MediaItem *item,
                    int oldIndex,
                    int newIndex );


    /**
     * @brief aboutToChangePlayQueue Emitted before changing the playlist
     */
    void aboutToChangePlayQueue();

    /**
      * @brief Emmited after an operation changes the playlist.
      */
    void playQueueChanged();

    /**
     * @brief playQueueConfigChanged Emitted when the configuration associated
     * with the play queue:
     *      - repeat type 
     *      - progression 
     *      - current track
     *      
     * This will not be emitted when actual contenets of the playqueue change.
     */
    void playQueueConfigChanged();

    /**
      * @brief Emmited before clearing the playlist
      */
    void clearingPlayQueue();

    /**
      * @brief Emmited after the playlist is cleared
      */
    void playQueueCleared();

    /**
      * @brief Emmited when the playlist progression behavior changes between
      * random and sequential
      * @param value if true the progression set to random or else if false
      * progression set to sequential
      */
    void randomChanged( bool value );


    /**
      * @brief Emmited when the loop behavior of the playlist changes
      * @param newType the new behavior
      * @param oldType the old behavior
      */
    void repeatChanged( Tanyatu::IPlayQueue::RepeatType newType,
                        Tanyatu::IPlayQueue::RepeatType oldType );

    /**
      * @brief Emmited when the playlist finishes.
      */
    void finished();

public:


    /**
      * @brief Gives the current item selected in the playlist
      * @returns the selected item in the playlist
      */
    virtual Data::MediaItem *current() const = 0;

    /**
      * @brief Gives the index of the current item in the playlist
      * @returns the index of the current item in the playlist
      */
    virtual int currentIndex() const = 0;

    /**
      * @brief Give the count of items in the playlist
      * @returns number of items in the playslit
      */
    virtual int numberOfItems() const = 0;


    /**
      * @brief Tells whether the playlist has next item to play
      * @returns true if there is a item to play next in the playlist
      */
    virtual bool hasNext() const = 0;

    /**
      * @brief Tells whether there is a previous track that has been played
      * @return true if a item was played previously
      */
    virtual bool hasPrev() const = 0;



    /**
      * @brief Gives the playlist selection behaviour i.e random or sequential
      * @returns true if the track selection is random, false if sequential
      */
    virtual bool isRandom() const = 0;

    /**
      * @brief Gives the playlist looping behavior
      * @returns @enum PlaylistRepeatType corresponding to the looping behavior
      * of the playlist
      */
    virtual RepeatType repeatType() const = 0;

    /**
      * @brief Gives all the items present in the playlist in the order they
      * exist in the playlist
      * @returns list of items present in the playlist in the order.
      */
    virtual const QList< Data::MediaItem * > * getAllItemsInOrder() const = 0;

    /**
      * Destructor.
      */
    virtual ~IPlayQueue() { /*NOTHING TO DO*/ }

public slots:
    /**
      * @brief Appends the given media item at the end of the playlist.
      * @param item the item that needs to be appended
      * @param select selects the recently added
      */
    virtual void addItem( const Tanyatu::Data::MediaItem *item,
                          bool select ) = 0;

    /**
      * @brief Appends a list of items one by one at the end of the playlist.
      * @param items list of media items to be appended
      */
    virtual void addItems(
            const QList< Tanyatu::Data::MediaItem *> &items ) = 0;

    /**
      * @brief Removes an item with the given id from the playlist.
      * @param id of the item to be removed
      */
    virtual void removeItem( const QString itemId ) = 0;

    /**
      * @brief Removes the given item from the playlist if found.
      * @param item tha1t is to be removed.
      */
    virtual void removeItem( const Tanyatu::Data::MediaItem *item ) = 0;

    /**
      * @brief Removes an item present at the given index in the playlist
      * @param index of the media item that is to removed
      */
    virtual void removeItem( int index ) = 0;


    /**
      * @brief Removes the given media items from the playlist.
      * @param items list of media items that needs to be removed.
      */
    virtual void removeItems(
            const QList< Tanyatu::Data::MediaItem *> &items ) = 0;


    /**
      * @brief Removes items at the given indices of the playlist. Ignores if
      * any index is invalid
      * @param indices list of indices corresponding to the media items that
      * are to be removed.
      */
    virtual void removeItems( QList< int > &indices) = 0;

    /**
      * @brief Removes media items from fromIndex to toIndex of the playlist,
      * ignoring non existing indices
      * @param fromIndex index from where the removal will begin.
      * @param toIndex where the removal will end.
      */
    virtual void removeItems( int fromIndex, int toIndex ) = 0;


    /**
     * @brief clearAllStoredItems Clears the track that are part of audio
     * library, that is tracks whose information is stored in the database.
     */
    virtual void clearAllStoredItems() = 0;


    /**
      * @brief Clears all the items from the playlist.
      */
    virtual void clear() = 0;

    /**
      * @brief Selects the next item in the playlist based on the Random and
      * Repeat configurations Signal indicating the selected item will be
      * emmited.
      */
    virtual void selectNext() = 0;

    /**
      * @brief Selects the previous item in the playlist. Signal indicating the
      * selected item will be emmited.
      */
    virtual void selectPrevious() = 0;

    /**
      * @brief Selects the media item corresponding to the given index.
      * @param index of the item to be selected
      */
    virtual void selectIndex( int index ) = 0;

    /**
      * @brief Set the play sequence mode to random/shuffle.
      * @param value true if random playback is to be enabled.
      */
    virtual void setRandom( bool value ) = 0;


    /**
      * @brief Inserts media items at the given index.
      * @param index where the insertion has to be done.
      * @param items list of media items that needs to be inserted
      */
    virtual void insert( int index,
                         const QList< Tanyatu::Data::MediaItem *> &items ) = 0;

    /**
      * @brief Sets the looping behavior of the playlist.
      * @param repeatType repeat behavior of the playlist. It could be repeat
      * all the items, repeat current item or do not repeat after the playlist
      * finishes
      */
    virtual void setRepeat(
            Tanyatu::IPlayQueue::RepeatType repeatType ) = 0;

    /**
      * @brief Moves one or more media items in the playlist from one index to
      * the other.
      * @param origIndex current index for moving
      * @param newIndex index after moving
      * @param numItems number of items to move
      */
    virtual void moveItem( int origIndex, int newIndex, int numItems ) = 0;



protected:

    /**
     * @brief IPlayQueue Constructor that does nothing except setting the
     * parent
     * @param parent of this object
     */
    IPlayQueue( QObject *parent = 0 )
        : QObject( parent ) { /*NOTHING TO DO*/ }
};

} //end of ns
