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

