/*******************************************************************************
 * SavedPlaylist.cpp
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

#include "SavedPlaylist.h"


namespace Tanyatu { namespace Data {

const QString SavedPlaylist::STORAGE_ID = QString("SAVED_PLAYLIST");

SavedPlaylist::SavedPlaylist( const QString name, const QDateTime creationTime)
    :m_name( name ), m_creationTime( creationTime )
{
}


QString SavedPlaylist::name() const
{
    return m_name;
}


QString SavedPlaylist::description() const
{
    return m_description;
}


QDateTime SavedPlaylist::creationTime() const
{
    return m_creationTime;
}


QDateTime SavedPlaylist::modifiedTime() const
{
    return m_modifiedTime;
}


void SavedPlaylist::setDescription( const QString description )
{
    m_description = description;
}


void SavedPlaylist::setModifiedTime( const QDateTime time )
{
    m_modifiedTime = time;
}


QString SavedPlaylist::storageId() const
{
    return STORAGE_ID;
}



QString SavedPlaylist::id() const
{
    return m_name;
}


} } //end of ns
