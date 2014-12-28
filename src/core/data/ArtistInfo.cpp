/*******************************************************************************
 * ArtistInfo.cpp
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

#include "ArtistInfo.h"

namespace Tanyatu { namespace Data {

const QString ArtistInfo::STORAGE_ID = QString("ARTIST_INFO");

ArtistInfo::ArtistInfo( const QString name )
    : m_name( name )
{
}


QString ArtistInfo::name() const
{
    return m_name;
}


QString ArtistInfo::comment() const
{
    return m_name;
}


const QList< QString >& ArtistInfo::albums() const
{
    return m_albums;
}


void ArtistInfo::setComment( const QString comment )
{
    m_comment = comment;
}


QString ArtistInfo::storageId() const
{
    return STORAGE_ID;
}


QString Tanyatu::Data::ArtistInfo::id() const
{
    return m_name;
}


} } //end of ns
