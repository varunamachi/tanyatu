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

#include <QObject>
#include <QSet>
#include <QStringList>

#include "../data/MediaItem.h"
#include "../TanyatuCoreGlobal.h"

namespace Tanyatu
{




/**
  * \class IEngine serves as the base class for all the media engines
  * responsible for playback of media content. AudioEngines and VideoEngines
  * inherit from this class
  */
class TANYATU_CORE_EXPORT IEngine : public QObject
{
    Q_OBJECT

public:
    enum class Type
    {
        Audio,
        Video
    };

    enum class State
    {
        Playing,
        Loading,
        Buffering,
        Paused,
        Stopped,
        Error
    };

    /**
      * Gives the type/identity of the engine
      */
    virtual Type type() const = 0;

    /**
      * Gives the short description about the engine.
      * \returns short description of the engine
      */
    virtual QString engineDesc() const = 0;


    /**
      * Gives the total duration of the currently loaded item in milliseconds
      * \returns total time of the current item.
      */
    virtual qint64 currentItemTotalTime() const = 0;


    /**
      * Gives the remaining time of the playback of current item.
      * \returns remaining play back time for current item
      */
    virtual qint64 currentItemRemainingTime() const = 0;

    /**
      * Tells whether the current item that is being played is seekable or not
      * \returns true if the current item is seekable
      */
    virtual bool isCurrentSeekable() const = 0;


    /**
      * Gives the file extentions of files of supported audio formats.
      * \returns set of file extentions of supported file types
      */
    virtual QStringList supportedFileExtentions() = 0;

    /**
     * @brief state Returns the state of the engine
     * @return state of the engine
     */
    virtual State state() = 0;

    virtual int volume() = 0;

    virtual Data::MediaItem* currentItem() = 0;



    /**
      * Normal QObject constructor
      */
    explicit IEngine(QObject *parent = 0) :
        QObject(parent) {

    }

    virtual ~IEngine()
    {

    }

signals:

    /**
     * @brief sourceChanged Emitted when a media source is set as current source
     * @param item Item which is set.
     */
    void sourceChanged( Tanyatu::Data::MediaItem *item );

    /**
      * Signals the start of playback
      * \param pointer to the item for which the signal is emitted.
      */
    void playStarted( Tanyatu::Data::MediaItem *item );

    /**
      * Signals the pause action for the engine
      * \param item [out] pointer to the item for which the signal is emitted.
      */
    void paused( Tanyatu::Data::MediaItem *item );


    /**
      * Signals the end / manual stop of playback
      * \param [out] the media item whose playback has been stopped
      */
    void stopped( Tanyatu::Data::MediaItem *item );


    /**
      * This signal is emitted just before the track finishes playing
      * \param [out] item which is going to finish soon
      */
    void aboutToFinish( Tanyatu::Data::MediaItem *item);


    /**
      * signals a time tick idealy 1ms
      * \param [out] time tick in milli seconds
      */
    void tick( qint64 tick );


    /**
      * Notifies the change in volume of the player
      * \param [out] newValue value to after change
      */
    void volumeChanged( int newValue );


    /** Notifies change in the mute state
      * \param [out] newValue value after change
      */
    void muteStateChanged( bool newValue );


    /**
      * In case of error this signal is fired with the error message
      * \param [out] error message
      */
    void error( QString message );


    /**
      * Notifies when the current position in the media playback is changed
      * \param value [out] the new time corresponding to new position in milli
      * seconds
      */
    void positionSeeked( quint64 newMs );


    /**
      * Signals the change of player/engine state
      * \param newState the new state of the engine
      * \param oldState the old state of the engine
      */
    void stateChanged( Tanyatu::IEngine::State newState,
                       Tanyatu::IEngine::State oldState);

    /**
      * signals the playback finish of the item
      * \param item which has finished playback
      */
    void finished( Tanyatu::Data::MediaItem *item );


public slots:

    /**
      * Sets the source for the engine. Playback may be delayed for short time
      * till the player loads the item
      * \param media item for playing
      */
    virtual void setSource( Tanyatu::Data::MediaItem *item ) = 0;


    /**
      * Pauses the playback
      */
    virtual void pause() = 0;

    /**
      * Initiate the play action. Actual playback can be delayed based on the
      * engine state.
      */
    virtual void play() = 0;

    /**
      * Stops playback
      */
    virtual void stop() = 0;


    /**
      * Sets the current time in the playback
      */
    virtual void seek( qint64 millis ) = 0;

    /**
      * sets the volume for the player
      * \param value value for the volume from 0 to 100 any other value will
      * be neglected
      */
    virtual void setVolume( int value ) = 0;

    /**
      * Sets the volume to 0 i.e muts the player.
      * \param value if true mutes the volume if not already muted, if false
      * unmutes the volume if it is already muted.
      */
    virtual void mute( bool value ) = 0;

    /**
     * @brief clear Stops playback and clears the current playing item
     */
    virtual void clear() = 0;
};
}
