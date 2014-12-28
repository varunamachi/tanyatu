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
