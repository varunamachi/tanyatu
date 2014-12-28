/*******************************************************************************
 * VideoTrack.cpp
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

#include "VideoTrack.h"

namespace Tanyatu { namespace Data {

VideoTrack::VideoTrack( const QString id, const QUrl url, const QString name )
    : MediaItem( id, url )
    , m_name( name )
{
}


QString VideoTrack::name()
{
    return m_name;
}


void VideoTrack::setName( const QString name )
{
    m_name = name;
}


int VideoTrack::duration() const
{
    return m_duration;
}


void VideoTrack::setDuration( int duration )
{
    m_duration = duration;
}


QString VideoTrack::category() const
{
    return m_category;
}

void VideoTrack::setCategory( const QString category )
{
    m_category = category;
}

quint8 VideoTrack::year() const
{
    return m_year;
}

void VideoTrack::setYear( quint8 year )
{
    m_year = year;
}

QString VideoTrack::description() const
{
    return m_descreption;
}

void VideoTrack::setDescription( const QString descreption )
{
    m_descreption = descreption;
}

QString VideoTrack::creator() const
{
    return m_creator;
}

void VideoTrack::setCreator( const QString creator )
{
    m_creator = creator;
}

const QHash< QString, QString > &VideoTrack::artistsAndRoles() const
{
    return m_artistsAndRoles;
}

void VideoTrack::addArtists( const QString artistName, const QString role )
{
    m_artistsAndRoles.insert( artistName, role );
}


} } //end of ns
