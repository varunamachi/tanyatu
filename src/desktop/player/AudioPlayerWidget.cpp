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

#include <QVBoxLayout>
#include <QString>
#include <QFileInfo>
#include <QLabel>
#include <QDropEvent>
#include <QMimeData>

#include <core/T.h>
#include <core/data/AudioTrack.h>
#include <core/coreutils/Utils.h>

#include  "ui_AudioPlayer.h"
#include "AudioPlayerWidget.h"




using namespace GreenChilli;


AudioPlayerWidget::AudioPlayerWidget(QWidget *parent) :
    QWidget(parent),
    m_engine( Tanyatu::T::get()->audioEngine() )
{
    init();
    setupButtons();
}


void AudioPlayerWidget::init()
{
    this->setFixedSize( AUDIO_PLAYER_WIDTH, AUDIO_PLAYER_HEIGHT );
    m_playerWidget = new Ui::AudioPlayer();
    m_playerWidget->setupUi( this );
    m_playerWidget->seekSlider->setWidth( 250 );
    m_playerWidget->seekSlider->setHeight( 3 );

    m_playerWidget->volumeSlider->setWidth( 80 );
    m_playerWidget->volumeSlider->setMaxValue( 100 );
    m_playerWidget->volumeSlider->setHeight( 3 );
    QPalette pal = this->palette();
    this->setAutoFillBackground( true );
    pal.setBrush( QPalette::Window, QBrush( QImage( ":/images/background" )));
    this->setPalette( pal );
    this->setAcceptDrops( true );

    setScrollingText();

    connect( m_engine,
             SIGNAL( stateChanged( Tanyatu::IEngine::State,
                                   Tanyatu::IEngine::State )),
             this,
             SLOT(engineStateChanged( Tanyatu::IEngine::State,
                                      Tanyatu::IEngine::State )));
    connect( m_engine, SIGNAL( tick(qint64) ),
             m_playerWidget->seekSlider, SLOT( setCurrentValue( qint64 )));
    connect( m_playerWidget->seekSlider, SIGNAL( seeked( qint64 )),
             m_engine, SLOT( seek(qint64 )));
    connect( m_playerWidget->volumeSlider, SIGNAL( seeked( qint64 )),
             this, SLOT( setVolume( qint64 )));
    connect( m_playerWidget->volumeSlider, SIGNAL( valueChanged( qint64 )),
             this, SLOT( setVolumeLabelText( qint64 )));
    connect( m_playerWidget->muteButton, SIGNAL( clicked( bool )),
             m_engine, SLOT( mute( bool )));
    connect( m_playerWidget->chilli, SIGNAL( clicked( bool )),
             this, SLOT( onChilliClicked(bool) ));
    connect( m_engine, SIGNAL( volumeChanged( int )),
             this, SLOT( setVolmeSliderValue( int )));
    connect( m_playerWidget->shuffleButton, SIGNAL( clicked( bool )),
             PLAYQUEUE(), SLOT( setRandom( bool )));
    connect( m_playerWidget->nextButton, SIGNAL( clicked() ),
             PLAYQUEUE(), SLOT( selectNext() ));
    connect( m_playerWidget->prevButton, SIGNAL( clicked() ),
             PLAYQUEUE(), SLOT( selectPrevious() ));
    connect( PLAYQUEUE(), SIGNAL( playQueueChanged() ),
             this, SLOT( updatePlayerActions() ));
    connect( m_playerWidget->repeatButton, SIGNAL( clicked() ),
             this, SLOT( onRepeatButtonClicked() ));
    connect( m_engine, SIGNAL( sourceChanged( Tanyatu::Data::MediaItem * )),
             this, SLOT( onTrackSelected( Tanyatu::Data::MediaItem * )));
    connect( m_engine, SIGNAL( finished(Tanyatu::Data::MediaItem*) ),
             this, SLOT( gotoBoringMode() ));
    connect( m_engine, SIGNAL( stopped( Tanyatu::Data::MediaItem * )),
             this, SLOT( gotoBoringMode() ));
    connect( m_engine, SIGNAL( tick( qint64 )),
             this, SLOT( onPlayerTick( qint64 )));
    m_playerWidget->seekSlider->setEnabled( false );
    m_playerWidget->volumeSlider->setEnabled( false );
    updatePlayerActions();
    gotoBoringMode();
}



void AudioPlayerWidget::setupButtons()
{

    m_playerWidget->prevButton->setIcons( QIcon( ":/images/prev"),
                                          QIcon( ":/images/prev_act" ),
                                          QSize( 32, 32 ) );
//    m_playerWidget->playButton->setCheckable( true );
    m_playerWidget->stopButton->setEnabled( false );
    m_playerWidget->playButton->setIcons( QIcon( ":/images/play"),
                                          QIcon( ":/images/play_act" ),
                                          QSize( 32, 32 ) );
    m_playerWidget->stopButton->setIcons( QIcon( ":/images/stop"),
                                          QIcon( ":/images/stop_act" ),
                                          QSize( 32, 32 ) );
    m_playerWidget->nextButton->setIcons( QIcon( ":/images/next"),
                                          QIcon( ":/images/next_act" ),
                                          QSize( 32, 32 ) );
    m_playerWidget->shuffleButton->setIcons( QIcon( ":/images/shuffle" ),
                                            QIcon( ":/images/shuffle_act" ),
                                            QSize( 24, 24 ) );
    m_playerWidget->shuffleButton->setCheckable( true );
    m_playerWidget->repeatButton->setIcons( QIcon( ":/images/repeatall_act" ),
                                            QIcon( ":/images/repeatone_act" ),
                                            QSize( 24, 24 ) );

    m_playerWidget->chilli->setIcons( QIcon( ":/images/chilliabt" ),
                                            QIcon( ":/images/chilliabt_act" ),
                                            QSize( 24, 24 ) );

    m_playerWidget->muteButton->setCheckable( true );
    m_playerWidget->muteButton->setIcons( QIcon( ":/images/muteoff" ),
                                          QIcon( ":/images/muteon" ),
                                          QSize( 16, 16 ) );
    connect( m_playerWidget->stopButton, SIGNAL( clicked() ),
             m_engine, SLOT( stop() ));
    connect( m_playerWidget->playButton, SIGNAL( clicked() ),
             this, SLOT( onPlayButtonClicked() ));
}



void AudioPlayerWidget::engineStateChanged(
        Tanyatu::IEngine::State newState,
        Tanyatu::IEngine::State oldState)
{
    Q_UNUSED( oldState )
    updatePlayerActions();
    if( ! m_engine->currentItem()
        || newState == Tanyatu::IEngine::State::Error
        || newState == Tanyatu::IEngine::State::Loading
        || newState == Tanyatu::IEngine::State::Buffering )
    {
        m_playerWidget->playButton->setEnabled( false );
        m_playerWidget->stopButton->setEnabled( false );
        m_playerWidget->seekSlider->setEnabled( false );
        m_playerWidget->volumeSlider->setEnabled( false );
    }
    if( newState == Tanyatu::IEngine::State::Playing )
    {
        m_playerWidget->seekSlider->setMaxValue(
                    m_engine->currentItemTotalTime() );
        m_playerWidget->playButton->setEnabled( true );
        m_playerWidget->stopButton->setEnabled( true);
        m_playerWidget->playButton->setIcons( QIcon( ":/images/pause_act" ),
                                             QIcon( ":/images/pause_act" ),
                                             QSize( 32, 32 ));
         m_playerWidget->seekSlider->setEnabled( true );
         m_playerWidget->volumeSlider->setEnabled( true );
         if( m_engine->volume() > 100 ) {
             m_engine->setVolume( 100 );
         }
         m_playerWidget->volumeSlider->setCurrentValue( m_engine->volume() );
    }
    else if( newState == Tanyatu::IEngine::State::Paused )
    {
        m_playerWidget->playButton->setEnabled( true );
        m_playerWidget->playButton->setIcons( QIcon( ":/images/play" ),
                                              QIcon( ":/images/play_act" ),
                                              QSize( 32, 32 ));
         m_playerWidget->seekSlider->setEnabled( false );
         m_playerWidget->volumeSlider->setEnabled( true );

    }
    else if( newState == Tanyatu::IEngine::State::Stopped )
    {
        m_playerWidget->playButton->setEnabled( true );
        m_playerWidget->stopButton->setEnabled( false );
        m_playerWidget->playButton->setIcons( QIcon( ":/images/play" ),
                                              QIcon( ":/images/play_act" ),
                                              QSize( 32, 32 ));
        m_playerWidget->seekSlider->setEnabled( false );
        m_playerWidget->volumeSlider->setEnabled( false );
        m_playerWidget->seekSlider->setCurrentValue( 0 );
    }
}

void AudioPlayerWidget::setVolume(qint64 value)
{
    m_engine->setVolume( static_cast< int >( value ));
}

void AudioPlayerWidget::setVolmeSliderValue( int volume )
{
    m_playerWidget->volumeSlider->setCurrentValue( volume );
    m_playerWidget->volumeLable->setText( QString::number( volume ));
}


void AudioPlayerWidget::setVolumeLabelText( qint64 value )
{
    m_playerWidget->volumeLable->setText( QString::number( value ));
}


void AudioPlayerWidget::onPlayButtonClicked()
{
    if( ! m_playerWidget->playButton->isChecked()
        && m_engine->state() == Tanyatu::IEngine::State::Playing ) {
        m_engine->pause();
        m_playerWidget->seekSlider->setEnabled( false );
    }
    else {
        if( m_engine->currentItem() ) {
            m_engine->play();
        }
        else {
            emit playFromPlaylist();
        }
    }
}


void AudioPlayerWidget::onRepeatButtonClicked()
{
    Tanyatu::IPlayQueue::RepeatType repeatType =
            PLAYQUEUE()->repeatType();
    switch( repeatType )
    {
    case Tanyatu::IPlayQueue::RepeatType_NoRepeat:
        PLAYQUEUE()->setRepeat(
                  Tanyatu::IPlayQueue::RepeatType_RepeatAll );
        m_playerWidget->repeatButton->setIcons(
                    QIcon( ":/images/repeatall_act" ),
                    QIcon( ":/images/repeatone_act"),
                    QSize( 24, 24 ));
        break;

    case Tanyatu::IPlayQueue::RepeatType_RepeatAll:
        PLAYQUEUE()->setRepeat(
                  Tanyatu::IPlayQueue::RepeatType_RepeatOne );
        m_playerWidget->repeatButton->setIcons(
                    QIcon( ":/images/repeatone_act" ),
                    QIcon( ":/images/repeatall"),
                    QSize( 24, 24 ));
        break;

    case Tanyatu::IPlayQueue::RepeatType_RepeatOne:
        PLAYQUEUE()->setRepeat(
                  Tanyatu::IPlayQueue::RepeatType_NoRepeat );
        m_playerWidget->repeatButton->setIcons(
                    QIcon( ":/images/repeatall" ),
                    QIcon( ":/images/repeatall_act"),
                    QSize( 24, 24 ));
        break;
    }
}

void AudioPlayerWidget::updatePlayerActions()
{
    m_playerWidget->nextButton->setEnabled( PLAYQUEUE()->hasNext() );
    m_playerWidget->prevButton->setEnabled( PLAYQUEUE()->hasPrev() );
}


void AudioPlayerWidget::onTrackSelected( Tanyatu::Data::MediaItem * item )
{
    if( item && ( item->type() == Tanyatu::Data::Media_LocalAudio
            || item->type() == Tanyatu::Data::Media_StoredAudio )) {
        m_playerWidget->chilli->setChecked( false );
        Tanyatu::Data::AudioTrack *track =
                static_cast< Tanyatu::Data::AudioTrack *>( item );
        showTrackDetails( track );
        m_playerWidget->bitrate->setText( QString( "%1kbps" ).arg(
                                              track->bitRate() ));
        m_playerWidget->sampleFreq->setText(
                    QString( "%1.%2KHz" )
                    .arg( track->sampleRate() / 1000 )
                    .arg( track->sampleRate() % 1000 / 100 ));
        QFileInfo info( track->url().toLocalFile() );
        m_playerWidget->format->setText( info.suffix().toUpper() );
        m_playerWidget->elapsedTime->setText( "00:00" );
        m_playerWidget->remainingTime->setText(
                    Tanyatu::Utils::getStringTime( track->duration() ));

        QImage *image = nullptr;//Tanyatu::Utils::imageForTrack( track );
        image = image != nullptr ? image
                                 : new QImage( ":/images/defaultcover" );
        QPixmap pixmap;
        pixmap.convertFromImage( *image );
        delete image;
        QPixmap background = QPixmap( ":/images/background" ).scaled(
                    this->size() );
        QPixmap result( background.size() );
        result.fill( Qt::transparent );
        QPainter painter( &result );
        painter.drawPixmap( 0, 0, pixmap.scaled( background.size() ));
        painter.setOpacity( 0.70 );
        painter.drawPixmap( 0, 0, background );
        QPalette pal = this->palette();
        pal.setBrush( QPalette::Window,
                      QBrush( result ));
        this->setPalette( pal );

    }
}


void AudioPlayerWidget::onPlayerTick( qint64 msecEllapsed )
{
    Tanyatu::Data::MediaItem *item = m_engine->currentItem();
    if( item && ( item->type() == Tanyatu::Data::Media_LocalAudio ||
                  item->type() == Tanyatu::Data::Media_StoredAudio ))
    {
        Tanyatu::Data::AudioTrack *track =
                static_cast< Tanyatu::Data::AudioTrack *>( item );
        m_playerWidget->elapsedTime->setText( Tanyatu::Utils::getStringTime(
                                                  msecEllapsed / 1000 ));
        m_playerWidget->remainingTime->setText(
                    Tanyatu::Utils::getStringTime(
                        track->duration() - ( msecEllapsed /1000 )));
    }
}


void AudioPlayerWidget::onChilliClicked( bool checked )
{
    if( checked ) {
        m_playerWidget->scroller->setScrollText(
                    "( GREENCHILLI + TANYATU ) by: VARUNA L AMACHI ", true );
    }
    else {
        Tanyatu::Data::MediaItem *item = m_engine->currentItem();
        if( item && ( item->type() == Tanyatu::Data::Media_LocalAudio
                      || item->type() == Tanyatu::Data::Media_StoredAudio )) {
            Tanyatu::Data::AudioTrack *track =
                    static_cast< Tanyatu::Data::AudioTrack *>( item );
            showTrackDetails( track );
        }
        else {
            setScrollingText();
        }
    }
}


void AudioPlayerWidget::showTrackDetails( Tanyatu::Data::AudioTrack *track )
{
    QString trackString;
    if( ! track->title().isEmpty() ) {
        trackString  += track->title();
    }
    if( ! track->artist().isEmpty() ) {
        trackString  += QString( "  ~  " ) + track->artist();
    }
    if( ! track->album().isEmpty() ) {
        trackString  += QString( "  ~  " ) + track->album();
    }
    trackString += QString( "  ~  " ) + Tanyatu::Utils::getStringTime(
                track->duration() );
    m_playerWidget->scroller->setScrollText( trackString );
}



void AudioPlayerWidget::dragEnterEvent( QDragEnterEvent * event )
{
    if( event->mimeData()->hasFormat( "text/uri-list" )) {
        event->acceptProposedAction();
    }
}


void AudioPlayerWidget::dropEvent( QDropEvent *event )
{
    emit urlsDropped( event->mimeData()->urls() );
}

