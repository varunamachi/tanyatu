/*******************************************************************************
 * FileSystemUtil.cpp
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
#include <QDir>

#include "FileSystemUtil.h"

#define TANYATU_DIR_NAME ".tanyatu"
#define TANYATU_PLAYLIST_DIR "playlists"

using namespace Tanyatu;


bool FileSystemUtil::initTanyatuFS()
{
    bool result = true;
    QDir tanyatuDir = QDir::home().absoluteFilePath( TANYATU_DIR_NAME );
    if( ! tanyatuDir.exists() ) {
        if( QDir::home().mkdir( TANYATU_DIR_NAME )) {
            result &= tanyatuDir.mkdir( TANYATU_PLAYLIST_DIR );
        }
        else {
            result = false;
        }
    }
    return result;
}


QDir FileSystemUtil::getTanyatuDir()
{
    QDir dir( QDir::home().absoluteFilePath( TANYATU_DIR_NAME ));
    return dir;
}

QString FileSystemUtil::getPathFromTanyatu( QString relPath )
{
    return QDir( QDir::home().absoluteFilePath( TANYATU_DIR_NAME ))
                                                   .absoluteFilePath( relPath );
}


QString FileSystemUtil::getPlaylistPath( QString playlistName )
{
    return getPathFromTanyatu( QString( TANYATU_PLAYLIST_DIR )
                                  .append( "/")
                                  .append( playlistName ));
}


