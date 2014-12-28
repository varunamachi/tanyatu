/*******************************************************************************
 * AlbumInfo.cpp
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

#include "AlbumInfo.h"

namespace Tanyatu { namespace Data {

const QString AlbumInfo::STORAGE_ID = QString("ALBUM_INFO");

AlbumInfo::AlbumInfo( const QString artistName, const QString name )
    :m_artistName( artistName ),
      m_name( name ),
      m_albumId( getAlbumIdFrom( artistName, name ))
{

}


QString AlbumInfo::name() const
{
    return m_name;
}


QString AlbumInfo::id() const
{
    return m_albumId;
}


QString AlbumInfo::comment() const
{
    return m_comment;
}


void AlbumInfo::setComment( const QString comment )
{
    m_comment = comment;
}


QString AlbumInfo::artistName() const
{
    return m_artistName;
}


QString AlbumInfo::storageId() const
{
    return STORAGE_ID;
}


QString AlbumInfo::getAlbumIdFrom( const QString artistName,
                                   const QString albumName )
{
    return QString( artistName ).append( "#" ).append( albumName );
}

} } //end of ns
