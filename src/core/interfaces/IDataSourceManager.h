/*******************************************************************************
 * IDataManager.h
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

#include <QString>
#include <QHash>

#include "../TanyatuCoreGlobal.h"

namespace Tanyatu {

class TANYATU_CORE_EXPORT IDataSourceManager
{
public:
    virtual bool init( QHash< QString, QString > &params ) = 0;

    virtual bool isDataSourceValid() = 0;

    virtual QString dataSourceId() = 0;

    virtual QString description() = 0;

    virtual QString lastError() = 0;

    virtual ~IDataSourceManager() { /*NOTHING TO DO*/ }

protected:
    IDataSourceManager() { /*NOTHING TO DO*/ }
};

} //end of ns
