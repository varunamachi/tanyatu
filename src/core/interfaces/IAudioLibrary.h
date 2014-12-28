/*******************************************************************************
 * IAudioLibrary.h
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
