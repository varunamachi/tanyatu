#include "ComponentIntegrator.h"
#include "T.h"

namespace Tanyatu {

void ComponentIntegrator::integrate()
{
    /**
     * Below we connect signals from a standard component to other standard
     * component slots, so that client does not have to bother about the
     * internal consistancies
     */

    //Connecting audio engine signals to others
    QObject::connect(
             AUDIO_ENGINE(),
             SIGNAL( playStarted( Tanyatu::Data::MediaItem* )),
             AUDIO_LIB(),
             SLOT( onItemPlaybackStarted( Tanyatu::Data::MediaItem* )));

    QObject::connect(
             AUDIO_ENGINE(),
             SIGNAL( playStarted( Tanyatu::Data::MediaItem* )),
             TRACKINF_MANAGER(),
             SLOT( onItemPlaybackStarted( Tanyatu::Data::MediaItem* )));

    QObject::connect(
             AUDIO_ENGINE(),
             SIGNAL( playStarted( Tanyatu::Data::MediaItem* )),
             HISTORY_MANAGER(),
             SLOT( onItemPlayed( Tanyatu::Data::MediaItem* )));

    QObject::connect(
             AUDIO_ENGINE(),
             SIGNAL( finished( Tanyatu::Data::MediaItem* )),
             AUDIO_LIB(),
             SLOT( onItemPlaybackFinished( Tanyatu::Data::MediaItem* )));

    QObject::connect(
             AUDIO_ENGINE(),
             SIGNAL( finished( Tanyatu::Data::MediaItem* )),
             TRACKINF_MANAGER(),
             SLOT( onItemPlaybackFinished( Tanyatu::Data::MediaItem* )));

    QObject::connect(
             AUDIO_LIB(),
             SIGNAL( itemAdded( Tanyatu::Data::StoredAudio * )),
             TRACKINF_MANAGER(),
             SLOT( onAdditionToStorage( Tanyatu::Data::StoredAudio * )));


    QObject::connect(
             AUDIO_LIB(),
             SIGNAL( aboutToRemoveItem( Tanyatu::Data::StoredAudio * )),
             this,
             SLOT( onTrackRemovalFromAudioLib( Tanyatu::Data::StoredAudio *)));

    QObject::connect(
             AUDIO_LIB(),
             SIGNAL( aboutToClear() ),
             this,
             SLOT( onAudioLibClear() ));

    QObject::connect(
             AUDIO_LIB(),
             SIGNAL( itemChanged( Tanyatu::Data::StoredAudio* )),
             TRACKINF_MANAGER(),
             SLOT( onItemModified( Tanyatu::Data::StoredAudio* )));

    QObject::connect(
             PLAYQUEUE(),
             SIGNAL( clearingPlayQueue() ),
             AUDIO_ENGINE(),
             SLOT( clear() ));

    QObject::connect(
             PLAYQUEUE(),
             SIGNAL( removingSelectedItem() ),
             AUDIO_ENGINE(),
             SLOT( clear() ));
}



void ComponentIntegrator::onTrackRemovalFromAudioLib(
        Tanyatu::Data::StoredAudio *track )
{
    PLAYQUEUE()->removeItem( track->trackId() );
    TRACKINF_MANAGER()->onRemovalFromStorage( track );
    HISTORY_MANAGER()->onItemRemovalFromStorage( track->trackId() );
    PL_MANAGER()->onItemRemovalFromStorage( track->trackId() );
}


void ComponentIntegrator::onAudioLibClear()
{
    TRACKINF_MANAGER()->clear( false );
    PLAYQUEUE()->clear();
    PL_MANAGER()->clear();
    HISTORY_MANAGER()->clear();
    PLAYQUEUE()->clearAllStoredItems();
}


} //end of namespace
