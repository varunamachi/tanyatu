#pragma once

#include <QMediaPlayer>

#include "../TanyatuCoreGlobal.h"
#include "../interfaces/IEngine.h"
#include "../data/MediaItem.h"

namespace Tanyatu { namespace Impl {

class TANYATU_CORE_EXPORT MMAudioEngine : public IEngine
{
    Q_OBJECT

public:
    /**
      * Gives the type/identity of the engine
      */
    Type type() const;

    /**
      * Gives the short description about the engine.
      * \returns short description of the engine
      */
    QString engineDesc() const;


    /**
      * Gives the total duration of the currently loaded item in milliseconds
      * \returns total time of the current item.
      */
    qint64 currentItemTotalTime() const;


    /**
      * Gives the remaining time of the playback of current item.
      * \returns remaining play back time for current item
      */
    qint64 currentItemRemainingTime() const;

    /**
      * Tells whether the current item that is being played is seekable or not
      * \returns true if the current item is seekable
      */
    bool isCurrentSeekable() const;


    /**
      * Gives the file extentions of files of supported audio formats.
      * \returns set of file extentions of supported file types
      */
    QStringList supportedFileExtentions();

    /**
     * @brief state Returns the state of the engine
     * @return state of the engine
     */
    IEngine::State state();


    int volume();

    Data::MediaItem * currentItem();



    /**
      * Normal QObject constructor
      */
    explicit MMAudioEngine( QObject *parent = 0 );

    ~MMAudioEngine() { delete m_player; }

    static const QString ENGINE_DESC;


public slots:

    /**
      * Sets the source for the engine. Playback may be delayed for short time
      * till the player loads the item
      * \param media item for playing
      */
    void setSource( Tanyatu::Data::MediaItem *item );


    /**
      * Pauses the playback
      */
    void pause();

    /**
      * Initiate the play action. Actual playback can be delayed based on the
      * engine state.
      */
    void play();

    /**
      * Stops playback
      */
    void stop();


    /**
      * Sets the current time in the playback
      */
    void seek( qint64 millis );

    /**
      * sets the volume for the player
      * \param value value for the volume from 0 to 100 any other value will
      * be neglected
      */
    void setVolume( int value );

    /**
      * Sets the volume to 0 i.e muts the player.
      * \param value if true mutes the volume if not already muted, if false
      * unmutes the volume if it is already muted.
      */
    void mute( bool value );

    /**
     * @brief clear Stops playback and clears the current playing item
     */
    void clear();

private slots:
    void playerStateChangedTo( QMediaPlayer::State state );

    void positionChanged( quint64 pos );

    void mediaStatusChanged( QMediaPlayer::MediaStatus status );

    void errorOccured( QMediaPlayer::Error error );

private:
    QMediaPlayer *m_player;

    Data::MediaItem *m_currentItem;

    IEngine::State m_currentState;

};

} }

