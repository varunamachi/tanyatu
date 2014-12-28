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

#include "AlbumInfo.h"

namespace Tanyatu { namespace Data {

const QString AlbumInfo::STORAGE_ID = QString("ALBUM_INFO");

AlbumInfo::AlbumInfo( const QString artistName, const QString name )
    :m_artistName( artistName ),
      m_name( name ),
      m_albumId( getAlbumIdFrom( artistName, name ))
{

}


QString AlbumInfo::name() const
{
    return m_name;
}


QString AlbumInfo::id() const
{
    return m_albumId;
}


QString AlbumInfo::comment() const
{
    return m_comment;
}


void AlbumInfo::setComment( const QString comment )
{
    m_comment = comment;
}


QString AlbumInfo::artistName() const
{
    return m_artistName;
}


QString AlbumInfo::storageId() const
{
    return STORAGE_ID;
}


QString AlbumInfo::getAlbumIdFrom( const QString artistName,
                                   const QString albumName )
{
    return QString( artistName ).append( "#" ).append( albumName );
}

} } //end of ns
