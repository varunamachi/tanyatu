/*******************************************************************************
 * StdTrackInformationManager.h
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

#include "../interfaces/ITrackInformationManager.h"


namespace Tanyatu { namespace Impl {

class TANYATU_CORE_EXPORT StdTrackInformationManager :
        public ITrackInformationManager
{
    Q_OBJECT

public:
    void onItemPlaybackStarted( Tanyatu::Data::MediaItem *item );

    void onItemPlaybackFinished( Tanyatu::Data::MediaItem *item  );

    void onItemModified( Tanyatu::Data::StoredAudio *item );

    void onAdditionToStorage( Tanyatu::Data::StoredAudio *track);

    void onRemovalFromStorage( Tanyatu::Data::StoredAudio *track );

    void onAdditionToStorage( QList< Tanyatu::Data::StoredAudio *> &items );

    void onRemovalFromStorage( QList< Tanyatu::Data::StoredAudio *> &items);

    void rateAlbum( QString artistName,
                            QString albumName,
                            Tanyatu::Data::MediaRating rating );

    void rateArtist( QString artistName,
                             Tanyatu::Data::MediaRating rating );

    void clear( bool enableSignals = true );

    bool removeArtist( QString artistName );

    bool removeAlbum( QString albumId );

    explicit StdTrackInformationManager(QObject *parent = 0);

    quint32 allArtist( QList< Data::ArtistInfo *> &allArtistOut );

    quint32 allAlbums( QList< Data::AlbumInfo *> &allAlbumsOut );

    quint32 albumsFromArtist( QString artistName,
                            QList<Data::AlbumInfo *> &albumOut );

    quint32 tracksFromArtist( QString artistName,
                            QList< QString > &trackIdsOut );

    quint32 tracksFromAlbum( QString albumId,
                             QList< QString > &trackIdsOut );

    quint32 numAlbumsFromArtst( QString artistName );

    quint32 numTracksFromAlbum(
            QString artistName, QString albumName);

    quint32 numTracksFromArtist( QString artistName );

    quint32 numArtists();

    quint32 numAlbums();

    Data::AlbumInfo *album( QString artistName,
                                     QString albumName );

    Data::AlbumInfo *album( QString albumId );

    Data::ArtistInfo *artist( QString artistName );

    virtual ~StdTrackInformationManager();

    QString uniqueName() const;

    QString module()  const;

    QString displayName() const;

    bool init();

    static QString UNKNOWN_ARTIST;

    static QString UNKNOWN_ALBUM;

    static QString COMPONENT_NAME;

    static QString COMPONENT_MODULE;

private:
    QHash< QString, Data::ArtistInfo *> m_artists;

    QHash< QString, Data::AlbumInfo *> m_albums;

    QHash< QString, QSet< QString > *> m_artistToAlbumIds;

    QHash< QString, QSet< QString > *> m_albumIdToTracks;

    quint32 m_artistCount;

    quint32 m_albumCount;


};

} } //end of ns
