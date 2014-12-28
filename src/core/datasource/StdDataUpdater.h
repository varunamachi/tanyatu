/*******************************************************************************
 * StandardDataUpdater.h
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

#include "../interfaces/IDataUpdater.h"
#include "../data/StoredVideo.h"
#include "../data/ArtistInfo.h"
#include "../data/AlbumInfo.h"


namespace Tanyatu { namespace Impl {

class TANYATU_CORE_EXPORT StdDataUpdater : public IDataUpdater
{
public:
    StdDataUpdater(){}

    ~StdDataUpdater(){}

    bool add( Data::StoredObject *stored );

    bool update( Data::StoredObject *stored );

    bool remove( Data::StoredObject *stored );

    bool add( QList< Data::StoredObject *> &objList );

    bool update( QList< Data::StoredObject *> &objList );

    bool remove(QList<Data::StoredObject *> &objList );

    bool savePlaylist( QString playlistName, const QList< QString > &ids );

    bool addHistory( QString itemId, QDateTime time );

    //Settings framework will come later, this for sample :P
    bool setProperty( QString key, QString type, QString value );

    bool removeProperty( QString key );

    bool clearAudioLibrary();

    bool clearVideoLibrary();

    bool clearHistory();

    bool clearHistory( QDateTime &from );

    bool clearPlaylist( QString playlistName );

    bool removePlaylist( QString playlistName );

    bool removeAudioTracks( QList< QString > &trackIds );

    bool removeAudioTrack( QString trackId );

    bool removeVideoTracks( QList< QString > &trackIds );

    bool removeVideoTrack( QString trackId );

    bool removeArtist( QString artistName );

    bool removeAlbum( QString albumId );

    bool removeHistoryItem( QString trackId );

    bool removeFromHistory( QList< QString > &trackIds );

    bool updateRating( Data::StoredMediaObject *stored );

    bool updatePlayCount( Data::StoredMediaObject *stored );

    bool updateTags( Data::StoredMediaObject *stored );

    bool saveCurrentPlaylist( const QList< Tanyatu::Data::MediaItem *> &list );

private:
    bool add( Data::StoredAudio *audio );

    bool add( Data::StoredVideo *video );

    bool add( Data::SavedPlaylist *playlist );

    bool add( Data::ArtistInfo *artist );

    bool add( Data::AlbumInfo *album );

    bool update( Data::StoredAudio *audio );

    bool update( Data::StoredVideo *video );

    bool update( Data::SavedPlaylist *playlist );

    bool update( Data::ArtistInfo *artist );

    bool update( Data::AlbumInfo *album );

    QString getTagString( const QSet<QString> &tags );

    QString getTableName( Data::StoredMediaObject *stored );

    QString getUniqueColumnName( Data::StoredMediaObject *stored );

};

} } // End of namespaces

