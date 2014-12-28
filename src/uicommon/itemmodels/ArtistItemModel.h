/*******************************************************************************
 * ArtistItemModel.h
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
#include <core/data/ArtistInfo.h>

#include "../TanyatuCommonGlobal.h"


namespace Tanyatu { namespace Ui {

class TANYATU_GUI_EXPORT ArtistItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ArtistItemModel( int columnCount = 3, QObject *parent = 0);

    virtual QVariant dataForArtist( Data::ArtistInfo *artist,
                                   int column ) const;

    virtual QString headerForColumn( int column ) const;


    ~ArtistItemModel();

    void setArtistList( QList< Data::ArtistInfo *> *artistList);

    const QList< Data::ArtistInfo *> * getArtistList()
    {
        return m_artists;
    }

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

    QList< Data::ArtistInfo * > *m_artists;

};
}
}

