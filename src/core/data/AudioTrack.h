/*******************************************************************************
 * AudioTrack.h
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
#pragma once

#include "MediaItem.h"
#include "ModelTypes.h"
#include "../TanyatuCoreGlobal.h"

namespace Tanyatu { namespace Data {

/**
 * @class AudioTrack is the instance of media item which represents an Audio
 * track on the local filesystem.
 */
class TANYATU_CORE_EXPORT  AudioTrack : public MediaItem
{
public:

    /**
      * @brief Constructor that initializes the MediaItem part
      * @param id unique id of this audio track
      * @param url url of source of this track
      */
    AudioTrack(const QString trackId, const QUrl url);

    /**
      * @brief Gives the title of this track
      * @returns title of the audio track preferably fetched from the tag
      */
    QString title() const;

    /**
      * @brief Gives the name of the artist or the composer of this track
      * @returns name of the artist/composer
      */
    QString artist() const;

    /**
      * @brief Gives the name of the album to which this track belongs
      * @param name of the track's album
      */
    QString album() const;

    /**
      * @brief Gives the date of composition of the track as given in the tag
      * @param date of track as given in the tag
      */
    QString date() const;

    /**
      * @brief Gives the genre to which this song is categorized
      * @returns genre of the track
      */
    QString genre() const;


    /**
      * @brief Gives any descreption given for this track in the tag
      * @returns descreption, if any, for this track
      */
    QString descreption() const;

    /**
      * @brief Gives the index of this track in it's album
      * @returns index of track in the album as given in the tag
      */
    int trackNumber() const;

    /**
      * @brief Gives the duration of the track
      * @returns the length/duration of the track
      */
    int duration() const;

    /**
      * @brief Gives the bitrate of the track in kbps
      * @returns the bitrate of the track in kbps
      */
    int bitRate() const;

    /**
      * @brief Gives the sample rate in kHz
      * @returns the sample rate in kHz
      */
    int sampleRate() const;


    /**
      * @brief Gives the number channel output for this track
      * @returns number of channels of audio for this track
      */
    int channels() const;


    /**
      * override
      */
    MediaType type() const;

    /**
      * @brief Sets the metadata information retrieved by reading the tag
      * associated with this track.
      * @param title Title of the audio track. Obtained from tag/file name
      * @param artist Artist/Composer of this track
      * @param album Album from which this track is
      * @param date Year/date of composition
      * @param genre Genre of this track
      * @param description Discreption of this track
      * @param trackNumber Index of this track in the album
      * @param duration Duration of this track
      * @param bitRate Bit rate of this track, generally in kbps
      * @param sampleRate Sample rate of this track, generally in kHz
      * @param numChannels Number audio channels
      **/
    void setMetadata( const QString title,
                     const QString artist,
                     const QString album,
                     const QString date,
                     const QString genre,
                     const QString descreption,
                     int trackNumber,
                     int duration,
                     int bitRate,
                     int sampleRate,
                     int numChannels );



protected:
    /**
      * @brief Title of the audio track. Obtained from tag/file name
      */
    QString m_title;

    /**
      * @brief Artist/Composer of this track
      */
    QString m_artist;

    /**
      * @brief Album from which this track is
      */
    QString m_album;

    /**
      * @brief Year/date of composition
      */
    QString m_date;

    /**
      * @brief Genre of this track
      */
    QString m_genre;

    /**
      * @brief Discreption of this track take from the tag
      */
    QString m_description;

    /**
      * @brief Index of this track in the album; taken from the tag
      */
    int m_trackNumber;

    /**
      * @brief Duration of this track
      */
    int m_duration;

    /**
      * @brief Bit rate of this track, generally in kbps
      */
    int m_bitRate;

    /**
      * @brief Sample rate of this track, generally in kHz
      */
    int m_sampleRate;

    /**
      * @brief Number audio channels
      */
    int m_channels;
};

} } //end of ns
