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
    else if( role == Qt::TextAlignmentRole && index.column() == 3 ) {
        return Qt::AlignRight;
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
