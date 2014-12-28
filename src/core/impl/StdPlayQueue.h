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
#pragma once

#include "../interfaces/IPlayQueue.h"
#include "../TanyatuCoreGlobal.h"

#include <QList>
#include <QSet>


namespace Tanyatu { namespace Impl {

/**
  * \class StdPlayQueue default playlist implementation of Tanyatu playlist
  * interface
  */
class TANYATU_CORE_EXPORT StdPlayQueue : public IPlayQueue
{
    Q_OBJECT

public:
    explicit StdPlayQueue( QObject *parent = 0);


    /**
      * Inherited docs
      */
    void addItem( const Tanyatu::Data::MediaItem *item, bool selected = true );

    /**
      * Inherited docs
      */
    void addItems( const QList< Tanyatu::Data::MediaItem *> &items );

    /**
      * Inherited docs
      */
    void removeItem( const QString itemId );

    /**
      * Inherited docs
      */
    void removeItem( const Tanyatu::Data::MediaItem *item );

    /**
      * Inherited docs
      */
    void removeItem( int index );

    /**
      * Inherited docs
      */

    void removeItems( const QList< Tanyatu::Data::MediaItem * > &items );
    /**
      * Inherited docs
      */
    void removeItems( QList< int > &indices );

    /**
      * Inherited docs
      */
    void removeItems( int fromIndex, int toIndex );

    /**
     * Inherited docs
     */
    void clearAllStoredItems();

    /**
      * Inherited docs
      */
    void clear();

    /**
      * Inherited docs
      */
    void selectNext();

    /**
      * Inherited docs
      */
    void selectPrevious();

    /**
      * Inherited docs
      */
    void selectIndex( int index );

    /**
      * Inherited docs
      */
    void setRandom( bool value );


    /**
      * Inherited docs
      */
    void insert( int index, const QList< Tanyatu::Data::MediaItem* > &items );

    /**
      * Inherited docs
      */
    void setRepeat(
            Tanyatu::IPlayQueue::RepeatType repeatType );

    /**
      * Inherited docs
      */
    void moveItem( int origIndex, int newIndex, int numItems );

    /**
      * Inherited docs
      */
    Data::MediaItem *current() const;

    /**
      * Inherited docs
      */
    int currentIndex() const;

    /**
      * Inherited docs
      */
    bool hasNext() const;

    /**
      * Inherited docs
      */
    bool hasPrev() const;

    /**
      * Inherited docs
      */
    int numberOfItems() const;

    /**
      * Inherited docs
      */
    bool isRandom() const;

    /**
      * Inherited docs
      */
    RepeatType repeatType() const;

    /**
      * Inherited docs
      */
    const QList< Data::MediaItem * >* getAllItemsInOrder() const;

    //IComponent implementations
    QString uniqueName() const;

    QString module() const;

    QString displayName() const;

    bool init();

private:
    /**
      * Checks the validity of the index
      * \param index for checking
      * \returns true if index is valid else false
      */
    bool isValidIndex( int index ) const;

    /**
      * \fn void StdPlayQueue::addToPrevList( int index ) adds an index to
      * a list that stores the indices of previous tracks. This method makes
      * sure that the list stores only the recesntly played tracks, and starts
      * removing the old entries after the size of the list reaches some
      * predefined size
      */
    void addToPrevList( int index );

    /**
      * Selects a random item from the playlist
      * \returns a index in the playlist randomly selected and unplayed in
      * current play-cycle
      */
    int getRandomIndex();

    /**
      * Gives the indices for the given item
      * \param itemId id of the item for which indices are required
      * \param indices after the function finishes this list contains indices
      * where the given item is present in the playlist
      */
    void getAllIndicesOf( QString itemId, QList< int > &indices ) const;


    /**
      * \variable m_items list of items present in the playlist.
      */
    QList< Data::MediaItem * > m_items;

    /**
      * \variable m_prevItems list of items that have been played.
      */
    QList< int > m_prevIndicesList;


    /**
      * \variable m_unplayedIds set of ids of items that are not yet played.
      */
    QSet< QString > m_unplayedIds;

    /**
      * Index of media item that is selected currently
      */
    int m_currentIndex;

    /**
      * Index of the list that holds the previously played items. When previous
      * list is not used this will be pointing to the last index on the prev
      * index list.
      */
    int m_prevListIndex;


    /**
      * \variable m_random boolean variable if true progression is random
      */
    bool m_random;

    /**
      * \variable m_repeatType specifies the behavior of the playlist once all
      * the items in the playlist finishes
      */
    RepeatType m_repeatType;
};

} } //End of namespaces
