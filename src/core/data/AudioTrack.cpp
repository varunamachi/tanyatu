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
