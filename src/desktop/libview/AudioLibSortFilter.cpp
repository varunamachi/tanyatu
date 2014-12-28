/*******************************************************************************
 * AudioBrowserSortFilter.cpp
 *
 * Copyright (c) 2013, Varuna L Amachi. All rights reserved.
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

#include <core/data/AlbumInfo.h>
#include <core/data/StoredAudio.h>
#include <core/T.h>

#include "AudioLibSortFilter.h"

namespace GreenChilli { namespace Components {

AudioBrowserSortFilter::AudioBrowserSortFilter(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_exp( "" )
{
}



AudioBrowserSortFilter::~AudioBrowserSortFilter() {

}

void AudioBrowserSortFilter::setExpression( QString expression ) {
    m_exp = expression;
    invalidateFilter();
}


bool AudioBrowserSortFilter::lessThan(
    const QModelIndex &left, const QModelIndex &right) const {
    QVariant lData = left.data();
    QVariant rData = right.data();
    bool ok;
    int leftInt = lData.toInt( &ok );
    int rightInt = rData.toInt( &ok );
    if( ok ) {
        return leftInt < rightInt;
    }
    else {
        return QString::localeAwareCompare(lData.toString(),
                                           rData.toString()) < 0;
    }
}


bool ArtistSortFilter::filterAcceptsRow( int source_row,
                                        const QModelIndex &source_parent ) const
{
    Q_UNUSED( source_parent )
    QString artist = sourceModel()->data(
                sourceModel()->index( source_row, 0 )).toString();
    return artist.contains( m_exp, Qt::CaseInsensitive );

}


bool AlbumSortFilter::filterAcceptsRow( int source_row,
                                        const QModelIndex &source_parent) const
{
    Q_UNUSED( source_parent )
    QString albumId = sourceModel()->data(
                sourceModel()->index( source_row, 0 )).toString();
    Tanyatu::Data::AlbumInfo *info = TRACKINF_MANAGER()->album( albumId );
    bool result = false;
    if( info ) {
        result = info->artistName().contains( m_exp, Qt::CaseInsensitive );
    }
    return result;
}


bool BrowserTrackSortFilter::filterAcceptsRow(
        int source_row,
        const QModelIndex &source_parent) const
{
    Q_UNUSED( source_parent )
    bool result = false;
    QString trackId = sourceModel()->data(
                sourceModel()->index( source_row, 0 )).toString();
    Tanyatu::Data::StoredAudio *track = AUDIO_LIB()->item( trackId );
    if( track )
    {
        result = track->artist().contains( m_exp, Qt::CaseInsensitive );
    }
    return result;
}

} }
