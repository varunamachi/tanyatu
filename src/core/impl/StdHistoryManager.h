/*******************************************************************************
 * StdHistoryManager.h
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

#include "../interfaces/IHistoryManager.h"


namespace Tanyatu { namespace Impl {

class TANYATU_CORE_EXPORT StdHistoryManager : public IHistoryManager
{
    Q_OBJECT

public:
    explicit StdHistoryManager( QObject *parent = 0 );

    void onItemPlayed( Tanyatu::Data::MediaItem *item );

    void onItemRemovalFromStorage( QString itemId );

    void clear();

    void getHistoryItemIds( QList< QString > &ids ) const;

    QString uniqueName() const;

    QString module()  const;

    QString displayName() const;

    bool init();

    static QString COMPONENT_NAME;

    static QString COMPONENT_MODULE;

private:
    QList< QPair< QString, QDateTime > *> m_history;

    void clearStruct();
};


} } //end of namespaces

