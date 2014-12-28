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

#include <QDateTime>

#include "../TanyatuCoreGlobal.h"
#include "StoredMediaObject.h"


namespace Tanyatu { namespace Data {

/**
 * @brief The SavedPlaylist class represents the playlist containing list of
 * track ids from media libraries which are stored in data source. The list
 * itself is not part of this data structure, only metadata related to specific
 * list is stored in this structure
 */
class TANYATU_CORE_EXPORT SavedPlaylist : public StoredMediaObject
{
    /**
     * @brief m_name name of the playlist
     */
    QString m_name;

    /**
     * @brief m_description brief description of this playlist
     */
    QString m_description;

    /**
     * @brief m_creationTime Time of creation of this list
     */
    QDateTime m_creationTime;

    /**
     * @brief m_modifiedTime last modified time for this list
     */
    QDateTime m_modifiedTime;


public:
    /**
     * @brief SavedPlaylist constructor
     * @param name name of the playlist
     * @param creationTime time of creation
     */
    SavedPlaylist( const QString name, const QDateTime creationTime);

    /**
     * inherited docs
     */
    QString id() const;

    /**
     * @brief name give the name of this list
     * @return name of this playlist
     */
    QString name() const;

    /**
     * @brief description brief description of this list
     * @return description or comment for this playlist
     */
    QString description() const;

    /**
     * @brief creationTime gives the time of creation for this list
     * @return time of creation
     */
    QDateTime creationTime() const;

    /**
     * @brief modifiedTime gives the last modification time for this list
     * @return last modified time
     */
    QDateTime modifiedTime() const;

    /**
     * @brief setDescription modifies the description for this list
     * @param description the new description
     */
    void setDescription( const QString description );

    /**
     * @brief setModifiedTime updates the last modified time for this list
     * @param time new modification time
     */
    void setModifiedTime( const QDateTime time);

    /**
     * Inherited docs
     */
    QString storageId() const;

    /**
     * @brief STORAGE_ID storage id for this class
     */
    static const QString STORAGE_ID;
};

} } //end of ns
