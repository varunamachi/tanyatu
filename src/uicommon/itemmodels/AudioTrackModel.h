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

#include <QAbstractItemModel>
#include <core/data/StoredAudio.h>

#include "../TanyatuCommonGlobal.h"

namespace Tanyatu { namespace Ui {

class TANYATU_GUI_EXPORT AudioTrackModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AudioTrackModel( int numColumns, QObject *parent = 0 );

    Data::StoredAudio * trackAt( int index );

    ~AudioTrackModel();

    void setTrackList( QList< Data::StoredAudio *> *trackList );

    QList< Data::StoredAudio *> * getTrackList();

    void setNumColumns( int numColums );

    virtual QVariant dataForTrack( Data::StoredAudio *item,
                                   int column ) const = 0;

    virtual QString headerForColumn( int column ) const = 0;

    int rowCount( const QModelIndex &parent ) const;

    int columnCount( const QModelIndex &parent ) const;

    QVariant data( const QModelIndex &index, int role ) const;

    QVariant headerData( int section,
                         Qt::Orientation orientation,
                         int role ) const;

    QModelIndex index( int row, int column, const QModelIndex &parent ) const;

    QModelIndex parent( const QModelIndex &child ) const;

    bool hasChildren( const QModelIndex &parent ) const;

    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role );

public slots:

    void clear( bool deleteList = true );

private:

    int m_columnCount;

    QList< Data::StoredAudio *> *m_tracks;
};

} }

