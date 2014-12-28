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


#include <QObject>
#include <QHash>
#include <QList>

#include "IComponent.h"
#include "../TanyatuCoreGlobal.h"
#include "../data/StoredAudio.h"
#include "../data/ModelTypes.h"



namespace Tanyatu {
/**
 * @brief The IAudioLibrary class defines an interface for an audio
 * library implementation. This class shall hold all the information about
 * the audio tracks that are added to the data source and allow other components
 * to query for the details
 */
class TANYATU_CORE_EXPORT IAudioLibrary : public QObject,
                                          public IComponent
{
    Q_OBJECT

signals:
    void itemAdded( Tanyatu::Data::StoredAudio *track );

    void itemRemoved( QString itemId );

    void aboutToRemoveItem( Tanyatu::Data::StoredAudio *item );

    void aboutToClear();

    void cleared();

    void itemChanged( Tanyatu::Data::StoredAudio *item );

    void ratingChanged( Tanyatu::Data::StoredAudio *item );

    void requestRescan();

    void aboutToChangeLibrary();

    void libraryChanged();

public slots:
    virtual void add( Tanyatu::Data::StoredAudio *item ) = 0;

    virtual void add( QList< Tanyatu::Data::StoredAudio *>& items ) = 0;

    virtual void remove( QString id ) = 0;

    virtual void remove( QList< QString > ids ) = 0;

    virtual void rescan() = 0;

    virtual void setRating( QString id,
                            Tanyatu::Data::MediaRating rating ) = 0;

    virtual void updateItem( QString id,
                             Tanyatu::Data::AudioTrack &updateInfo ) = 0;

    virtual void onItemPlaybackStarted( Tanyatu::Data::MediaItem *item ) = 0;

    virtual void onItemPlaybackFinished( Tanyatu::Data::MediaItem *item ) = 0;

    virtual void tagItem( QString id, QString tag ) = 0;

    virtual void removeItemTag( QString id, QString tag ) = 0;

    virtual void clearLibrary() = 0;

public:

    virtual ~IAudioLibrary() { /*NOTHING TO DO*/ }

    virtual quint32 itemCount() const = 0 ;

    virtual bool hasItem( const QString itemId ) = 0;

    virtual Data::StoredAudio *item( QString id ) const = 0;

    virtual quint32 items( QList< QString > &ids, QHash< QString,
                                 Data::StoredAudio *> &itemsOut) const = 0;

    virtual quint32 items( QList< QString >& ids,
                           QList< Data::StoredAudio *> &itemsOut ) const = 0;

    virtual quint32 items( QList< QString >& ids,
                           QList< Data::MediaItem *> &itemsOut) const = 0;

//    virtual Data::StoredAudio *item( int index ) const = 0;

    virtual quint32 allItems(
            QList< Tanyatu::Data::StoredAudio *> &out ) const = 0;

protected:

    /**
     * @brief IAudioLibrary Constructor
     * @param parent parent for this class
     */
    IAudioLibrary( QObject *parent = 0)
        : QObject( parent ) { /*NOTHING TO DO*/ }


};
}
