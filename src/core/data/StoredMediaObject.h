/*******************************************************************************
 * PersistedItem.h
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

#include <QSet>

#include "MediaItem.h"
#include "../TanyatuCoreGlobal.h"
#include "StoredObject.h"


namespace Tanyatu { namespace Data {

/** @class PersistedItem this is the superclass of the all the items whose
  * information will be kept track in a persistant data store.
  */
class TANYATU_CORE_EXPORT StoredMediaObject : public StoredObject
{
protected:

    /**
     * @brief StoredMediaObject constructor that does not do much
     */
    StoredMediaObject();

public:

    /**
      * @brief Virtual distructor
      */
    virtual ~StoredMediaObject();


    /** @brief Gives the rating of this object
      * @returns user rating of this item\
      */
    MediaRating rating() const;

    /** @brief Sets the user rating of this item.
      * @param rating the new rating for this item
      */
    void setRating( MediaRating rating );

    /** @brief Tells how many times this item has been played
      * @returns play count for this item
      */
    unsigned int playCount() const;

    /** @brief Updates the playcount with the given value
      * @param playCount new play count to set
      */
    void setPlayCount( unsigned int playCount );

    /**
      * @brief Gives tags for this object
      * @returns list of tags associated with the object
      */
    const QSet < QString > &tags() const;

    /**
      * @brief Adds a tag to this object
      * @param tag name to associate with the object
      */
    void addTag( const QString tagName );

    /**
      * @brief Removes a tag from this object
      * @param tagName tag to be removed4
      */
    void removeTag( const QString tagName );


protected:
    /**
     * @brief m_rating rating for this object
     */
    MediaRating m_rating;

    /**
     * @brief Stores the number of time this item is played.
     */
    unsigned int m_playCount;

    /**
      * @brief Stores the tags associated with the object
      */
    QSet< QString > m_tags;
};

} } //end of ns
