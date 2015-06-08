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



#include "PhononAudioEngine.h"

#ifdef Q_OS_LINUX

#include <QCoreApplication>
#include <QTime>

#include <phonon4qt5/phonon/BackendCapabilities>

#include "../data/MediaItem.h"


#define NUM_RETRIES 3

namespace Tanyatu { namespace Impl {


static inline void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs( 500 );
    while( QTime::currentTime() < dieTime );
//    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

PhononAudioEngine::PhononAudioEngine( QObject *parent)
    : IEngine( parent )
    , m_mediaObject( new Phonon::MediaObject(this) )
    , m_audioOutput( new Phonon::AudioOutput(Phonon::MusicCategory, this))
    , m_numRetries( NUM_RETRIES )

{
    m_mediaObject->setTickInterval(1000);
    Phonon::createPath(m_mediaObject, m_audioOutput);
    connect( m_mediaObject, 
             SIGNAL( aboutToFinish() ),
             this, 
             SLOT( onAboutToFinish()) );
    connect( m_mediaObject, 
             SIGNAL( finished() ), 
             this, 
             SLOT( onFinished() ));
    connect( m_mediaObject, 
             SIGNAL( stateChanged( Phonon::State,Phonon::State )),
             this, 
             SLOT( onStateChange( Phonon::State,Phonon::State )));
    connect( m_mediaObject, 
             SIGNAL( tick( qint64 )), 
             this, 
             SIGNAL( tick( qint64 )));
    connect( m_audioOutput, 
             SIGNAL( volumeChanged( qreal )),
             this, 
             SLOT( onVolumeChange( qreal )));
    connect( m_audioOutput, 
             SIGNAL( mutedChanged( bool )),
             this, 
             SIGNAL( muteStateChanged( bool )));
    m_currentItem = 0;
}


IEngine::Type PhononAudioEngine::type() const
{
    return Tanyatu::IEngine::Type::Audio;
}


QString PhononAudioEngine::engineDesc() const
{
    return QString( tr( "Phonon backend for Tanyatu" ));
}


qint64 PhononAudioEngine::currentItemTotalTime() const
{
    //The total time is not defined before the item is loaded
    if( m_currentItem ) {
        quint64 val = m_mediaObject->totalTime();
        return val;
    }
    return 0;
}


qint64 PhononAudioEngine::currentItemRemainingTime() const
{
    quint64 val = m_mediaObject->remainingTime();
    return val;
}


bool PhononAudioEngine::isCurrentSeekable() const
{
    return m_mediaObject->isSeekable();
}


void PhononAudioEngine::setSource( Data::MediaItem *item )
{
    if( item ) {
        m_currentItem = item;
        m_mediaObject->clear();
        m_mediaObject->setCurrentSource(
                    Phonon::MediaSource( item->url() ));
        emit sourceChanged( item );
        m_numRetries = NUM_RETRIES;
        this->play();
    }
}


void PhononAudioEngine::pause()
{
    m_mediaObject->pause();
    emit paused( m_currentItem );
}


void PhononAudioEngine::play()
{
    m_mediaObject->play();
    //playStarted will be emitted after the state changes to PlayingState
}


void PhononAudioEngine::stop()
{
    m_mediaObject->stop();
    emit stopped( m_currentItem );
}


void PhononAudioEngine::seek(qint64 millis)
{
    if( m_mediaObject->isSeekable() ) {
        m_mediaObject->seek(millis);
    }
}


int PhononAudioEngine::volume()
{
    return static_cast< int >( m_audioOutput->volume() * 100 );
}


void PhononAudioEngine::setVolume(int value) {
    if(value <= 100) {
        m_audioOutput->setVolume((static_cast<qreal>(value)) * 0.01);
        emit volumeChanged(value);
    }
}


void PhononAudioEngine::mute(bool value) {
    m_audioOutput->setMuted(value);
    emit muteStateChanged(value);
}


IEngine::State PhononAudioEngine::state()
{
    if( m_mediaObject )
    {
        return mapPhononStateToEngineState( m_mediaObject->state() );
    }
    return IEngine::State::Stopped;
}


void PhononAudioEngine::clear()
{
    m_mediaObject->stop();
    emit stopped( m_currentItem );
    m_mediaObject->clear();
    m_currentItem = 0;
}


void PhononAudioEngine::onStateChange(Phonon::State newState,
                                      Phonon::State oldState) {
    if(newState == Phonon::ErrorState) {
        if( m_numRetries ) {
            emit error(QString(
                           (m_mediaObject->errorType() == Phonon::FatalError ?
                                "Fatal Error: " :"Error: ")) +
                       m_mediaObject->errorString());
            delay();
            play();
            -- m_numRetries;
        }

    }
    else if( newState == Phonon::PlayingState ) {
        playStarted( m_currentItem );
        m_numRetries = NUM_RETRIES;
    }
    emit stateChanged( mapPhononStateToEngineState(newState),
                      mapPhononStateToEngineState(oldState) );
}


void PhononAudioEngine::onAboutToFinish() {
    emit aboutToFinish( m_currentItem );
}


void PhononAudioEngine::onFinished() {
    emit finished( m_currentItem );
}


void PhononAudioEngine::onVolumeChange(qreal value)
{
    emit volumeChanged( static_cast< int >( value * 100 ));
}


IEngine::State PhononAudioEngine::mapPhononStateToEngineState(
        Phonon::State phononState) {
    switch (phononState) {
    case Phonon::BufferingState: return IEngine::State::Buffering;
    case Phonon::LoadingState:   return IEngine::State::Loading;
    case Phonon::ErrorState:     return IEngine::State::Error;
    case Phonon::PlayingState:   return IEngine::State::Playing;
    case Phonon::StoppedState:   return IEngine::State::Stopped;
    case Phonon::PausedState:    return IEngine::State::Paused;
    }
    return IEngine::State::Stopped;
}


QStringList PhononAudioEngine::supportedFileExtentions()
{
    return QStringList() << "*.mp3";
}


Data::MediaItem *PhononAudioEngine::currentItem()
{
    return m_currentItem;
}


} } //Ending namespaces


#endif








