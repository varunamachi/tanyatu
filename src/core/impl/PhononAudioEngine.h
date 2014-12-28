/*******************************************************************************
 * PhononAudioEngine.h
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

#if 0

#pragma once

#include <QtGlobal>

//#ifdef Q_OS_LINUX

#include <QObject>
#include <phonon4qt5/phonon/MediaObject>
#include <phonon4qt5/phonon/AudioOutput>

#include "../data/MediaItem.h"
#include "../interfaces/IEngine.h"


namespace Tanyatu { namespace Impl {
/**
  * \class PhononAudioEngine represents the audio engine which uses the
  * Phonon framework for playback of media content.
  */
class TANYATU_CORE_EXPORT PhononAudioEngine : public IEngine
{
    Q_OBJECT
public:
    /**
      * Constructor that initializes the phonon audio engine for Tanyatu
      * \param mediaObject an initialized media object. This will be reparented
      * to the engine hence the creater of this object should not delete it.
      */
    explicit PhononAudioEngine( QObject *parent = 0 );

    /**
      * Destructor
      */
    ~PhononAudioEngine()
    {
        delete m_mediaObject;
        delete m_audioOutput;
    }

    /**
      * Inherited docs
      */
    IEngine::Type type() const;

    /**
      * Inherited docs
      */
    QString engineDesc() const;


    /**
      * Inherited docs
      */
    qint64 currentItemTotalTime() const;


    /**
      * Inherited docs
      */
    qint64 currentItemRemainingTime() const;

    /**
      * Inherited docs
      */
    bool isCurrentSeekable() const;

    /**
      * Inherited docs
      */
    QStringList supportedFileExtentions();

    Data::MediaItem* currentItem();

    int volume();
    
    /**
      * Inherited docs
      */
    void setSource( Tanyatu::Data::MediaItem *item);

    /**
      * Inherited docs
      */
    void pause();

    /**
      * Inherited docs
      */
    void play();

    /**
      * Inherited docs
      */
    void stop();

    /**
      * Inherited docs
      */
    void seek(qint64 millis);


    /**
      * Inherited docs
      */
    void setVolume(int value);


    /**
      * Inherited docs
      */
    void mute(bool value);

    /**
     * @brief state Returns the state of the engine
     * @return state of the engine
     */
    IEngine::State state();

    void clear();

private slots:

    /**
      * Converts the phonons media object state change to engine states
      * \param newState state after the change
      * \param oldState state before the change
      */
    void onStateChange( Phonon::State newState, Phonon::State oldState);

    /**
      * Converts media objects 'aboutToFinish' signal to engine's own
      */
    void onAboutToFinish();

    /**
      * Converts media objects 'finished' signal to engine's own
      */
    void onFinished();

    void onVolumeChange( qreal value );


private:
    /**
      * Media object to control the playback
      */
    Phonon::MediaObject *m_mediaObject;

    /**
      * Abstraction of sound output
      */
    Phonon::AudioOutput *m_audioOutput;

    int m_numRetries;

    /**
      * current item
      */
    Data::MediaItem *m_currentItem;

    /**
      * Maps phonon state to engine state
      * \param phononState state of the media object
      * \returns engine state
      */
    State mapPhononStateToEngineState(Phonon::State phononState);
};

}

}


#endif

