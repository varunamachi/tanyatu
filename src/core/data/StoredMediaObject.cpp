/*******************************************************************************
 * StoredItem.cpp
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

#include "StoredMediaObject.h"

namespace Tanyatu { namespace Data {

StoredMediaObject::StoredMediaObject()
    : m_rating(Tanyatu::Data::Rating_None),
      m_playCount(0)
{

}


StoredMediaObject::~StoredMediaObject()
{

}


MediaRating StoredMediaObject::rating() const
{
    return m_rating;
}


void StoredMediaObject::setRating(MediaRating rating)
{
    m_rating = rating;
}


unsigned int StoredMediaObject::playCount() const
{
    return m_playCount;
}


void StoredMediaObject::setPlayCount(unsigned int playCount)
{
    m_playCount = playCount;
}


const QSet<QString> &StoredMediaObject::tags() const
{
    return m_tags;
}


void StoredMediaObject::addTag( const QString tagName )
{
    m_tags.insert( tagName );
}


void StoredMediaObject::removeTag( const QString tagName )
{
    m_tags.remove( tagName );
}

} } //end of ns
