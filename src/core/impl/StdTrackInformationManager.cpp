/*******************************************************************************
 * StdTrackInformationManager.cpp
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

#include "StdTrackInformationManager.h"
#include "../coreutils/Utils.h"
#include "../T.h"

namespace Tanyatu { namespace Impl {


QString StdTrackInformationManager::UNKNOWN_ALBUM = "unknown_album";

QString StdTrackInformationManager::UNKNOWN_ARTIST = "unknown_artist";

QString StdTrackInformationManager::COMPONENT_NAME =
        "Tanyatu::Core::StdTrackInformationManager";

QString StdTrackInformationManager::COMPONENT_MODULE =
        "Tanyatu::Core";


StdTrackInformationManager::StdTrackInformationManager( QObject *parent )
    : ITrackInformationManager( parent )
    , m_artistCount( 0 )
    , m_albumCount( 0 )
{
}


bool StdTrackInformationManager::init()
{
    bool result = true;
    result &= DATA_RETRIEVER()->getAllArtists( m_artists );
    result &= DATA_RETRIEVER()->getAllAlbums( m_albums );

    QHash< QString, Data::ArtistInfo *>::iterator artistIt = m_artists.begin();
    for( ; artistIt != m_artists.end(); ++ artistIt ) {
        QSet< QString > *albumIds = new QSet< QString >();
        result &= DATA_RETRIEVER()->getAlbumIdsFromArtist(
                    artistIt.key(), ( *albumIds ));
        m_artistToAlbumIds.insert( artistIt.key(), albumIds );
    }

    QHash< QString, Data::AlbumInfo *>::iterator albumIt = m_albums.begin();
    for( ; albumIt != m_albums.end(); ++ albumIt) {
        QSet< QString > *trackIds = new QSet< QString >();
        Data::AlbumInfo *album = albumIt.value();
        result &= DATA_RETRIEVER()->getTrackIdsFromAlbum(
                    album->artistName(), album->name(), ( *trackIds ));
        m_albumIdToTracks.insert( album->id(), trackIds );
    }
    return result;
}


void StdTrackInformationManager::onItemPlaybackStarted(
        Tanyatu::Data::MediaItem *item )
{
    Data::StoredAudio *track = AUDIO_LIB()->item( item->trackId() );
    if( track ) {
        Data::ArtistInfo *artist = this->artist( track->artist() );
        Data::AlbumInfo *album = this->album( track->artist(),track->album() );
        if( artist && album ) {
            artist->setPlayCount( artist->playCount() + 1);
            album->setPlayCount( album->playCount() + 1);
            emit artistModified( artist );
            emit albumModified( album );

            auto task = [ artist, album ]() -> Tanyatu::RespFunc
            {
                DATA_UPDATER()->updatePlayCount( artist );
                DATA_UPDATER()->updatePlayCount( album );
                return nullptr;
            };
            JOB_MANAGER()->addJob( "update_artist_album", task );
        }
    }
}


void StdTrackInformationManager::onItemPlaybackFinished(
        Tanyatu::Data::MediaItem *item )
{
    //Nothing as of now...
    Q_UNUSED( item );
}


void StdTrackInformationManager::onItemModified(
        Tanyatu::Data::StoredAudio *item )
{
    //Nothing as of now. This will require item before the change
    //We'll see later.....
    // This method will be important when we allow editing metadata
    Q_UNUSED( item );
}


void StdTrackInformationManager::onAdditionToStorage(
        Tanyatu::Data::StoredAudio *track)
{
    Data::ArtistInfo *artist = this->artist( track->artist() );
    Data::AlbumInfo *album = this->album( track->artist(),
                                           track->album() );
    auto objects = new QList< Data::StoredObject *>();

    if( ! artist ) {
        artist = new Data::ArtistInfo( track->artist() );
        m_artists.insert( artist->name(), artist );
        m_artistToAlbumIds.insert( artist->name(), new QSet< QString >);
        ++ m_artistCount;
        emit artistAdded( artist );
        objects->append( artist );
    }
    if( ! album ) {
        album = new Data::AlbumInfo( track->artist(), track->album() );
        m_albumIdToTracks.insert( album->id(), new QSet< QString >() );
        ++ m_albumCount;
        emit albumAdded( album );
    }

    QSet< QString > *albumsIds =
            m_artistToAlbumIds.value( artist->name() );
    if( ! albumsIds->contains( album->id() )) {
        albumsIds->insert( album->id() );
        m_albums.insert( album->id(), album );
        objects->append( album );
    }
    m_albumIdToTracks.value( album->id() )->insert( track->id() );
    auto task = [ objects ]() -> Tanyatu::RespFunc
    {
        AT_SCOPE_EXIT( delete objects );
        DATA_UPDATER()->add( *objects );
        return nullptr;
    };
    JOB_MANAGER()->addJob( "add_artist_album", task );

}


void StdTrackInformationManager::onRemovalFromStorage(
        Tanyatu::Data::StoredAudio * track )
{
    QString artistName = track->artist();
    QString albumName = track->album();
    Data::ArtistInfo *artist = this->artist( artistName );
    Data::AlbumInfo *album = this->album( artistName, albumName );
    QString albumId = album->id();
    if( artist && album ) {
        QSet< QString > *tracks = m_albumIdToTracks.value( albumId );
        if( tracks->contains( track->id() )) {
            tracks->remove( track->id() );
            if( tracks->empty() ) {
                QSet< QString > *albums = m_artistToAlbumIds.value(
                            artistName );
                emit aboutToRemoveAlbum( album );
                m_albumIdToTracks.remove( albumId );
                m_albums.remove( albumId );
                albums->remove( albumId );
                if( albums->empty() ) {
                    emit aboutToRemoveArtist( artist );
                    m_artists.remove( artistName );
                    m_artistToAlbumIds.remove( artistName );
                    delete albums;
                    delete artist;
                    --m_artistCount;
                    emit artistRemoved( artistName );
                }
                delete tracks;
                delete album;
                --m_albumCount;
                emit albumRemoved( artistName, albumName );

                auto task = [ artist, album, tracks ]() -> Tanyatu::RespFunc
                {
                    AT_SCOPE_EXIT( delete artist; delete album;  delete tracks);
                    DATA_UPDATER()->remove( artist );
                    DATA_UPDATER()->remove( album );
                    return nullptr;
                };
                JOB_MANAGER()->addJob( "remove_artist_album", task );

            }
        }
    }
}


void StdTrackInformationManager::onAdditionToStorage(
        QList< Tanyatu::Data::StoredAudio *> &items )
{
    auto objects = new QList< Data::StoredObject *>();

    foreach( Data::StoredAudio *track, items) {
        Data::ArtistInfo *artist = this->artist( track->artist() );
        Data::AlbumInfo *album = this->album( track->artist(),
                                               track->album() );
        if( artist != nullptr ) {
            artist = new Data::ArtistInfo( track->artist() );
            m_artists.insert( artist->name(), artist );
            m_artistToAlbumIds.insert( artist->name(), new QSet< QString >);
            ++ m_artistCount;
            objects->append( artist );
            emit artistAdded( artist );
        }
        if( album != nullptr ) {
            album = new Data::AlbumInfo( track->artist(), track->album() );
            m_albumIdToTracks.insert( album->id(), new QSet< QString >() );
            ++ m_albumCount;
            emit albumAdded( album );
        }

        QSet< QString > *albumsIds =
                m_artistToAlbumIds.value( artist->name() );
        if( ! albumsIds->contains( album->id() )) {
            albumsIds->insert( album->id() );
            m_albums.insert( album->id(), album );
            objects->append( artist );
        }
        m_albumIdToTracks.value( album->id() )->insert( track->id() );
    }

    auto task = [ objects ]() -> Tanyatu::RespFunc
    {
        AT_SCOPE_EXIT( delete objects );
        DATA_UPDATER()->add( *objects );
        return nullptr;
    };
    JOB_MANAGER()->addJob( "add_artist_album", task );
}


void StdTrackInformationManager::onRemovalFromStorage(
        QList< Tanyatu::Data::StoredAudio *> &items )
{
    auto objects = new QList< Data::StoredObject *>();
    auto sets = new QList< QSet< QString > *>();

    foreach( Data::StoredAudio *track, items ) {
        QString artistName = track->artist();
        QString albumName = track->album();
        Data::ArtistInfo *artist = this->artist( artistName );
        Data::AlbumInfo *album = this->album( artistName, albumName );
        QString albumId = album->id();
        if( artist && album ) {
            QSet< QString > *tracks = m_albumIdToTracks.value( albumId );
            if( tracks->contains( track->id() )) {
                tracks->remove( track->id() );
                if( tracks->empty() ) {
                    QSet< QString > *albums = m_artistToAlbumIds.value(
                                artistName );
                    emit aboutToRemoveAlbum( album );
                    m_albumIdToTracks.remove( albumId );
                    m_albums.remove( albumId );
                    albums->remove( albumId );
                    if( albums->empty() ) {
                        emit aboutToRemoveArtist( artist );
                        m_artists.remove( artistName );
                        m_artistToAlbumIds.remove( artistName );
                        sets->append( albums );
                        objects->append( artist );
                        --m_artistCount;
                        emit artistRemoved( artistName );
                    }

                    sets->append( tracks );
                    objects->append( album );
                    --m_albumCount;
                    emit albumRemoved( artistName, albumName );
                }
            }
        }
    }
    auto task = [ objects, sets ]() -> Tanyatu::RespFunc
    {
        AT_SCOPE_EXIT( delete objects; delete sets );
        DATA_UPDATER()->remove( *objects );
        Tanyatu::Utils::deleteAllFormList< Data::StoredObject >( *objects );
        Tanyatu::Utils::deleteAllFormList< QSet< QString > >( *sets );
        return nullptr;
    };
    JOB_MANAGER()->addJob( "remove_artist_album", task );
}


bool StdTrackInformationManager::removeArtist( QString artistName )
{
    bool result = false;

    QList< QString > trackIds;
    Data::ArtistInfo *artist = m_artists.value( artistName );
    QList< Data::AlbumInfo *> albums;
    if( artist ) {
        auto objects = new QList< Data::StoredObject *>();
        auto sets = new QList< QSet< QString > *>();
        objects->append( artist );

        emit aboutToRemoveArtist( artist );
        tracksFromArtist( artistName, trackIds );
        albumsFromArtist( artistName, albums );
        AUDIO_LIB()->remove( trackIds );
        foreach( Data::AlbumInfo *alb, albums ) {
            emit aboutToRemoveAlbum( alb );
            sets->append( m_albumIdToTracks.value( alb->id() ));
            m_albumIdToTracks.remove( alb->id() );
            m_albums.remove( alb->id() );
            -- m_albumCount;
            emit albumRemoved( alb->artistName(), alb->name() );
            objects->append( alb );
        }
        emit aboutToRemoveArtist( artist );
        sets->append( m_artistToAlbumIds.value( artist->name() ));
        m_artistToAlbumIds.remove( artist->name() );
        m_artists.remove( artist->name() );
        objects->append( artist );
        -- m_artistCount;
        emit artistRemoved( artistName );
        result = true;

        auto task = [ artistName, objects, sets ]() -> Tanyatu::RespFunc
        {
            AT_SCOPE_EXIT( delete objects; delete sets  );
            DATA_UPDATER()->remove( *objects );
            Tanyatu::Utils::deleteAllFormList< Data::StoredObject >( *objects );
            Tanyatu::Utils::deleteAllFormList< QSet< QString > >( *sets );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "remove_artist", task );
    }
    return result;
}


bool StdTrackInformationManager::removeAlbum( QString albumId )
{
    QList< QString > trackIds;
    Data::AlbumInfo *album = m_albums.value( albumId );
    bool result = false;
    if( album ) {
        auto sets = new QList< QSet< QString >*>();
        auto objects = new QList< Tanyatu::Data::StoredObject *>();
        objects->append( album );

        tracksFromAlbum( album->id(), trackIds );
        AUDIO_LIB()->remove( trackIds );
        QSet< QString > *allAlbums = m_artistToAlbumIds.value(
                    album->artistName() );
        emit aboutToRemoveAlbum( album );
        sets->append( m_albumIdToTracks.value( albumId ));
        m_albumIdToTracks.remove( albumId );
        m_albums.remove( albumId );
        allAlbums->remove( albumId );
        -- m_albumCount;
        emit albumRemoved( album->artistName(), album->name() );
        if( allAlbums->empty() ) {
            QString artistName = album->artistName();
            emit aboutToRemoveArtist( m_artists.value(
                                          album->artistName() ));
            sets->append( allAlbums );
            m_artistToAlbumIds.remove( album->artistName() );
            objects->append( m_artists.value( album->artistName() ));
            m_artists.remove( album->artistName() );
            -- m_artistCount;
            emit artistRemoved( artistName );
        }
        objects->append( album );
        result = true;

        auto task = [ albumId, objects, sets ]() -> Tanyatu::RespFunc
        {
            AT_SCOPE_EXIT( delete objects; delete sets  );
            DATA_UPDATER()->remove( *objects );
            Tanyatu::Utils::deleteAllFormList< Data::StoredObject >( *objects );
            Tanyatu::Utils::deleteAllFormList< QSet< QString > >( *sets );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "remove_album", task );
    }
    return result;
}

void StdTrackInformationManager::clear( bool enableSignals )
{
    if( enableSignals ) {
        emit aboutToClear();
    }
    auto sets = new QList< QSet< QString > *>();
    auto objects = new QList< Data::StoredObject *>();

    for( QHash< QString, Data::ArtistInfo *>::iterator it = m_artists.begin();
         it != m_artists.end();
         ++ it) {
        QString artistName = it.key();
        QSet< QString > *albumIds = m_artistToAlbumIds.value( artistName );
        foreach( QString id, ( *albumIds )) {
            QSet< QString > *tracks = m_albumIdToTracks.value( id );
//            tracks->clear();
            sets->append( tracks );
            objects->append( m_albums.value( id ));
        }
        albumIds->clear();
        sets->append( albumIds );
        objects->append( it.value());
    }
    m_albumIdToTracks.clear();
    m_artistToAlbumIds.clear();
    m_albums.clear();
    m_artists.clear();
    if( enableSignals ) {
        emit cleared();
    }
    auto task = [ objects, sets ]() -> Tanyatu::RespFunc
    {
        AT_SCOPE_EXIT( delete objects; delete sets );
        Tanyatu::Utils::deleteAllFormList< Data::StoredObject >( *objects );
        Tanyatu::Utils::deleteAllFormList< QSet< QString > >( *sets );
        return nullptr;
    };
    JOB_MANAGER()->addJob( "clear_artist_album_info", task );
}


void StdTrackInformationManager::rateAlbum( QString artistName,
                                     QString albumName,
                                     Tanyatu::Data::MediaRating rating )
{
    Data::AlbumInfo *album = this->album( artistName, albumName );
    if( album ) {
        album->setRating( rating );
        emit albumModified( album );
        auto task = [ album ]() -> Tanyatu::RespFunc
        {
            DATA_UPDATER()->updateRating( album );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "update_album", task );
    }
}


void StdTrackInformationManager::rateArtist( QString artistName,
                                             Tanyatu::Data::MediaRating rating )
{
    Data::ArtistInfo *artist = this->artist( artistName );
    if( artist ) {
        artist->setRating( rating );
        emit artistModified( artist );
    }
    auto task = [ artist ]() -> Tanyatu::RespFunc
    {
            DATA_UPDATER()->updateRating( artist );
            return nullptr;
    };
    JOB_MANAGER()->addJob( "update_artist", task );

}



Data::AlbumInfo *StdTrackInformationManager::album( QString artistName,
                                               QString albumName )
{
    if( ! m_artists.contains( artistName )) {
        artistName = UNKNOWN_ARTIST;
    }
    QString albumId = Data::AlbumInfo::getAlbumIdFrom( artistName, albumName );
    if( ! m_albums.contains( albumId )) {
        albumName = UNKNOWN_ALBUM;
    }
    Data::AlbumInfo *info = m_albums.value( albumId );
    return info;
}

Data::AlbumInfo *StdTrackInformationManager::album( QString albumId )
{
    return m_albums.value( albumId );
}


Data::ArtistInfo *StdTrackInformationManager::artist( QString artistName )
{
    Data::ArtistInfo *artist = m_artists.value( artistName );
    return artist ? artist : m_artists.value( UNKNOWN_ARTIST );
}


StdTrackInformationManager::~StdTrackInformationManager()
{
    clear( false );
}

quint32 StdTrackInformationManager::allArtist(
        QList< Data::ArtistInfo *> &allArtistOut )
{
    for( QHash< QString, Data::ArtistInfo *>::iterator it = m_artists.begin();
         it != m_artists.end();
         ++ it) {
        allArtistOut.append( it.value() );
    }
    return m_artistCount;
}


quint32 StdTrackInformationManager::allAlbums(
        QList< Data::AlbumInfo *> &allAlbumsOut )
{
    for( QHash< QString, Data::AlbumInfo *>::iterator it = m_albums.begin();
         it != m_albums.end();
         ++ it) {
        allAlbumsOut.append( it.value() );
    }
    return m_albumCount;
}


quint32 StdTrackInformationManager::albumsFromArtist(
        QString artistName,
        QList< Data::AlbumInfo *> &albumsOut )
{
    quint32 albumCount = 0;
    if( m_artistToAlbumIds.contains( artistName )) {
        QSet< QString > *albums = m_artistToAlbumIds.value( artistName );
        foreach( QString id, ( *albums )) {
            if( m_albums.contains( id )) {
                albumsOut.append( m_albums.value( id ));
                ++ albumCount;
            }
        }
    }
    return albumCount;
}


quint32 StdTrackInformationManager::tracksFromAlbum( QString albumId,
                                               QList<QString> &trackIdsOut )
{
    quint32 trackCount = 0;
    if( m_albumIdToTracks.contains( albumId )) {
        QSet< QString > *trackIds = m_albumIdToTracks.value( albumId );
        foreach( QString id, ( *trackIds )) {
            trackIdsOut.append( id );
            ++ trackCount;
        }
    }
    return trackCount;
}



quint32 StdTrackInformationManager::tracksFromArtist(
        QString artistName,
        QList< QString > &trackIdsOut )
{
    quint32 trackCount = 0;
    if( m_artists.contains( artistName )) {
        QList< Data::AlbumInfo *> albums;
        albumsFromArtist( artistName, albums );
        if( ! albums.empty() ) {
            foreach( Data::AlbumInfo *albumInfo, albums ) {
                trackCount += tracksFromAlbum( albumInfo->id(),
                                               trackIdsOut );
            }
        }
    }
    return trackCount;
}

quint32 StdTrackInformationManager::numAlbumsFromArtst( QString artistName )
{
    if( m_artistToAlbumIds.contains( artistName )) {
        return m_artistToAlbumIds.value( artistName )->size();
    }
    return 0;
}


quint32 StdTrackInformationManager::numTracksFromAlbum(QString artistName,
                                                 QString albumName)
{
    QString albumId = Data::AlbumInfo::getAlbumIdFrom( artistName, albumName );
    if( m_albumIdToTracks.contains( albumId )) {
        return m_albumIdToTracks.value( albumId )->size();
    }
    return 0;
}


quint32 StdTrackInformationManager::numTracksFromArtist(QString artistName)
{
    quint32 numTracks = 0;
    if( m_artistToAlbumIds.contains( artistName )) {
        QSet< QString > *albumIds = m_artistToAlbumIds.value( artistName );
        foreach( QString id, ( *albumIds )) {
            if( m_albumIdToTracks.contains( id )) {
                numTracks += m_albumIdToTracks.value( id )->size();
            }
        }
    }
    return numTracks;
}


quint32 StdTrackInformationManager::numArtists()
{
    return m_artistCount;
}


quint32 StdTrackInformationManager::numAlbums()
{
    return m_albumCount;
}


QString StdTrackInformationManager::uniqueName() const
{
    return COMPONENT_NAME;
}


QString StdTrackInformationManager::module() const
{
    return COMPONENT_MODULE;
}

QString StdTrackInformationManager::displayName() const
{
    return tr( "Audio" );
}


} } //end of ns
