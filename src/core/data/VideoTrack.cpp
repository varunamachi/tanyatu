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
