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
