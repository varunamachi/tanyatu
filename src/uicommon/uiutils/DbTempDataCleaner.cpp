/*******************************************************************************
 * DbTempDataCleaner.cpp
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

#if 0


//#include <core/workers/DbCommandHandler.h>
#include <core/data/StoredAudio.h>

#include "DbTempDataCleaner.h"

using namespace GreenChilli;

DbTempDataCleaner::DbTempDataCleaner(QObject *parent) :
    QObject(parent)
{
    connect( DBCMD_HANDLER(), SIGNAL( cmdExecutionFinished( int ,void*,bool)),
             this,SLOT( onDbCmdFinished( int, void*, bool )));
}


void DbTempDataCleaner::onDbCmdFinished(
        int type,
        void *data,
        bool result)
{
    Q_UNUSED( result )
    if( ! data ) {
        return;
    }
    DBCMD_CODE::CommandCode code = static_cast< DBCMD_CODE::CommandCode >(
                type );
    switch( code ) {
    case DBCMD_CODE::Cmd_AddStoredObjects: {
        delete static_cast< QList< Tanyatu::Data::StoredObject*> *>( data );
        break;
    }
    case DBCMD_CODE::Cmd_AddTracksToLib: {
        delete static_cast< QList< Tanyatu::Data::StoredAudio *> *>( data );
        break;
    }
    case DBCMD_CODE::Cmd_RemoveTracksFromLib: {
        delete static_cast< QList< QString > *>( data );
        break;
    }
    case DBCMD_CODE::Cmd_RemoveArtist:
    case DBCMD_CODE::Cmd_RemoveAlbum:
        delete static_cast< QString *>( data );
    default: break;
    }
}


#endif
