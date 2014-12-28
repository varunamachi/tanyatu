/*******************************************************************************
 * TrackFilter.cpp
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

#include <core/T.h>
#include <core/data/StoredAudio.h>
#include <uicommon/itemmodels/AudioTrackModel.h>

#include "TrackFilter.h"


namespace GreenChilli { namespace Views {


void TrackFilter::setExpression( QString expression ) {
    m_exp = expression;
    invalidateFilter();
}


bool TrackFilter::filterAcceptsRow( int source_row,
                                    const QModelIndex &source_parent) const {
    Q_UNUSED( source_parent )
    bool result = false;
    auto trackModel =
            static_cast< Tanyatu::Ui::AudioTrackModel *>( this->sourceModel() );
    Tanyatu::Data::AudioTrack *track = trackModel->trackAt( source_row );
    if( track != nullptr ) {
        result = track->title().contains( m_exp, Qt::CaseInsensitive )
              || track->artist().contains( m_exp, Qt::CaseInsensitive )
              || track->album().contains( m_exp, Qt::CaseInsensitive );

//        auto storedAudio = dynamic_cast< Tanyatu::Data::StoredAudio *>(track);
//        if( storedAudio != nullptr ) {
//              result = result || track->tags().contains( m_exp );
//        }
    }
    return result;
}


bool TrackFilter::lessThan(
    const QModelIndex &left, const QModelIndex &right) const {
    QVariant lData = left.data();
    QVariant rData = right.data();
    return QString::localeAwareCompare(lData.toString(), rData.toString()) < 0;
}


} }
