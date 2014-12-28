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
