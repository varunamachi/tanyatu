/*******************************************************************************
 * MediaItem.h
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

#include <QUrl>
#include <QString>

#include "ModelTypes.h"
#include "../TanyatuCoreGlobal.h"

namespace Tanyatu { namespace Data {

/**
 * @class A class representing a generic media item which could be audio/video
 * or local/remote. This is abstract class and needs to inherited by the classes
 * whcih represent the above mentioned catagories.
 */
class TANYATU_CORE_EXPORT MediaItem
{
public:
    /**
     * @brief Constructor. Creates a media item with given id, type and the
     * source url
     * @param id a unique id that represents this item, mostly its path or url
     * in string form.
     * @param url URL of the media source for which this object is built
     */
    MediaItem( const QString trackId, const QUrl url );

    /**
     * @brief Gives the unique id of the this item
     * @returns unique id of this item
     */
    QString trackId() const;

    /**
     * @brief Gives the type of this media item. This method should be
     * implemented the children of this class i.e. specific items such as audio
     * item or video item.
     * @returns MediaType of this item
     */
    virtual MediaType type() const = 0 ;

    /**
     * @brief Gives the URL to the source of this item. Using this phonon can build
     * the media object.
     * @returns URL to the source of the item
     */
    virtual QUrl url() const;



    /**
      * @brief Virtual distructor
      */
    virtual ~MediaItem();


    /**
      * @brief virtual equaluty operator. Equality is checked based on the id of
      * the item
      * @todo making equality operator virtual does not make any sense? because
      * we do not compare pointers
      * @param other media item instance
      */
    virtual bool operator ==( Tanyatu::Data::MediaItem& other ) const;

protected:
    /**
     * @brief m_id unique Id for the item
     */
    QString m_id;

    /**
     * @brief m_url URL local or remote for the item
     */
    QUrl m_url;
};

} } // end of ns

