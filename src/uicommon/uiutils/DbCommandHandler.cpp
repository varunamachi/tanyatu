/*******************************************************************************
 * DbCommandHandler.cpp
 *
 * Copyright (c) 2013, Varuna L Amachi. All rights reserved.
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

#include <core/T.h>
#include <core/data/StoredAudio.h>
#include <QDebug>

#include "DbCommandHandler.h"



using namespace Tanyatu;
using namespace Worker;

DbCommandHandler* DbCommandHandler::s_instance = 0;


DbCommandHandler *DbCommandHandler::get()
{
    if( ! s_instance ) {
        s_instance = new DbCommandHandler( QCoreApplication::instance() );
    }
    return s_instance;
}

void DbCommandHandler::run()
{
    while( hasQueuedCommands() ) {
        Command cmd = takeNextCommand();
        bool result = false;
        emit cmdExecutionStarted( cmd.m_code, cmd.m_data );
        switch( cmd.m_code ) {
        case Cmd_AddStoredObjects: {
            if( cmd.m_data ) {
                QList< Tanyatu::Data::StoredObject *> *objs= 0;
                objs = static_cast< QList< Tanyatu::Data::StoredObject*> *>(
                            cmd.m_data );
                result = DATA_UPDATER()->add( *objs );
            }
            break;
        }
        case Cmd_AddTracksToLib: {
            if( cmd.m_data ) {
                QList< Tanyatu::Data::StoredAudio *> *tracks = 0;
                tracks = static_cast< QList< Tanyatu::Data::StoredAudio *> *>(
                            cmd.m_data );
                foreach( Data::StoredAudio *track, *tracks ) {
                    if( AUDIO_LIB()->hasItem( track->trackId() )) {
                        result &= DATA_UPDATER()->add( track );
                    }
                }
            }
            break;
        }
        case Cmd_RemoveTracksFromLib: {
            if( cmd.m_data ) {
                QList< QString > *trackIds = static_cast< QList< QString > *>(
                            cmd.m_data );
                result = DATA_UPDATER()->removeAudioTracks( *trackIds );
            }
            break;
        }
        case Cmd_UpdateTrackInfo: {
            if( cmd.m_data ) {
                Tanyatu::Data::StoredAudio *track =
                        static_cast< Tanyatu::Data::StoredAudio *>(
                            cmd.m_data );
                result = DATA_UPDATER()->update( track );
            }
            break;
        }
        case Cmd_ClearAudioLib: {
            result = DATA_UPDATER()->clearAudioLibrary();
            break;
        }
        case Cmd_AddArtist: {
            if( cmd.m_data ) {
                Data::ArtistInfo *artist = static_cast< Data::ArtistInfo *>(
                            cmd.m_data );
                result = DATA_UPDATER()->add( artist );
            }
            break;
        }
        case Cmd_AddAlbum: {
            if( cmd.m_data ) {
                Data::AlbumInfo *album = static_cast< Data::AlbumInfo *>(
                            cmd.m_data );
                result = DATA_UPDATER()->add( album );
            }
            break;
        }
        case Cmd_RemoveArtist: {
            if( cmd.m_data ) {
                QString *artistName = static_cast< QString *>(
                            cmd.m_data );
                result = DATA_UPDATER()->removeArtist( *artistName );
            }
            break;
        }
        case Cmd_RemoveAlbum: {
            if( cmd.m_data ) {
                QString *albumName = static_cast< QString *>(
                            cmd.m_data );
                result = DATA_UPDATER()->removeAlbum( *albumName );
            }
            break;
        }
        case Cmd_UpdateArtist: {
            if( cmd.m_data ) {
                Data::ArtistInfo *artist = static_cast< Data::ArtistInfo *>(
                            cmd.m_data );
                result = DATA_UPDATER()->update( artist );
            }
            break;
        }
        case Cmd_UpdateAlbum: {
            if( cmd.m_data ) {
                Data::AlbumInfo *album = static_cast< Data::AlbumInfo *>(
                            cmd.m_data );
                result = DATA_UPDATER()->update( album );
            }
            break;

        }
        case Cmd_UpdateStoredObject: {
            if( cmd.m_data ) {
                Data::AlbumInfo *album = static_cast< Data::AlbumInfo *>(
                            cmd.m_data );
                result = DATA_UPDATER()->update( album );
            }
            break;
        }
        case Cmd_SavePlaylist: {
            if( cmd.m_data ) {
                QString *plName = static_cast< QString *>( cmd.m_data );
                QList< QString > trackIds = PL_MANAGER()->trackIdsIn( *plName );
                result = DATA_UPDATER()->savePlaylist( *plName, trackIds );
                delete plName;
            }
            break;
        }
        }
        emit cmdExecutionFinished( cmd.m_code, cmd.m_data, result );
    }
}


void DbCommandHandler::createAndAddCommand( DbCommandHandler::CommandCode code,
                                           void *data)
{
    switch( code ) {
    case Cmd_AddStoredObjects:
    case Cmd_AddTracksToLib:
    case Cmd_AddArtist:
    case Cmd_AddAlbum:
    case Cmd_SavePlaylist: {
        Command command( code, data );
        m_addOpQueue.append( command );
        break;
    }
    case Cmd_UpdateTrackInfo:
    case Cmd_UpdateArtist:
    case Cmd_UpdateAlbum:
    case Cmd_UpdateStoredObject: {
        Command command( code, data );
        m_updateOpQueue.append( command );
        break;
    }
    case Cmd_ClearAudioLib:
    case Cmd_RemoveTracksFromLib:
    case Cmd_RemoveArtist:
    case Cmd_RemoveAlbum: {
        Command command( code, data );
        m_removeOpQueue.append( command );
        break;
    }
    }
}


DbCommandHandler::Command DbCommandHandler::takeNextCommand()
{
    if( ! m_addOpQueue.isEmpty() ) {
        return m_addOpQueue.takeFirst();
    }
    else if( ! m_updateOpQueue.isEmpty() ) {
        return m_updateOpQueue.takeFirst();
    }
    else {
        return m_removeOpQueue.takeFirst();
    }
}


bool DbCommandHandler::hasQueuedCommands()
{
    return     ! m_addOpQueue.isEmpty()
            || ! m_updateOpQueue.isEmpty()
            || ! m_removeOpQueue.isEmpty();
}
