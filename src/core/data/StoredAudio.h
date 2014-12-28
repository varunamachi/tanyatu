/*******************************************************************************
 * StoredAudio.h
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

#include "AudioTrack.h"
#include "StoredMediaObject.h"
#include "../TanyatuCoreGlobal.h"

namespace Tanyatu { namespace Data {

/**
  * \class StoredAudio represents a audio track which is tracked in database
  */
class TANYATU_CORE_EXPORT StoredAudio : public AudioTrack,
                                        public StoredMediaObject
{

public:

    /**
      * \fn StoredAudio( QString id, QUrl url ) constructor initializes the
      * media item
      */
    StoredAudio( const QString trackId, const QUrl url);


    /**
      * inherited
      */
    MediaType type() const;


    /**
      * inherited
      */
    QString storageId() const;

    /**
     * inherited
     */
    QString id() const;


    static const QString STORAGE_ID;


};

} } //end of ns
