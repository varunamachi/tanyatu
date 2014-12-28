/*******************************************************************************
 * StoredAudio.cpp
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


#include "StoredAudio.h"

namespace Tanyatu { namespace Data {


const QString StoredAudio::STORAGE_ID = QString("STORED_AUDIO");

StoredAudio::StoredAudio( const QString id, const QUrl url)
    :AudioTrack( id, url )
{

}


MediaType StoredAudio::type() const
{
    return Media_StoredAudio;
}



QString StoredAudio::storageId() const
{
    return STORAGE_ID;
}


QString Tanyatu::Data::StoredAudio::id() const
{
    return m_id;
}


} } //end of ns
