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
