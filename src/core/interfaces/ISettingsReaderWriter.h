/*******************************************************************************
 * ISettingsManager.h
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

#include <QString>

namespace Tanyatu {

struct StoredEntry
{
    StoredEntry( QString ikey, QString itype, QString ivalue )
        : key( ikey )
        , type( itype )
        , value( ivalue )
    {

    }

    QString key;

    QString type;

    QString value;
};


class ISettingsReaderWriter
{
public:
    virtual StoredEntry * get( QString key ) = 0;

    virtual bool set( StoredEntry *entry ) = 0;

    virtual bool remove( QString key ) = 0;

    ~ISettingsReaderWriter() { }

protected:
    ISettingsReaderWriter();

};

} // n
