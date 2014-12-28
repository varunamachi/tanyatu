#include <QAudioDeviceInfo>
#include <QtGlobal>
#include <QList>


#include "MMAudioEngine.h"

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
    return qMax( m_player->duration() - m_player->position(), qint64( 0 ));
}


bool MMAudioEngine::isCurrentSeekable() const
{
    return m_player->isSeekable();
}


QStringList MMAudioEngine::supportedFileExtentions()
{
    QStringList extensions;

    QList< QAudioDeviceInfo > devices =
            QAudioDeviceInfo::availableDevices( QAudio::AudioOutput );
    foreach( QAudioDeviceInfo info, devices ) {
        qDebug() << info.deviceName() << "-->" << info.supportedCodecs();
    }



    extensions << "*.mp3";
    QAudioDeviceInfo info( QAudioDeviceInfo::defaultOutputDevice() );
    qDebug() << info.deviceName() << "*";
    QStringList formats = info.supportedCodecs();
    for( QString &fmts : formats ) {
        if( fmts == "mpeg ") {
            extensions << "*.mp3";
        }
        else if( fmts == "mp4" ) {
            extensions << "*.mp4" << "*.aac" << "*.m4a";
        }
        else if( fmts == "ogg" ) {
            extensions << "*.ogg";
        }
    }
    return extensions;
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
    if( m_player->mediaStatus() == QMediaPlayer::BufferedMedia ) {
        m_player->play();
    }
}


void MMAudioEngine::stop()
{
    m_player->stop();
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
    if( state == QMediaPlayer::PlayingState ) {
        emit playStarted( m_currentItem );
    }
    else if( state == QMediaPlayer::PausedState ) {
        emit paused( m_currentItem );
    }
    else if( state == QMediaPlayer::StoppedState ) {
        emit stopped( m_currentItem );
    }
}


void MMAudioEngine::positionChanged(quint64 pos)
{
    emit tick( pos );
    if( m_player->duration() - pos < 1000 ) {
        emit aboutToFinish( m_currentItem );
    }
}

void MMAudioEngine::mediaStatusChanged( QMediaPlayer::MediaStatus status )
{
    IEngine::State newState = IEngine::State::Error;
    switch( status ) {
    case QMediaPlayer::LoadingMedia      :
    {
        newState = IEngine::State::Loading;
    }
    case QMediaPlayer::BufferingMedia    :
    {
        newState = IEngine::State::Buffering;
    }

    case QMediaPlayer::StalledMedia      :
    case QMediaPlayer::BufferedMedia     :
    {
        newState = m_player->state() == QMediaPlayer::PlayingState
                                                    ? IEngine::State::Playing
                                                    : IEngine::State::Paused;
    }
    case QMediaPlayer::NoMedia           :
    case QMediaPlayer::LoadedMedia       :
    {
        newState = IEngine::State::Stopped;
    }

    case QMediaPlayer::EndOfMedia        :
    {
        newState = IEngine::State::Stopped;
        emit finished( m_currentItem );
    }

    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::InvalidMedia      :
        newState = IEngine::State::Error;
    }
    IEngine::State oldState = m_currentState;
    m_currentState = newState;
    emit stateChanged( newState, oldState );
}

void MMAudioEngine::errorOccured( QMediaPlayer::Error merror )
{

    switch( merror ) {
    case QMediaPlayer::ResourceError:
        emit error( "Could not load the media" );
        break;
    case QMediaPlayer::FormatError:
        emit error( "Invalid media format" );
        break;
    case QMediaPlayer::NetworkError:
        emit error( "Could not connect to network" );
        break;
    case QMediaPlayer::AccessDeniedError:
        emit error( "Denied permission to access the media" );
        break;
    case QMediaPlayer::ServiceMissingError:
        emit error( "Missing service error" );
        break;
    case QMediaPlayer::MediaIsPlaylist:
        emit error( "Invalid media provided" );
        break;
    default:
        break;
    }
}

} }
