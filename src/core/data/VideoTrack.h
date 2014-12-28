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
#pragma once


#include <QHash>

#include "../TanyatuCoreGlobal.h"
#include "MediaItem.h"



namespace Tanyatu { namespace Data {

/**
  * @class VideoTrack is representation of a video item in the filesystem.
  * The many properties here represents the values entered by users
  */
class TANYATU_CORE_EXPORT VideoTrack : public MediaItem
{
public:
    /**
      * @brief Constructor intializes the item
      * @param id unique identifier
      * @param url URL of the source
      * @param name name of the video
      */
    VideoTrack( const QString id, const QUrl url, const QString name );


    /**
      * @brief Gives the name of the video
      * @returns name of the video
      */
    QString name();


    /**
      * @brief Sets a new name for the video
      * @param name new name for the video
      */
    void setName( const QString name );


    /**
      * @brief Gives the length of the video in seconds
      * @returns duration of the video
      */
    int duration() const;


    /**
      * @brief Sets the duration for this video
      * @param duration new length of the video
      */
    void setDuration( int duration );


    /**
      * @brief Gives the category of the video
      * @returns category of video
      */
    QString category() const;

    /**
      * @brief Sets the category of the video
      * @param categroy of the video
      */
    void setCategory( const QString category );

    /**
      * @brief Gives year of composition
      * @returns year of creation
      */
    quint8 year() const;

    /**
      * @brief Sets the year of composition/creation
      * @param year of creation
      */
    void setYear(quint8 year);

    /**
      * @brief Gives the descreption/comment
      * @returns descretion assciated with the video
      */
    QString description() const;

    /**
      * @brief Sets a new discreption
      * @param description
      */
    void setDescription( const QString descreption );

    /**
      * @brief Gives the name of the creator of the video
      * @returns name of the video's creator
      */
    QString creator() const;

    /**
      * @brief Sets the creator's name
      * @param name of the creator
      */
    void setCreator( const QString creator );

    /**
      * @brief Gives a hash map which has the name of artists mapped to their
      * roles in this video
      * @returns mapping between artists involved in this video and their roles
      */
    const QHash< QString, QString > &artistsAndRoles() const;

    /**
      * @brief Adds an artist information with the role information to the map
      * @param name of the artist
      * @param role of the artist in this video
      */
    void addArtists( const QString artistName, const QString role);


protected:
    QString m_name;


    /**
      * @brief Category of video, like movies, music videos, documentary etc.
      * Given by user or retrieved from some other data source
      */
    QString m_category;

    /**
      * @brief Year of creation
      */
    quint8 m_year;

    /**
      * @brief Length of the video
      */
    int m_duration;

    /**
      * @brief Description/comment
      */
    QString m_descreption;

    /**
      * @brief Name of the creator of the content
      */
    QString m_creator;

    /**
      * @brief Artists involved and their roles.
      */
    QHash<QString, QString> m_artistsAndRoles;
};


} } //end of ns


