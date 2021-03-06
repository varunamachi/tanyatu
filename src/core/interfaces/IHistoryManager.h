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

