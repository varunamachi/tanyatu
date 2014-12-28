/*******************************************************************************
 * MediaItemModel.cpp
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
#include <QSize>
#include <QFont>

#include <core/T.h>

#include "MediaItemModel.h"

namespace Tanyatu { namespace Ui {


MediaItemModel::MediaItemModel( int numColumns, QObject *parent ) :
    QAbstractItemModel( parent ),
    m_columnCount( numColumns ),
    m_items( 0 )
{
}

Data::MediaItem * MediaItemModel::itemAt( int index )
{
    Data::MediaItem *item= nullptr;
    if( m_items != nullptr && index < m_items->size() ) {
        item = m_items->at( index );
    }
    return item;
}


MediaItemModel::~MediaItemModel()
{
    clear();
}

const QList< Data::MediaItem *> * MediaItemModel::getItemList()
{
    return m_items;
}

void MediaItemModel::setItemList( const QList< Data::MediaItem *> *itemList )
{
    if( m_items ) {
        beginResetModel();
        m_items = itemList;
        endResetModel();
    }
    else {
        m_items = itemList;
    }
}

void MediaItemModel::clear()
{
    if( m_items ) {
        beginResetModel();
        m_items = 0;
        endResetModel();
    }
}


void MediaItemModel::setNumColumns( int numColumns )
{
    if( numColumns >= 0 ) {
        m_columnCount = numColumns;
    }
}


int MediaItemModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_items ? m_items->size() : 0;
}


int MediaItemModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_columnCount;
}


QVariant MediaItemModel::data( const QModelIndex &index, int role ) const
{
    if( ! index.isValid()
            || ! m_items
            || index.row() >= m_items->size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Data::MediaItem *item = m_items->at( index.row() );
        return dataForTrack( item, index.column() );
    }
    //??
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    return QVariant();
}


QVariant MediaItemModel::headerData( int section,
                                     Qt::Orientation orientation,
                                     int role ) const
{
    Q_UNUSED( orientation );
    if ( role == Qt::TextAlignmentRole ) {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        return headerForColumn( section );
    }
    return QVariant();
}


QModelIndex MediaItemModel::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex MediaItemModel::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool MediaItemModel::hasChildren( const QModelIndex &parent ) const
{
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


} }
