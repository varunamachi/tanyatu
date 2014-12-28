/*******************************************************************************
 * Copyright (c) 2014 Varuna L Amachi. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
