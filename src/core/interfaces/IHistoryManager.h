/*******************************************************************************
 * IHistoryManager.h
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

#include <QObject>

#include "IComponent.h"
#include "../data/StoredAudio.h"



namespace Tanyatu {

class TANYATU_CORE_EXPORT IHistoryManager : public QObject,
                                            public IComponent
{
    Q_OBJECT

protected:
    IHistoryManager( QObject *parent = 0 )
        : QObject( parent )
        , IComponent() { /* NOTHING_HERE */ }

signals:
    void aboutToClearHistory();

    void clearedHistory();

    void itemAddedToHistory( Tanyatu::Data::StoredAudio *item );

    void aboutToChangeHistoryManager();

    void historyManagerChanged();

public slots:
    virtual void onItemPlayed( Tanyatu::Data::MediaItem *item ) = 0;

    virtual void onItemRemovalFromStorage( QString itemId ) = 0;

    virtual void clear() = 0;

public:
    virtual void getHistoryItemIds( QList< QString > &historyIds ) const = 0;

    virtual ~IHistoryManager() { /*NOTHING_TO_DO*/ }
};

} //end  of ns

