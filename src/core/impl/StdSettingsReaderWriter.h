/*******************************************************************************
 * StdSettingsReaderWriter.h
 *
 * Copyright (c) 2014, Varuna L Amachi. All rights reserved.
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

#include "../interfaces/ISettingsReaderWriter.h"
#include "T.h"


namespace Tanyatu { namespace Impl {

class StdSettingsManager : public
{
public:
    StdSettingsManager() { }
    ~StdSettingsManager() { }

    StoredEntry * get( QString key )
    {
        QString type;
        QString value;
        StoredEntry *entry = nullptr;
        if( DATA_RETRIEVER()->getProperty( key, type, value )) {
            entry = new StoredEntry( key, type, value );
        }
        return entry;
    }

    bool set( StoredEntry *entry )
    {
        bool result = false;
        if( entry != nullptr ) {
            result = DATA_UPDATER()->setProperty( entry->key,
                                                  entry->type,
                                                  entry->value);
        }
        return result;
    }

    bool remove( QString key )
    {
        return DATA_UPDATER()->removeProperty( key );
    }


};

} } //end of ns
