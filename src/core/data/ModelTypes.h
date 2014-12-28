/*******************************************************************************
 * ModelTypes.h
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

#include "../TanyatuCoreGlobal.h"


namespace Tanyatu { namespace Data
{

    enum MediaType
    {
        Media_LocalAudio,
        Media_LocalVideo,
        Media_StoredAudio,
        Media_StoredVideo,
        Media_RemoteAudio,
        Media_RemoteVideo
    };


    enum MediaRating
    {
        Rating_None,
        Rating_Bad,
        Rating_Average,
        Rating_Good,
        Rating_VeryGood,
        Rating_Excellent
    };

} } //end of namespaces
