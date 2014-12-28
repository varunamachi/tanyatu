/*******************************************************************************
 * ITrackInformationManager.h
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
#include "../data/ArtistInfo.h"
#include "../data/AlbumInfo.h"
#include "../data/StoredAudio.h"
#include "../TanyatuCoreGlobal.h"

namespace Tanyatu {

class TANYATU_CORE_EXPORT ITrackInformationManager : public QObject,
                                                     public IComponent
{
    Q_OBJECT
signals:
    void artistAdded( Tanyatu::Data::ArtistInfo *artistInfo );

    void artistRemoved( QString artistName );

    void aboutToRemoveArtist( Tanyatu::Data::ArtistInfo *artistInfo );

    void artistModified( Tanyatu::Data::ArtistInfo *artistInfo );

    void albumAdded( Tanyatu::Data::AlbumInfo *albumInfo );

    void aboutToRemoveAlbum( Tanyatu::Data::AlbumInfo *albumInfo );

    void albumRemoved( QString artistName, QString albumName );

    void albumModified( Tanyatu::Data::AlbumInfo *albumInfo );

    void aboutToClear();

    void cleared();

//    void aboutChangeTrackInfoManager();
//    void trackInfoManagerChanged(); These two doesn't seem nescessary

public slots:

    virtual void onItemPlaybackStarted( Tanyatu::Data::MediaItem *item ) = 0;

    virtual void onItemPlaybackFinished( Tanyatu::Data::MediaItem *item ) = 0;

    virtual void onItemModified( Tanyatu::Data::StoredAudio *item ) = 0;

    virtual void onAdditionToStorage( Tanyatu::Data::StoredAudio *track ) = 0;

    virtual void onRemovalFromStorage(
            Tanyatu::Data::StoredAudio * track ) = 0;

    virtual void rateAlbum( QString artistName,
                            QString albumName,
                            Tanyatu::Data::MediaRating rating ) = 0;

    virtual void rateArtist( QString artistName,
                             Tanyatu::Data::MediaRating rating ) = 0;

    virtual void clear( bool enableSignals = true ) = 0;

    virtual bool removeArtist( QString artistName ) = 0;

    virtual bool removeAlbum( QString albumId ) = 0;

public:

    virtual quint32 allArtist(
            QList< Data::ArtistInfo *> &allArtistOut ) = 0;

    virtual quint32 allAlbums(
            QList< Data::AlbumInfo *> &allAlbumsOut) = 0;

    virtual quint32 albumsFromArtist( QString artistName,
                            QList< Data::AlbumInfo *> &albumOut) = 0;

    virtual quint32 tracksFromArtist( QString artistName,
                            QList< QString > &trackIdsOut ) = 0;

    virtual quint32 tracksFromAlbum( QString albumId,
                                     QList< QString > &trackIdsOut ) = 0;

    virtual Data::AlbumInfo *album( QString artistName,
                                     QString albumName ) = 0;

    virtual Data::AlbumInfo *album( QString albumId ) = 0;

    virtual Data::ArtistInfo *artist( QString artistName ) = 0;

    virtual quint32 numAlbumsFromArtst( QString artistName ) = 0;

    virtual quint32 numTracksFromAlbum(
            QString artistName, QString albumName) = 0;

    virtual quint32 numTracksFromArtist( QString artistName ) = 0;

    virtual quint32 numArtists() = 0;

    virtual quint32 numAlbums() = 0;

    virtual ~ITrackInformationManager() { /*NOTHING TO DO*/ }

protected:
    ITrackInformationManager( QObject *parent )
        : QObject( parent ) { /*NOTHING TO DO*/ }

};

}
