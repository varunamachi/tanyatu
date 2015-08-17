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

#include "PlaylistItemModel.h"

namespace Tanyatu { namespace Ui {


PlaylistItemModel::PlaylistItemModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_playlists( nullptr )
{

}


PlaylistItemModel::~PlaylistItemModel()
{

}

void PlaylistItemModel::setPlaylists(
        QList< Data::SavedPlaylist * > *playlists )
{
    if( playlists != nullptr ) {
        beginResetModel();
        if( m_playlists != nullptr ) {
            m_playlists->clear();
            delete m_playlists;
            m_playlists = nullptr;
        }
        m_playlists = playlists;
        endResetModel();
    }
}


const QList< Data::SavedPlaylist * > * PlaylistItemModel::playlists()
{
    return m_playlists;
}


int PlaylistItemModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_playlists != nullptr ? m_playlists->size() : 0;
}

int PlaylistItemModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 5;
}

QVariant PlaylistItemModel::data( const QModelIndex &index, int role ) const
{
    if( m_playlists == nullptr ) {
        return QVariant();
    }
    if( ! index.isValid() || index.row() >= m_playlists->size() ) {
        return QVariant();
    }
    if( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if( role == Qt::DisplayRole ) {
        Data::SavedPlaylist *playlist = m_playlists->at( index.row() );
        switch( index.column() ) {
        case 0: return playlist->name();
        case 1: return T::get()->playlistManager()->numTracksIn(
                        playlist->name() );
        case 2: return playlist->creationTime();
        case 3: return int ( playlist->rating() );
        }
    }
    else if( role == Qt::ToolTipRole ) {
        Data::SavedPlaylist *playlist = m_playlists->at( index.row() );
        QString tooltip = QString() +
                  tr( "Name  : <b>" ) + playlist->name()  + "</b><br>"
                + tr( "Created : <b>" )+
                playlist->creationTime().toString() + "</b><br>"
                + tr( "Modified : ")+
                playlist->modifiedTime().toString() + "</b<br>";
        return tooltip;
    }
    //??
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    else if( role == Qt::FontRole ) {
        return QFont( "Droid Sans", 9 );
    }
    return QVariant();
}


QVariant PlaylistItemModel::headerData( int section,
                                           Qt::Orientation orientation,
                                           int role ) const
{
    Q_UNUSED( orientation )
    if ( role == Qt::TextAlignmentRole ) {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        switch( section ) {
        case 0: return tr( "Name" );
        case 1: return tr( "Tracks" );
        case 2: return tr( "Created" );
        case 4: return tr( "Rating" );
        }
    }
    return QVariant();
}


QModelIndex PlaylistItemModel::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex PlaylistItemModel::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool PlaylistItemModel::hasChildren( const QModelIndex &parent ) const
{
    return ! parent.isValid();
}


Qt::ItemFlags PlaylistItemModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 3 ) {
        flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool PlaylistItemModel::setData( const QModelIndex &index,
                                    const QVariant &value,
                                    int role )
{
    if( m_playlists == nullptr ) {
        return false;
    }
    bool result = false;
    Q_UNUSED( role )
    if( index.column() == 3 ) {
        Data::SavedPlaylist *playlist = m_playlists->at( index.row() );
        T::get()->playlistManager()->ratePlaylist( playlist->name(),
                    Utils::getRatingFromInt( value.toInt() ));
        result = true;
    }
    return result;
}



void PlaylistItemModel::clear( bool deleteList )
{
    if( m_playlists != nullptr ) {
        beginResetModel();
        if( deleteList ) {
            m_playlists->clear();
            delete m_playlists;
        }
        m_playlists = 0;
        endResetModel();
    }
}

} }
