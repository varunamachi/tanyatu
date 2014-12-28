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
