/*******************************************************************************
 * StdAudioLibrary.cpp
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

#include <QSize>
#include <QFont>

#include "StdAudioLibrary.h"
#include "../T.h"
#include "../coreutils/Utils.h"
#include "../data/MediaItem.h"
#include "../coreutils/JobManager.h"


namespace Tanyatu { namespace Impl {


QString StdAudioLibrary::COMPONENT_NAME =
        QString( "Tanyatu::Core::StdAudioLibrary" );

QString StdAudioLibrary::COMPONENT_MODULE = QString( "Tanyatu::Core" );

StdAudioLibrary::StdAudioLibrary( QObject *parent )
    : IAudioLibrary( parent )
    , m_valid( false )
{
}


bool StdAudioLibrary::init()
{
    bool result = false;
    emit aboutToChangeLibrary();
    if( DATA_RETRIEVER()->getStoredAudio( m_tracks )) {
        emit aboutToChangeLibrary();
        m_valid = true;
        result = true;
    }
    emit libraryChanged();
    return result;
}

void StdAudioLibrary::add( Data::StoredAudio *item )
{
    if( item && ! m_tracks.contains( item->trackId() )) {
        emit aboutToChangeLibrary();
        m_tracks.insert( item->id(), item );
        emit itemAdded( item );
        emit libraryChanged();

        auto task = [ item ]() -> Tanyatu::RespFunc
        {
            DATA_UPDATER()->add( item );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "add_track", task );
    }
}

void StdAudioLibrary::add( QList< Tanyatu::Data::StoredAudio *> &items )
{
    if( ! items.empty() ) {
        emit aboutToChangeLibrary();
        foreach( Data::StoredAudio *track, items ) {
            if( ! m_tracks.contains( track->id() )) {
                m_tracks.insert( track->id(), track );
                auto task = [ track ]() -> Tanyatu::RespFunc
                {
                    DATA_UPDATER()->add( track );
                    return nullptr;
                };
                JOB_MANAGER()->addJob( "add_track", task );
                emit itemAdded( track );
            }
        }
        emit libraryChanged();
    }
}


void StdAudioLibrary::remove( QString id )
{
    Data::StoredAudio *track = m_tracks.value( id );
    if( track ) {
        emit aboutToChangeLibrary();
        emit aboutToRemoveItem( track );
        QList< QString > *trackIds = new QList< QString >();
        trackIds->append( id );
        m_tracks.remove( id );
        emit itemRemoved( id );
        emit libraryChanged();

        auto task = [ trackIds, track ]() -> Tanyatu::RespFunc
        {
            AT_SCOPE_EXIT( delete trackIds; delete track );
            DATA_UPDATER()->removeAudioTracks( *trackIds );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "remove_from_audiolib", task );
    }
}

void StdAudioLibrary::remove( QList< QString > ids )
{
    emit aboutToChangeLibrary();
    auto trackList = new QList< Data::MediaItem *>();
    auto trackIdList = new QList< QString >( ids );
    foreach( QString id, ids ) {
        Data::StoredAudio *track = item( id );
        if( track ) {
            emit aboutToRemoveItem( track );
            m_tracks.remove( id );
            emit itemRemoved( id );
        }
    }
    auto task = [ trackList, trackIdList ]() -> Tanyatu::RespFunc
    {
        AT_SCOPE_EXIT( delete trackList; delete trackIdList );
        DATA_UPDATER()->removeAudioTracks( *trackIdList );
        foreach( Data::MediaItem *item, *trackList ) {
            delete item;
        }
        return nullptr;
    };
    JOB_MANAGER()->addJob( "remove_from_audiolib", task );

    emit libraryChanged();
}


void StdAudioLibrary::rescan()
{
    emit requestRescan();
}


void StdAudioLibrary::setRating( QString id,
                                 Tanyatu::Data::MediaRating rating )
{
    Data::StoredAudio *track = m_tracks.value( id );
    if( track ) {
        emit aboutToChangeLibrary();
        track->setRating( rating );
        emit itemChanged( track );
        emit libraryChanged();
    }
}


void StdAudioLibrary::updateItem( QString id,
                                  Tanyatu::Data::AudioTrack &updateInfo )
{
    Data::StoredAudio *track = m_tracks.value( id );
    if( track ) {
        emit aboutToChangeLibrary();
        track->setMetadata( updateInfo.title(),
                            updateInfo.artist(),
                            updateInfo.album(),
                            updateInfo.date(),
                            updateInfo.genre(),
                            updateInfo.descreption(),
                            updateInfo.trackNumber(),
                            updateInfo.duration(),
                            updateInfo.bitRate(),
                            updateInfo.sampleRate(),
                            updateInfo.channels() );
        emit libraryChanged();
        emit itemChanged( track );

        auto task = [ track ]() -> Tanyatu::RespFunc
        {
            DATA_UPDATER()->update( track );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "update_audio_track", task );
    }
}


void StdAudioLibrary::onItemPlaybackStarted( Tanyatu::Data::MediaItem *item )
{
    if( item && item->type() == Data::Media_StoredAudio ) {
        emit aboutToChangeLibrary();
        Data::StoredAudio *track = dynamic_cast< Data::StoredAudio *>( item );
        track->setPlayCount( track->playCount() + 1 );
        emit itemChanged( track );
        emit libraryChanged();

        auto task = [ track ]() -> Tanyatu::RespFunc
        {
            DATA_UPDATER()->updatePlayCount( track );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "update_audio_track", task );
    }
}


void StdAudioLibrary::onItemPlaybackFinished( Tanyatu::Data::MediaItem *item )
{
    Q_UNUSED( item );
}


void StdAudioLibrary::tagItem( QString id, QString tag )
{
    Data::StoredAudio *track = m_tracks.value( id );
    if( track ) {
        emit aboutToChangeLibrary();
        track->addTag( tag );
        emit itemChanged( track );
        emit libraryChanged();

        auto task = [ track ]() -> Tanyatu::RespFunc
        {
            DATA_UPDATER()->updateTags( track );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "update_track_tags", task );
    }
}

void StdAudioLibrary::removeItemTag( QString id, QString tag )
{
    Data::StoredAudio *track = m_tracks.value( id );
    if( track ) {
        emit aboutToChangeLibrary();
        track->removeTag( tag );
        emit itemChanged( track );
        emit libraryChanged();

        auto task = [ track ]() -> Tanyatu::RespFunc
        {
            DATA_UPDATER()->updateTags( track );
            return nullptr;
        };
        JOB_MANAGER()->addJob( "update_track_tags", task );
    }
}


StdAudioLibrary::~StdAudioLibrary()
{
    emit aboutToChangeLibrary();
    emit aboutToClear();
    for( auto it = m_tracks.begin(); it != m_tracks.end(); ++ it ) {
        delete it.value();
    }
    m_tracks.clear();
    emit cleared();
    emit libraryChanged();
}


void StdAudioLibrary::setValid( bool value )
{
    m_valid = value;
}


void StdAudioLibrary::clearLibrary()
{
    if( m_valid ) {
        emit aboutToChangeLibrary();
        emit aboutToClear();
        QList< Data::StoredAudio *> *allTracks = nullptr;
        allTracks = new QList< Data::StoredAudio *>();
        std::copy( m_tracks.begin(), m_tracks.end(), allTracks->begin() );
        m_tracks.clear();
        emit cleared();
        emit libraryChanged();

        auto task = [ allTracks ]() -> Tanyatu::RespFunc
        {
            AT_SCOPE_EXIT( delete allTracks );
            for( auto it = allTracks->begin(); it != allTracks->end(); ++ it ) {
                delete (*it);
            }
            return nullptr;
        };
        JOB_MANAGER()->addJob( "clear_library", task );
    }
}


bool StdAudioLibrary::hasItem( const QString itemId )
{
    return m_tracks.contains( itemId );
}


Data::StoredAudio *StdAudioLibrary::item( QString id ) const
{
    return m_tracks.value( id );
}


quint32 StdAudioLibrary::items( QList< QString > &ids,
                        QHash< QString, Data::StoredAudio *> &itemsOut ) const
{
    quint32 itemCount = 0;
    foreach( QString trackId, ids ) {
        Data::StoredAudio *track = m_tracks.value( trackId );
        if( track ) {
            itemsOut.insert( track->id(), track );
            ++ itemCount;
        }
    }
    return itemCount;
}


quint32 StdAudioLibrary::items( QList< QString > &ids,
                                QList< Data::MediaItem *> &itemsOut ) const
{
    quint32 itemCount = 0;
    foreach( QString trackId, ids ) {
        Data::StoredAudio *track = m_tracks.value( trackId );
        if( track ) {
            itemsOut.append( track );
            ++ itemCount;
        }
    }
    return itemCount;
}


quint32 StdAudioLibrary::items(QList<QString> &ids,
                         QList<Tanyatu::Data::StoredAudio *> &itemsOut) const
{
    quint32 itemCount = 0;
    foreach( QString trackId, ids ) {
        Data::StoredAudio *track = m_tracks.value( trackId );
        if( track ) {
            itemsOut.append( track );
            ++ itemCount;
        }
    }
    return itemCount;
}


//Data::StoredAudio *StdAudioLibrary::item( int index ) const
//{
//    if( index < m_itemList->size() ) {
//        return m_itemList->at( index );
//    }
//    return 0;
//}


quint32 StdAudioLibrary::allItems( QList< Data::StoredAudio *> &out ) const
{
    out.append( m_tracks.values() );
    return out.size();
}


quint32 StdAudioLibrary::itemCount() const
{
    return m_tracks.size();
}


QString StdAudioLibrary::uniqueName() const
{
    return COMPONENT_NAME;
}


QString StdAudioLibrary::module() const
{
    return COMPONENT_MODULE;
}


QString StdAudioLibrary::displayName() const
{
    return tr("Audio Library");
}


} } //End of namespaces
