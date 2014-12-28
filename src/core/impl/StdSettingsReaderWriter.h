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
