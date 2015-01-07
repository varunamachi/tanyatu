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

#include <QAudioDeviceInfo>
#include <QtGlobal>
#include <QList>
#include <QThread>


#include "MMAudioEngine.h"
#include "../data/AudioTrack.h"

namespace Tanyatu { namespace Impl {

const QString MMAudioEngine::ENGINE_DESC = "QtMultimediaEngine";

MMAudioEngine::MMAudioEngine( QObject *parent )
    : IEngine( parent )
    , m_player( new QMediaPlayer( this ))
    , m_currentItem( nullptr )
    , m_currentState( IEngine::State::Stopped )
{
    connect( m_player,
             SIGNAL( mutedChanged( bool )),
             this,
             SIGNAL( muteStateChanged( bool )));

    connect( m_player,
             SIGNAL( mediaStatusChanged( QMediaPlayer::MediaStatus )),
             this,
             SLOT( mediaStatusChanged( QMediaPlayer::MediaStatus )));

    connect( m_player,
             SIGNAL( volumeChanged( int )),
             this,
             SIGNAL( volumeChanged( int )));
    connect( m_player,
             SIGNAL( stateChanged( QMediaPlayer::State )),
             this,
             SLOT( playerStateChangedTo(QMediaPlayer::State )));
    connect( m_player,
             SIGNAL( positionChanged( qint64 )),
             this,
             SLOT( positionChanged( qint64 )));
    connect( m_player,
             SIGNAL( error( QMediaPlayer::Error )),
             this,
             SLOT( errorOccured( QMediaPlayer::Error )));
    m_player->setNotifyInterval( 1000 );
}


IEngine::Type MMAudioEngine::type() const
{
    return IEngine::Type::Audio;
}


QString MMAudioEngine::engineDesc() const
{
    return ENGINE_DESC;
}


qint64 MMAudioEngine::currentItemTotalTime() const
{
    return m_player->duration();
}


qint64 MMAudioEngine::currentItemRemainingTime() const
{
    return qMax( currentItemTotalTime() - m_player->position(), qint64( 0 ));
}


bool MMAudioEngine::isCurrentSeekable() const
{
    return m_player->isSeekable();
}


QStringList MMAudioEngine::supportedFileExtentions()
{
    QStringList ext;
    ext << "*.mp3" << "*.m4a";
    return ext;
}


IEngine::State MMAudioEngine::state()
{
    return m_currentState;
}


int MMAudioEngine::volume()
{
    return m_player->volume();
}


Data::MediaItem* MMAudioEngine::currentItem()
{
    return m_currentItem;
}



void MMAudioEngine::setSource( Tanyatu::Data::MediaItem *item )
{
    if( item != nullptr ) {
        m_currentItem = item;
        m_player->setMedia( item->url() );
        emit sourceChanged( item );
    }
}


void MMAudioEngine::pause()
{
    m_player->pause();
}


void MMAudioEngine::play()
{
    m_player->play();
}


void MMAudioEngine::stop()
{
    m_player->stop();
    m_player->setMedia( QMediaContent() );
}


void MMAudioEngine::seek( qint64 millis )
{
    m_player->setPosition( qMin( millis, m_player->duration() ));
    emit positionSeeked( millis );
}


void MMAudioEngine::setVolume( int value )
{
    int volume = qMin( value, 100 );
    m_player->setVolume( volume );
}


void MMAudioEngine::mute( bool value )
{
    m_player->setMuted( value );
}


void MMAudioEngine::clear()
{
    m_player->stop();
    emit stopped( m_currentItem );
    m_currentItem = nullptr;
}


void MMAudioEngine::playerStateChangedTo( QMediaPlayer::State state )
{
    IEngine::State oldState = m_currentState;
    if( state == QMediaPlayer::PlayingState ) {
        m_currentState = IEngine::State::Playing;
        emit playStarted( m_currentItem );
    }
    else if( state == QMediaPlayer::PausedState ) {
        m_currentState = IEngine::State::Paused;
        emit paused( m_currentItem );
    }
    else if( state == QMediaPlayer::StoppedState ) {
        m_currentState = IEngine::State::Stopped;
        emit stopped( m_currentItem );
    }
    emit stateChanged( m_currentState, oldState );
}


void MMAudioEngine::positionChanged( qint64 pos )
{
    emit tick( pos );
//    if(( m_player->duration() - pos ) < 1000 ) {
//        emit aboutToFinish( m_currentItem );
//    }
}


void MMAudioEngine::mediaStatusChanged( QMediaPlayer::MediaStatus status )
{
    IEngine::State newState = IEngine::State::Unknown;
    switch( status ) {
    case QMediaPlayer::LoadingMedia : {
        newState = IEngine::State::Loading;
        break;
    }
    case QMediaPlayer::BufferingMedia : {
        newState = IEngine::State::Buffering;
        break;
    }
    case QMediaPlayer::StalledMedia :
    case QMediaPlayer::BufferedMedia : {
        newState = m_player->state() == QMediaPlayer::PlayingState
                ? IEngine::State::Playing
                : IEngine::State::Paused;
        break;
    }
    case QMediaPlayer::NoMedia : {
        newState = IEngine::State::Stopped;
        break;
    }
    case QMediaPlayer::LoadedMedia : {
        if( m_currentState != IEngine::State::Stopped ) {
            play();
            newState = IEngine::State::Playing;
        }
        break;
    }
    case QMediaPlayer::EndOfMedia : {
        newState = IEngine::State::Stopped;
        emit finished( m_currentItem );
        break;
    }
    case QMediaPlayer::InvalidMedia : {
        newState = IEngine::State::Error;
        break;
    }
    case QMediaPlayer::UnknownMediaStatus : {
        if( m_currentState != IEngine::State::Playing ) {
            newState = IEngine::State::Playing;
        }
        break;
    }
    }
    IEngine::State oldState = m_currentState;
    m_currentState = newState;
    emit stateChanged( newState, oldState );
}


void MMAudioEngine::errorOccured( QMediaPlayer::Error merror )
{
    QString errorMessage = "";
    switch( merror ) {
    case QMediaPlayer::ResourceError:
        errorMessage = "Could not load the media";
        break;
    case QMediaPlayer::FormatError:
        errorMessage = "Invalid media format";
        break;
    case QMediaPlayer::NetworkError:
        errorMessage = "Could not connect to network";
        break;
    case QMediaPlayer::AccessDeniedError:
        errorMessage = "Denied permission to access the media";
        break;
    case QMediaPlayer::ServiceMissingError:
        errorMessage = "Missing service error";
        break;
    case QMediaPlayer::MediaIsPlaylist:
        errorMessage = "Invalid media provided";
        break;
    default:
        errorMessage = "An error occured";
        break;
    }
    qDebug() << errorMessage;
    emit error( errorMessage );
}

} }
