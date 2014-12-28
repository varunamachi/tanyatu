/*******************************************************************************
 * AudioTrack.cpp
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


#include "AudioTrack.h"

namespace Tanyatu { namespace Data {

//Consider using light weight object


AudioTrack::AudioTrack( const QString id, const QUrl url):
    MediaItem( id, url )
{
}


void AudioTrack::setMetadata( const QString title,
                             const QString artist,
                             const QString album,
                             const QString date,
                             const QString genre,
                             const QString descreption,
                             int trackNumber,
                             int duration,
                             int bitRate,
                             int sampleRate,
                             int numChannels )
{
    //TODO if title is NULL/empty get the title from file path i.e url
    m_title = title;
    m_artist = artist;
    m_album = album;
    m_date = date;
    m_genre = genre;
    m_description = descreption;
    m_trackNumber = trackNumber;
    m_duration = duration;
    m_bitRate = bitRate;
    m_sampleRate = sampleRate;
    m_channels = numChannels;


}


MediaType AudioTrack::type() const
{
    return Tanyatu::Data::Media_LocalAudio;
}



QString AudioTrack::title() const
{
    return m_title;
}


QString AudioTrack::artist() const
{
    return m_artist;
}


QString AudioTrack::album() const
{
    return m_album;
}


QString AudioTrack::date() const
{
    return m_date;
}


QString AudioTrack::genre() const
{
    return m_genre;
}


QString AudioTrack::descreption() const
{
    return m_description;
}


int AudioTrack::trackNumber() const
{
    return  m_trackNumber;
}


int AudioTrack::duration() const
{
    return m_duration;
}


int AudioTrack::bitRate() const
{
    return  m_bitRate;
}


int AudioTrack::sampleRate() const
{
    return m_sampleRate;
}


int AudioTrack::channels() const
{
    return m_channels;
}


} } //end of ns
