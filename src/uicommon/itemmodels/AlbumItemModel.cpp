/*******************************************************************************
 * AlbumItemModel.cpp
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
#include <QAbstractItemModel>


#include <core/T.h>
#include <core/coreutils/Utils.h>

#include "AlbumItemModel.h"

namespace Tanyatu { namespace Ui {

AlbumItemModel::AlbumItemModel( int columnCount, QObject *parent ) :
    QAbstractItemModel( parent ),
    m_columnCount( columnCount ),
    m_albums( 0 )
{
}


AlbumItemModel::~AlbumItemModel()
{
    clear();
}


void AlbumItemModel::setAlbumList( QList< Data::AlbumInfo *> *albumList )
{
    if( albumList ) {
        beginResetModel();
        m_albums = albumList;
        endResetModel();
    }
}



void AlbumItemModel::clear( bool deleteList )
{
    if( m_albums ) {
        beginResetModel();
        if( deleteList ) {
            m_albums->clear();
            delete m_albums;
        }
        m_albums = 0;
        endResetModel();
    }
}


int AlbumItemModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_albums ? m_albums->size() : 0;
}


int AlbumItemModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_columnCount;
}


QVariant AlbumItemModel::data( const QModelIndex &index, int role ) const
{
    if( ! index.isValid()
            || ! m_albums
            || index.row() >= m_albums->size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Data::AlbumInfo *album = m_albums->at( index.row() );
        return dataForAlbum( album, index.column() );
    }
    else if( role == Qt::ToolTipRole ) {
        Data::AlbumInfo *album = m_albums->at( index.row() );
        QString tooltip = QString() +
                tr( "Name  : <b>" ) + album->name()  + "</b><br>" +
                tr( "Artist : <b>" ) + album->artistName() + "</b><br>";
        return tooltip;
    }
    //??
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    return QVariant();
}


QVariant AlbumItemModel::headerData( int section,
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


QModelIndex AlbumItemModel::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex AlbumItemModel::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool AlbumItemModel::hasChildren( const QModelIndex &parent ) const
{
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


Qt::ItemFlags AlbumItemModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 3 ) {
        return flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool AlbumItemModel::setData( const QModelIndex &index,
                                    const QVariant &value,
                                    int role )
{
    Q_UNUSED( role )
    if( m_albums && index.column() == 3 ) {
        Data::AlbumInfo *album = m_albums->at( index.row() );
        TRACKINF_MANAGER()->rateAlbum( album->artistName(),
                       album->name(), Utils::getRatingFromInt( value.toInt() ));
        return true;
    }
    return false;
}

QVariant AlbumItemModel::dataForAlbum( Data::AlbumInfo *album,
                                       int column ) const
{
    switch( column ) {
    case 0: return album->id();
    case 1: return album->name();
    case 2: return TRACKINF_MANAGER()->numTracksFromAlbum(
                    album->artistName(), album->name() );
    case 3: return int ( album->rating() );
    }
    return QVariant();
}

QString AlbumItemModel::headerForColumn(int column) const
{
    switch( column ) {
    case 0: return tr( "Id" );
    case 1: return tr( "Name" );
    case 2: return tr( "Tracks" );
    case 3: return tr( "Rating" );
    }
    return "";
}


} }
