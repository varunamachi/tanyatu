/*******************************************************************************
 * PlaylistItemModel.cpp
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

#include "PlaylistItemModel.h"

namespace Tanyatu { namespace Ui {


PlaylistItemModel::PlaylistItemModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}


PlaylistItemModel::~PlaylistItemModel()
{

}


QList< Tanyatu::Data::SavedPlaylist * >& PlaylistItemModel::playlists( )
{
    return m_playlists;
}


int PlaylistItemModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_playlists.size();
}

int PlaylistItemModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 5;
}

QVariant PlaylistItemModel::data( const QModelIndex &index, int role ) const
{
    if( ! index.isValid() || index.row() >= m_playlists.size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Data::SavedPlaylist *playlist = m_playlists.at( index.row() );
        switch( index.column() ) {
        case 0: return playlist->name();
        case 1: return T::get()->playlistManager()->numTracksIn(
                        playlist->name() );
        case 2: return playlist->creationTime();
        case 3: return int ( playlist->rating() );
        }
    }
    else if( role == Qt::ToolTipRole ) {
        Data::SavedPlaylist *playlist = m_playlists.at( index.row() );
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
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


Qt::ItemFlags PlaylistItemModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 3 ) {
        return flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool PlaylistItemModel::setData( const QModelIndex &index,
                                    const QVariant &value,
                                    int role )
{
    Q_UNUSED( role )
    if( index.column() == 3 ) {
        Data::SavedPlaylist *playlist = m_playlists.at( index.row() );
        T::get()->playlistManager()->ratePlaylist( playlist->name(),
                    Utils::getRatingFromInt( value.toInt() ));
        return true;
    }
    return false;
}



void PlaylistItemModel::clear()
{
    m_playlists.clear();
}

} }
