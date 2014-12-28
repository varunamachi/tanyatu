/*******************************************************************************
 * SavedPlaylist.h
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
