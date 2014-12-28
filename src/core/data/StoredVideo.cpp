/*******************************************************************************
 * StoredVideo.cpp
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

#include "StoredVideo.h"

namespace Tanyatu { namespace Data {

const QString StoredVideo::STORAGE_ID = QString("STORED_VIDEO");

StoredVideo::StoredVideo( QString id, QUrl url, QString name )
    : VideoTrack( id, url, name ) {

}


MediaType StoredVideo::type() const
{
    return Media_StoredVideo;
}


QString StoredVideo::storageId() const
{
    return STORAGE_ID;
}


} } //end of ns
