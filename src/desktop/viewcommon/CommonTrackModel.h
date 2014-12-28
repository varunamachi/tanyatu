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

#pragma once

#include <uicommon/itemmodels/AudioTrackModel.h>

namespace GreenChilli { namespace Views {

class CommonTrackModel : public Tanyatu::Ui::AudioTrackModel
{
public:
    CommonTrackModel( int numCols, QObject *parent )
        : Tanyatu::Ui::AudioTrackModel( numCols, parent ) {

    }

    QVariant dataForTrack( Tanyatu::Data::StoredAudio *item, int column ) const
    {
        if( item != nullptr ) {
            switch( column ) {
            case 0: return item->trackId();
            case 1: return item->title();
            case 2: return item->artist();
            case 3: return item->album();
            case 4: return item->rating();
            }
        }
        return "";
    }


    QString headerForColumn( int column ) const
    {
        switch( column ) {
        case 0: return tr( "TrackId" );
        case 1: return tr( "Title" );
        case 2: return tr( "Artist" );
        case 3: return tr( "Album" );
        case 4: return tr( "Rating" );
        }
        return "";
    }
};

} }
