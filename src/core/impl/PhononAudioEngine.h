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

