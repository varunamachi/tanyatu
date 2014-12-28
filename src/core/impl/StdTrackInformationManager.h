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
