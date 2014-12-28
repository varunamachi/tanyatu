/*******************************************************************************
 * AlbumItemModel.h
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
#include <QList>
#include <core/data/AlbumInfo.h>

#include "../TanyatuCommonGlobal.h"

namespace Tanyatu { namespace Ui {

class TANYATU_GUI_EXPORT AlbumItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AlbumItemModel( int columnCount = 4, QObject *parent = 0 );

    virtual ~AlbumItemModel();

    void setAlbumList( QList< Data::AlbumInfo *> *albumList );

    const QList< Data::AlbumInfo *> * getAlbumList()
    {
        return m_albums;
    }

    virtual QVariant dataForAlbum( Data::AlbumInfo *album,
                                   int column ) const;

    virtual QString headerForColumn( int column ) const;

    void refreshModel();

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

    QList< Data::AlbumInfo * > *m_albums;

};
}
}
