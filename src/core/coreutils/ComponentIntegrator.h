#pragma once

#include <QObject>
#include <QCoreApplication>

#include "../data/StoredAudio.h"

namespace Tanyatu {

class ComponentIntegrator : public QObject
{
    Q_OBJECT
public:
    ComponentIntegrator()
        : QObject( QCoreApplication::instance() ) {     }

    void integrate();

    friend class T;

private slots:
    void onTrackRemovalFromAudioLib(
            Tanyatu::Data::StoredAudio * removedTracks );

    void onAudioLibClear();

};

} //end of namespace
