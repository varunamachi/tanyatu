/*******************************************************************************
 * ArtistItemModel.cpp
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
#include <core/coreutils/Utils.h>

#include "ArtistItemModel.h"

namespace Tanyatu { namespace Ui {

ArtistItemModel::ArtistItemModel( int columnCount, QObject *parent ) :
    QAbstractItemModel( parent ),
    m_columnCount( columnCount ),
    m_artists( 0 )
{
}



ArtistItemModel::~ArtistItemModel()
{
    clear();
}


void ArtistItemModel::setArtistList( QList< Data::ArtistInfo *> *artistList )
{
    if( artistList ) {
        beginResetModel();
        m_artists = artistList;
        endResetModel();
    }
}


void ArtistItemModel::clear( bool deleteList )
{
    if( m_artists ) {
        beginResetModel();
        if( deleteList ) {
            m_artists->clear();
            delete m_artists;
        }
        m_artists = 0;
        endResetModel();
    }
}


int ArtistItemModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_artists ? m_artists->size() : 0;
}


int ArtistItemModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_columnCount;
}


QVariant ArtistItemModel::data( const QModelIndex &index, int role ) const
{
    if( ! index.isValid()
            || ! m_artists
            || index.row() >= m_artists->size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Data::ArtistInfo *artist = m_artists->at( index.row() );
        return dataForArtist( artist, index.column() );

    }
    else if( role == Qt::ToolTipRole ) {
        Data::ArtistInfo *artist = m_artists->at( index.row() );
        QString tooltip = QString() +
                tr( "Name  : <b>" ) + artist->name()  + "</b><br>";
        return tooltip;
    }
    //??
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    return QVariant();
}


QVariant ArtistItemModel::headerData( int section,
                                      Qt::Orientation orientation,
                                      int role ) const
{
    Q_UNUSED( orientation )
    if ( role == Qt::TextAlignmentRole ) {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        return headerForColumn( section );
    }
    return QVariant();
}


QModelIndex ArtistItemModel::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex ArtistItemModel::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool ArtistItemModel::hasChildren( const QModelIndex &parent ) const
{
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


Qt::ItemFlags ArtistItemModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 2 ) {
        return flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool ArtistItemModel::setData( const QModelIndex &index,
                                    const QVariant &value,
                                    int role )
{
    Q_UNUSED( role )
    if( index.column() == 2 ) {
        Data::ArtistInfo *artist = m_artists->at( index.row() );
        TRACKINF_MANAGER()->rateArtist(
                    artist->name(), Utils::getRatingFromInt( value.toInt() ));
        return true;
    }
    return false;
}



QVariant ArtistItemModel::dataForArtist( Data::ArtistInfo *artist,
                                        int column ) const
{
    switch( column ) {
    case 0: return artist->name();
    case 1: return TRACKINF_MANAGER()->numTracksFromArtist(
                    artist->name() );
    case 2: return int ( artist->rating() );
    }
    return QVariant();
}

QString ArtistItemModel::headerForColumn(int column) const
{
    switch( column ) {
    case 0: return tr( "Artist" );
    case 1: return tr( "Tracks" );
    case 2: return tr( "Rating" );
    }
    return "";
}

} }
