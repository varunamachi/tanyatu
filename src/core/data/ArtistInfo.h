/*******************************************************************************
 * ArtistInfo.h
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

#include "StoredMediaObject.h"

namespace Tanyatu { namespace Data {

/**
 * @brief The ArtistInfo class represents metadata associated with an artist
 * who has atleast one track in the audio library
 */
class TANYATU_CORE_EXPORT ArtistInfo : public StoredMediaObject
{
    
    /**
     * @brief m_name name of the artist
     */
    QString m_name;

    /**
     * @brief m_comment a brief description/comment about this artist
     */
    QString m_comment;

    /**
     * @brief m_albums list of albums associated with this artist
     */
    QList< QString > m_albums;

public:
    /**
     * @brief ArtistInfo constructor
     * @param name name of the artist
     */
    ArtistInfo( const QString name );

    /**
      * virtual destructor
      */
    ~ArtistInfo() { }

    /**
     * Inherited docs
     */
    QString id() const;

    /**
     * @brief name gives this name of the artist
     * @return name of the artist
     */
    QString name() const;

    /**
     * @brief comment gives comment about this artist
     * @return description/comment for the artist
     */
    QString comment() const;

    /**
     * @brief albums gives a list of albums for which this artist has created
     * @return albums from this artist
     */
    const QList< QString >& albums() const;

    /**
     * @brief setComment modifies the comment about this artist
     * @param comment a new comment/desc. about this artist
     */
    void setComment( const QString comment );

    /**
     * Inherited docs
     */
    QString storageId() const;

    /**
     * @brief STORAGE_ID storage id for this class
     */
    static const QString STORAGE_ID;

};

} } //end of ns
