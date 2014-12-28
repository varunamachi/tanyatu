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
