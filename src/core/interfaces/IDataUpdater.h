/*******************************************************************************
 * IDateUpdater.h
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
#include <QString>
#include <QDate>

#include "../data/StoredObject.h"
#include "../data/StoredAudio.h"
#include "../data/SavedPlaylist.h"
#include "../data/StoredMediaObject.h"

namespace Tanyatu {

class TANYATU_CORE_EXPORT IDataUpdater
{

public:
    virtual bool add( Data::StoredObject *stored ) = 0;

    virtual bool update( Data::StoredObject *stored ) = 0;

    virtual bool remove( Data::StoredObject *stored ) = 0;

    virtual bool add( QList< Data::StoredObject *> &objList ) = 0;

    virtual bool update( QList< Data::StoredObject *> &objList ) = 0;


    virtual bool remove( QList< Data::StoredObject *> &stored ) = 0;

    virtual bool savePlaylist( QString playlistName,
                               const QList< QString > &ids ) = 0;

    virtual bool addHistory( QString itemId, QDateTime time ) = 0;

    virtual bool setProperty( QString key,
                              QString type,
                              QString value ) = 0;

    virtual bool removeProperty( QString key ) = 0;

    virtual bool clearAudioLibrary() = 0;

    virtual bool clearVideoLibrary() = 0;

    virtual bool clearHistory() = 0;

    virtual bool clearHistory( QDateTime &from ) = 0;

    virtual bool clearPlaylist( QString playlistName ) = 0;

    virtual bool removePlaylist( QString playlistName ) = 0;

    virtual bool removeAudioTracks( QList< QString > &trackIds ) = 0;

    virtual bool removeAudioTrack( QString trackId ) = 0;

    virtual bool removeVideoTrack( QString trackId ) = 0;

    virtual bool removeVideoTracks( QList< QString > &trackId) = 0;

    virtual bool removeArtist( QString artistName ) = 0;

    virtual bool removeAlbum( QString albumId ) = 0;

    virtual bool removeHistoryItem( QString trackId ) = 0;

    virtual bool removeFromHistory( QList< QString > &trackIds ) = 0;

    virtual bool updateRating( Data::StoredMediaObject *stored ) = 0;

    virtual bool updatePlayCount( Data::StoredMediaObject *stored ) = 0;

    virtual bool updateTags( Data::StoredMediaObject *stored ) = 0;

    virtual bool saveCurrentPlaylist(
            const QList< Tanyatu::Data::MediaItem *> &list ) = 0;

    virtual ~IDataUpdater() { /*NOTHING TO DO*/ }

protected:
    IDataUpdater() { /*NOTHING TO DO*/ }
};

} //end of ns
