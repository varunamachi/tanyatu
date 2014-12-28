#pragma once

#include "AudioTrackModel.h"
#include "../TanyatuCommonGlobal.h"

namespace Tanyatu { namespace Ui {

class TANYATU_GUI_EXPORT CommonTrackModel : public Tanyatu::Ui::AudioTrackModel
{
public:
    CommonTrackModel( int numCols, QObject *parent )
        : Tanyatu::Ui::AudioTrackModel( numCols, parent ) {

    }

    QVariant dataForTrack( Tanyatu::Data::StoredAudio *item, int column ) const
    {
        if( item ) {
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
