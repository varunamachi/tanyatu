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
