/*******************************************************************************
 * StoredObject.h
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

#include "../TanyatuCoreGlobal.h"

namespace Tanyatu { namespace Data {

/**
 * @brief The StoredObject class is an interface that must be implemented by
 * model object stored in the database
 */
class TANYATU_CORE_EXPORT StoredObject
{
protected:

    StoredObject() { /*NOTHING_TO_DO*/ }


public:
    /**
     * @brief ~StoredObject empty virtual distructor
     */
    virtual ~StoredObject() { /*NOTHING_HERE*/ }

     /**
     * @brief storageId Gives the storage id for the class that implements this
     * interface, which is used to determine how the information represented by
     * the objects of this class will be stored in the data store.
     * @return an identifier representing the type of stored object a reference
     * is pointing to.
     */
    virtual QString storageId() const {
        return "";
    }

    /**
     * @brief key Gives a unique string identifieng the object in the database.
     * This essentially represents the key in the database
     * @return unique database entry key for the object
     */
    virtual QString id() const = 0;
};

} } //end of ns
