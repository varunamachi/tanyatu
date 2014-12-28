/*******************************************************************************
 * IDataRetriever.h
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

#include <QHash>
#include <QSet>
#include <QVariant>

#include "../TanyatuCoreGlobal.h"
#include "../data/SavedPlaylist.h"
#include "../data/AlbumInfo.h"
#include "../data/ArtistInfo.h"
#include "../data/StoredAudio.h"

namespace Tanyatu {

class TANYATU_CORE_EXPORT IDataRetriever
{
public:
    virtual bool getStoredAudio(
            QHash< QString, Data::StoredAudio*> &items ) = 0;

    virtual bool getHistory(
            QList< QPair< QString, QDateTime > *> &recentIds) = 0;

    virtual bool getTrackIdsFromArtist( QString artistName,
                                        QSet< QString > &trackIds ) = 0;

    virtual bool getAlbumIdsFromArtist( QString artistName,
                                        QSet< QString > &albumIds ) = 0;

    virtual bool getTrackIdsFromAlbum( QString artistName,
                                       QString albumName,
                                       QSet< QString > &trackIds ) = 0;

    virtual bool getAllArtists(
            QHash< QString, Data::ArtistInfo *> &artist ) = 0;


    virtual bool getAllAlbums( QHash< QString, Data::AlbumInfo *> &albums) = 0;

    virtual bool getPlaylists(
            QHash< QString, Data::SavedPlaylist *> &playlists) = 0;

    virtual bool getPlaylistContent( QString playlistName,
                                     QList< QString > &trackIds ) = 0;

    virtual int getPlayCount( QString trackId ) = 0;

    virtual Data::MediaRating getRating( QString trackId )            = 0;

    virtual bool getProperty( QString key,
                              QString &typeOut,
                              QString &valueOut )      = 0;

    /**
     * @brief getSavedPlayQueue Gives the content of the play queue that was
     * saved when application exited last time
     * @param list of track_id - track_path pair - is an out param
     */
    virtual int getSavedPlayQueue(
            QList< QPair< QString, QString > > &trackInfoOut ) = 0;

    virtual ~IDataRetriever() { /*NOTHING TO DO*/ }

protected:
    IDataRetriever() { /*NOTHING TO DO*/ }
};

} //End of ns Tanyatu
