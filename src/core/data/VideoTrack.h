/*******************************************************************************
 * VideoTrack.h
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


