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
    if( artistList != nullptr ) {
        beginResetModel();
        if( m_artists != nullptr ) {
            m_artists->clear();
            delete m_artists;
            m_artists = nullptr;
        }
        m_artists = artistList;
        endResetModel();
    }
}


void ArtistItemModel::clear( bool deleteList )
{
    if( m_artists != nullptr ) {
        beginResetModel();
        if( deleteList ) {
            m_artists->clear();
            delete m_artists;
        }
        m_artists = nullptr;
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
            || m_artists == nullptr
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
    else if( role == Qt::TextAlignmentRole && index.column() == 2 ) {
        return Qt::AlignRight;
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
    return ! parent.isValid();
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
