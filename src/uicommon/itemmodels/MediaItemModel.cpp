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
