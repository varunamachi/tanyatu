/*******************************************************************************
 * AlbumInfo.h
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

#include "../TanyatuCoreGlobal.h"
#include "StoredMediaObject.h"


namespace Tanyatu { namespace Data {

/**
 * @brief The AlbumInfo class is a in memory model of a audio album. This
 * information reflects the entries in the database and is kept consistent with
 * the database
 */
class TANYATU_CORE_EXPORT AlbumInfo : public StoredMediaObject
{
    /**
     * @brief m_artistName name of the artist who created this album
     */
    QString m_artistName;

    /**
     * @brief m_name name of the album
     */
    QString m_name;

    /**
     * @brief m_comment A discreptive comment about this album
     */
    QString m_comment;

    /**
     * @brief m_albumId a unique id constructed from the album name and artist
     * name
     */
    QString m_albumId;

public:
    /**
     * @brief AlbumInfo Ctor that constructs AlbumInfo from the artist name and
     * the album name The Id is calculated from the artist and album names.
     * @param artistName Name of the creator of this album
     * @param name of the album
     */
    AlbumInfo( const QString artistName, const QString name );

    /**
     * @brief id gives an identifier that is constructed from the names of
     * artist and the album that uniquely identifies this album
     * @return unique id for this album
     */
    QString id() const;

    /**
     * @brief name gives the name of the album that this object represents
     * @return name of the album
     */
    QString name() const;

    /**
     * @brief comment gives a comment associated with this object
     * @return comment about this album
     */
    QString comment() const;

    /**
     * @brief setComment sets the comment associated with this album
     * @param comment new comment to be associated
     */
    void setComment( const QString comment );

    /**
     * @brief artistName gives the name of the artist who created this album
     * @return name of the artist of this album
     */
    QString artistName() const;

    /**
     * @brief storageId gives an id representing the type of model object. This
     * is used to decide the persistance logic
     * @return id representing the type of object
     */
    QString storageId() const;

    /**
     * @brief getAlbumIdFrom constructs and gives a unique id from the album and
     * artist names of an album
     * @param artistName name of the artist of the album
     * @param albumName name of the album
     * @return a unique id for an album
     */
    static QString getAlbumIdFrom( const QString artistName,
                                   const QString albumName );

    /**
     * @brief STORAGE_ID unique id representing all the objects of the type
     * AlbumInfo
     */
    static const QString STORAGE_ID;
};

} } //end of ns
