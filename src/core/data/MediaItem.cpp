/*******************************************************************************
 * MediaItem.cpp
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

#include "MediaItem.h"

namespace Tanyatu { namespace Data {


MediaItem::MediaItem( const QString id, const QUrl url) :
    m_id(id), m_url(url)
{

}


MediaItem::~MediaItem()
{

}


bool MediaItem::operator ==(MediaItem& other) const
{
    return  this->trackId() == other.trackId();
}


QString MediaItem::trackId() const
{
    return m_id;
}


QUrl MediaItem::url() const
{
    return m_url;
}


} } //end of ns
