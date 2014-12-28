/*******************************************************************************
 * StdPlaylistManager.h
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
#include <QList>

#include "../data/SavedPlaylist.h"
#include "../interfaces/IPlaylistManager.h"
#include "../data/AudioTrack.h"


namespace Tanyatu { namespace Impl {


class TANYATU_CORE_EXPORT StdPlaylistManager : public IPlaylistManager
{
    Q_OBJECT

public:
    StdPlaylistManager( QObject *parent = 0 );

    virtual ~StdPlaylistManager();

    void appendToPlaylist( QString playlistName, QList< QString > &trackIds );

    void appendToPlaylist( QString playlistName, QString trackId );

    bool deletePlaylist( QString playlistName );

    bool createPlaylist( QString playlistName);

    bool setTrackIdList( QString playlistName,
                       QList< QString > &list);

    bool clearPlaylist( QString playlistName );

    bool deleteAllPlaylists();

    bool removeTracksFromPlaylist( QString playlistId,
                                   QList< int > &trackIndices );

    void clear();

    void onPlaylistSelected( QString playlistName );

    void onItemRemovalFromStorage( QString trackId );

    void ratePlaylist( QString playlistName,
                               Data::MediaRating rating);

    void tagItem( QString playlistName, QString tag );

    void removeItemTag( QString playlistName, QString tag );


    bool hasPlaylist( QString playlistName );

    Tanyatu::Data::SavedPlaylist * playlist( QString plName ) const;

    const QList< QString > trackIdsIn( QString playlistName );


    quint32 numTracksIn( QString playlistName ) const;

    quint32 allPlaylists(
                       QList<Data::SavedPlaylist *> &playlists );

    QString uniqueName() const;

    QString module()  const;

    QString displayName() const;

    bool init();

    static QString COMPONENT_NAME;

    static QString COMPONENT_MODULE;

private:
    void loadPlaylist( const QString playlistName );

    QHash< QString, Data::SavedPlaylist *> m_playlists;

    QHash< QString, QList< QString > *> m_playlistToTrackIds;


};
}
}

