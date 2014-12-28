/*******************************************************************************
 * AudioTrackModel.h
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

