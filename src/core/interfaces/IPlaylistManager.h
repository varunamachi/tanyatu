/*******************************************************************************
 * AbstractPlaylistManager.h
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

#include "IComponent.h"
#include "../data/SavedPlaylist.h"
#include "../data/MediaItem.h"
#include "../data/ModelTypes.h"


namespace Tanyatu {


class TANYATU_CORE_EXPORT IPlaylistManager : public QObject,
                                             public IComponent
{
    Q_OBJECT
protected:
    IPlaylistManager( QObject *parent )
        : QObject( parent ) { /*NOTHING TO DO*/}

signals:
    void playlistContentModified( Tanyatu::Data::SavedPlaylist *playlist );

    void playlistModified( Tanyatu::Data::SavedPlaylist *playlist );

    void playlistCreated( Tanyatu::Data::SavedPlaylist *playlist );

    void aboutToRemovePlaylist( Tanyatu::Data::SavedPlaylist *playlist );

    void playlistRemoved( QString playlistName );

    void aboutToClear();

    void cleared();

    void aboutToChangePlaylistManager();

    void playlistManagerChanged();

public slots:
    virtual void appendToPlaylist( QString playlistName,
                                   QList< QString > &trackIds ) = 0;

    virtual void appendToPlaylist( QString playlistName,
                                   QString trackId ) = 0;

    virtual bool deletePlaylist( QString playlistName ) = 0;

    virtual bool createPlaylist( QString playlist) = 0;

    virtual bool setTrackIdList( QString playlistName,
                                 QList< QString > &list)  = 0;

    virtual bool clearPlaylist( QString playlistName ) = 0;

    virtual bool deleteAllPlaylists() = 0;

    virtual bool removeTracksFromPlaylist( QString playlistId,
                                           QList< int > &trackIndices ) = 0;

    virtual void clear() = 0;

    virtual void tagItem( QString playlistName, QString tag ) = 0;

    virtual void removeItemTag( QString playlistName, QString tag ) = 0;

    virtual void onPlaylistSelected( QString playlistName ) = 0;

    virtual void ratePlaylist( QString playlistName,
                               Tanyatu::Data::MediaRating rating ) = 0;

    virtual void onItemRemovalFromStorage( QString trackId ) = 0;

public:

    virtual bool hasPlaylist( QString playlistName ) = 0;

    virtual const QList< QString > trackIdsIn( QString playlistName ) = 0;

    virtual quint32 numTracksIn( QString playlistName ) const = 0;

    virtual quint32 allPlaylists(
                       QList< Data::SavedPlaylist *>& playlists ) = 0;

    virtual Tanyatu::Data::SavedPlaylist * playlist( QString plName ) const= 0;

    virtual ~IPlaylistManager() { /*NOTHING TO DO */}

};

}
