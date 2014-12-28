/*******************************************************************************
 * AudioTrackModel.cpp
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

#include "AudioTrackModel.h"

namespace Tanyatu { namespace Ui {


AudioTrackModel::AudioTrackModel( int numColumns, QObject *parent ) :
    QAbstractItemModel( parent ),
    m_columnCount( numColumns ),
    m_tracks( 0 )
{
}

Data::StoredAudio * AudioTrackModel::trackAt( int index )
{
    Data::StoredAudio *track = nullptr;
    if( m_tracks != nullptr && index < m_tracks->size() ) {
        track = m_tracks->at( index );
    }
    return track;
}


AudioTrackModel::~AudioTrackModel()
{
    clear();
}

QList< Data::StoredAudio *> * AudioTrackModel::getTrackList()
{
    return m_tracks;
}

void AudioTrackModel::setTrackList( QList< Data::StoredAudio *> *trackList )
{
    if( trackList ) {
        beginResetModel();
        m_tracks = trackList;
        endResetModel();
    }
}

void AudioTrackModel::clear( bool deleteList  )
{
    if( m_tracks ) {
        beginResetModel();
        if( deleteList ) {
            m_tracks->clear();
            delete m_tracks;
        }
        m_tracks = 0;
        endResetModel();
    }
}


void AudioTrackModel::setNumColumns( int numColums )
{
    if( numColums >= 0 ) {
        m_columnCount = numColums;
    }
}


int AudioTrackModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_tracks ? m_tracks->size() : 0;
}


int AudioTrackModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_columnCount;
}


QVariant AudioTrackModel::data( const QModelIndex &index, int role ) const
{
    if( ! index.isValid()
            || ! m_tracks
            || index.row() >= m_tracks->size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Data::StoredAudio *track = m_tracks->at( index.row() );
        return dataForTrack( track, index.column() );
    }
    //??
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    return QVariant();
}


QVariant AudioTrackModel::headerData( int section,
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


QModelIndex AudioTrackModel::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex AudioTrackModel::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool AudioTrackModel::hasChildren( const QModelIndex &parent ) const
{
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


Qt::ItemFlags AudioTrackModel::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 4 ) {
        return flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool AudioTrackModel::setData( const QModelIndex &index,
                               const QVariant &value,
                               int role )
{
    Q_UNUSED( role )
    if( m_tracks && index.column() == 4 ) {
        Data::StoredAudio *track = m_tracks->at( index.row() );
        AUDIO_LIB()->setRating( track->trackId(),
                      static_cast< Data::MediaRating >( value.toInt() ));
        return true;
    }
    return false;
}

} }
