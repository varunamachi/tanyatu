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

#define NO_MARGINS( layout ) \
    layout->setContentsMargins( QMargins() ); \


using namespace GreenChilli;


AudioPlayerWidget::AudioPlayerWidget(QWidget *parent) :
    QWidget(parent),
    m_engine( Tanyatu::T::get()->audioEngine() )
{
    setupUi();
    setupConnections();
    setupButtons();
}


void AudioPlayerWidget::setupUi()
{
    m_scroller      = new GreenChilli::Widgets::TextScroller( this );
    m_seekSlider    = new GreenChilli::Widgets::SuperSlider( this );

    m_prevButton    = new GreenChilli::Widgets::ImagePushButton( this );
    m_stopButton    = new GreenChilli::Widgets::ImagePushButton( this );
    m_playButton    = new GreenChilli::Widgets::ImagePushButton( this );
    m_nextButton    = new GreenChilli::Widgets::ImagePushButton( this );

    m_shuffleButton = new QPushButton( tr( "SHFL" ));
    m_repeatButton  = new QPushButton( tr( "OFF"));

    m_muteButton    = new GreenChilli::Widgets::ImagePushButton();
    m_volumeLable   = new QLabel( "100" );
    m_volumeSlider  = new GreenChilli::Widgets::SuperSlider();

    m_elapsedTime   = new QLabel( "--:--" );
    m_remainingTime = new QLabel( "--:--" );
    m_bitrate       = new QLabel( "--- kbps" );
    m_sampleFreq    = new QLabel( "-- Hz" );
    m_format        = new QLabel( "****" );

    m_muteButton->setIcons( QIcon( ":/images/muteoff" ),
                            QIcon( ":/images/muteon" ),
                            QSize( 16, 16 ));
    m_prevButton->setIcons( QIcon( ":/images/prev"),
                            QIcon( ":/images/prev_act" ),
                            QSize( 32, 32 ));
    m_stopButton->setIcons( QIcon( ":/images/stop"),
                            QIcon( ":/images/stop_act" ),
                            QSize( 32, 32 ) );
    m_playButton->setIcons( QIcon( ":/images/play"),
                            QIcon( ":/images/play_act" ),
                            QSize( 32, 32 ));
    m_nextButton->setIcons( QIcon( ":/images/next"),
                            QIcon( ":/images/next_act" ),
                            QSize( 32, 32 ));

    QHBoxLayout *lytPlayCtrl = new QHBoxLayout();
    lytPlayCtrl->addWidget( m_prevButton );
    lytPlayCtrl->addWidget( m_stopButton );
    lytPlayCtrl->addWidget( m_playButton );
    lytPlayCtrl->addWidget( m_nextButton );

    QHBoxLayout *lytVolume = new QHBoxLayout();
    lytVolume->addWidget( m_muteButton );
    lytVolume->addWidget( m_volumeSlider );
    lytVolume->addWidget( m_volumeLable );
    lytVolume->setSpacing( 2 );
//    volumeSlider->setHeight( 19 );
    m_muteButton->setCheckable( true );
    QWidget *volWidget = new QWidget( this );
    volWidget->setLayout( lytVolume );
    volWidget->setMaximumWidth( 140 );

    QHBoxLayout *lytSeeker = new QHBoxLayout();
    lytSeeker->addWidget( m_elapsedTime );
    lytSeeker->addWidget( m_seekSlider );
    lytSeeker->addWidget( m_remainingTime );
    m_seekSlider->setWidth( 200 );

    QVBoxLayout *lytScrollerSeeker = new QVBoxLayout();
    lytScrollerSeeker->addWidget( m_scroller );
    lytScrollerSeeker->addLayout( lytSeeker );
    lytScrollerSeeker->setSpacing( 0 );
    m_scroller->setScrollText( " G R E E N C H I L L I ", true );
    m_scroller->setParameters( Widgets::TextScroller::ScrollEffect::Bounce,
                             100,
                             32,
                             0 );

    QVBoxLayout *lytRepShfl = new QVBoxLayout();
    lytRepShfl->addWidget( m_repeatButton );
    lytRepShfl->addWidget( m_shuffleButton );
    m_shuffleButton->setCheckable( true );
    QString stylesheet =  " QPushButton {"
                          "     border-radius: 5px;"
                          "     background-color: regba( 32, 32, 32, 200 );"
                          "     max-width: 30px;"
                          "     max-height:14px;"
                          "     min-width: 30px;"
                          "     min-height: 14px;"
                          "     font-size: 8px;"
                          " }"
                          " QPushButton:checked {"
                          "     background-color: #FFA858;"
                          "     color: #202020;"
                          " }"
                          " QPushButton:hover{"
                          "     background-color: #FFA858;"
                          "     color: red;"
                          " }";
    m_shuffleButton->setStyleSheet( stylesheet );
    m_repeatButton->setStyleSheet( stylesheet );


    QHBoxLayout *lytMaster = new QHBoxLayout();
    lytMaster->addLayout( lytPlayCtrl );
//    lytMaster->addLayout( lytVolume );
    lytMaster->addWidget( volWidget );
    lytMaster->addLayout( lytScrollerSeeker );
    lytMaster->addLayout( lytRepShfl );
    this->setLayout( lytMaster );
    NO_MARGINS( lytMaster );

    m_bitrate    ->hide();
    m_sampleFreq ->hide();
    m_format     ->hide();

    QPalette pal = this->palette();
    this->setAutoFillBackground( true );
//    pal.setBrush( QPalette::Window, QBrush( QImage( ":/images/background" )));
    pal.setBrush( QPalette::Window, Qt::black );
    this->setPalette( pal );
    this->setAcceptDrops( true );

    setScrollingText();

    this->setFixedHeight( 48 );
}


void AudioPlayerWidget::setupConnections()
{
    connect( m_engine,
             SIGNAL( stateChanged( Tanyatu::IEngine::State,
                                   Tanyatu::IEngine::State )),
             this,
             SLOT(engineStateChanged( Tanyatu::IEngine::State,
                                      Tanyatu::IEngine::State )));
    connect( m_engine, SIGNAL( tick(qint64) ),
             m_seekSlider, SLOT( setCurrentValue( qint64 )));
    connect( m_seekSlider, SIGNAL( seeked( qint64 )),
             m_engine, SLOT( seek(qint64 )));
    connect( m_volumeSlider, SIGNAL( seeked( qint64 )),
             this, SLOT( setVolume( qint64 )));
    connect( m_volumeSlider, SIGNAL( valueChanged( qint64 )),
             this, SLOT( setVolumeLabelText( qint64 )));
    connect( m_muteButton, SIGNAL( clicked( bool )),
             m_engine, SLOT( mute( bool )));
    connect( m_engine, SIGNAL( volumeChanged( int )),
             this, SLOT( setVolmeSliderValue( int )));
    connect( m_shuffleButton, SIGNAL( clicked( bool )),
             PLAYQUEUE(), SLOT( setRandom( bool )));
    connect( m_nextButton, SIGNAL( clicked() ),
             PLAYQUEUE(), SLOT( selectNext() ));
    connect( m_prevButton, SIGNAL( clicked() ),
             PLAYQUEUE(), SLOT( selectPrevious() ));
    connect( PLAYQUEUE(), SIGNAL( playQueueChanged() ),
             this, SLOT( updatePlayerActions() ));
    connect( m_repeatButton, SIGNAL( clicked() ),
             this, SLOT( onRepeatButtonClicked() ));
    connect( m_engine, SIGNAL( sourceChanged( Tanyatu::Data::MediaItem * )),
             this, SLOT( onTrackSelected( Tanyatu::Data::MediaItem * )));
    connect( m_engine, SIGNAL( finished(Tanyatu::Data::MediaItem*) ),
             this, SLOT( gotoBoringMode() ));
    connect( m_engine, SIGNAL( stopped( Tanyatu::Data::MediaItem * )),
             this, SLOT( gotoBoringMode() ));
    connect( m_engine, SIGNAL( tick( qint64 )),
             this, SLOT( onPlayerTick( qint64 )));
    m_seekSlider->setEnabled( false );
    m_volumeSlider->setEnabled( false );
    updatePlayerActions();
    gotoBoringMode();
}



void AudioPlayerWidget::setupButtons()
{

    connect( m_stopButton, SIGNAL( clicked() ),
             m_engine, SLOT( stop() ));
    connect( m_playButton, SIGNAL( clicked() ),
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
        m_playButton->setEnabled( false );
        m_stopButton->setEnabled( false );
        m_seekSlider->setEnabled( false );
        m_volumeSlider->setEnabled( false );
    }
    if( newState == Tanyatu::IEngine::State::Playing )
    {
        m_seekSlider->setMaxValue(
                    m_engine->currentItemTotalTime() );
        m_playButton->setEnabled( true );
        m_stopButton->setEnabled( true);
        m_playButton->setIcons( QIcon( ":/images/pause_act" ),
                                QIcon( ":/images/pause_act" ),
                                QSize( 32, 32 ));
        m_seekSlider->setEnabled( true );
        m_volumeSlider->setEnabled( true );
        if( m_engine->volume() > 100 ) {
            m_engine->setVolume( 100 );
        }
        m_volumeSlider->setCurrentValue( m_engine->volume() );
    }
    else if( newState == Tanyatu::IEngine::State::Paused )
    {
        m_playButton->setEnabled( true );
        m_playButton->setIcons( QIcon( ":/images/play" ),
                                QIcon( ":/images/play_act" ),
                                QSize( 32, 32 ));
        m_seekSlider->setEnabled( false );
        m_volumeSlider->setEnabled( true );

    }
    else if( newState == Tanyatu::IEngine::State::Stopped )
    {
        m_playButton->setEnabled( true );
        m_stopButton->setEnabled( false );
        m_playButton->setIcons( QIcon( ":/images/play" ),
                                QIcon( ":/images/play_act" ),
                                QSize( 32, 32 ));
        m_seekSlider->setEnabled( false );
        m_volumeSlider->setEnabled( false );
        m_seekSlider->setCurrentValue( 0 );
    }
}

void AudioPlayerWidget::setVolume(qint64 value)
{
    m_engine->setVolume( static_cast< int >( value ));
}

void AudioPlayerWidget::setVolmeSliderValue( int volume )
{
    m_volumeSlider->setCurrentValue( volume );
    m_volumeLable->setText( QString::number( volume ));
}


void AudioPlayerWidget::setVolumeLabelText( qint64 value )
{
    m_volumeLable->setText( QString::number( value ));
}


void AudioPlayerWidget::onPlayButtonClicked()
{
    if( ! m_playButton->isChecked()
            && m_engine->state() == Tanyatu::IEngine::State::Playing ) {
        m_engine->pause();
        m_seekSlider->setEnabled( false );
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
        m_repeatButton->setText( tr( "OFF" ));
        break;

    case Tanyatu::IPlayQueue::RepeatType_RepeatAll:
        PLAYQUEUE()->setRepeat(
                    Tanyatu::IPlayQueue::RepeatType_RepeatOne );
        m_repeatButton->setText( tr( "ALL" ));
        break;

    case Tanyatu::IPlayQueue::RepeatType_RepeatOne:
        PLAYQUEUE()->setRepeat(
                    Tanyatu::IPlayQueue::RepeatType_NoRepeat );
        m_repeatButton->setText( tr( "ONE" ));
        break;
    }
}

void AudioPlayerWidget::updatePlayerActions()
{
    m_nextButton->setEnabled( PLAYQUEUE()->hasNext() );
    m_prevButton->setEnabled( PLAYQUEUE()->hasPrev() );
}


void AudioPlayerWidget::onTrackSelected( Tanyatu::Data::MediaItem * item )
{
    if( item && ( item->type() == Tanyatu::Data::Media_LocalAudio
                  || item->type() == Tanyatu::Data::Media_StoredAudio )) {
        Tanyatu::Data::AudioTrack *track =
                static_cast< Tanyatu::Data::AudioTrack *>( item );
        showTrackDetails( track );
        m_bitrate->setText( QString( "%1kbps" ).arg(
                                track->bitRate() ));
        m_sampleFreq->setText(
                    QString( "%1.%2KHz" )
                    .arg( track->sampleRate() / 1000 )
                    .arg( track->sampleRate() % 1000 / 100 ));
        QFileInfo info( track->url().toLocalFile() );
        m_format->setText( info.suffix().toUpper() );
        m_elapsedTime->setText( "00:00" );
        m_remainingTime->setText(
                    Tanyatu::Utils::getStringTime( track->duration() ));

//        QImage *image = nullptr;//Tanyatu::Utils::imageForTrack( track );
//        image = image != nullptr ? image
//                                 : new QImage( ":/images/defaultcover" );
//        QPixmap pixmap;
//        pixmap.convertFromImage( *image );
//        delete image;
//        QPixmap background = QPixmap( ":/images/background" ).scaled(
//                    this->size() );
//        QPixmap result( background.size() );
//        result.fill( Qt::transparent );
//        QPainter painter( &result );
//        painter.drawPixmap( 0, 0, pixmap.scaled( background.size() ));
//        painter.setOpacity( 0.70 );
//        painter.drawPixmap( 0, 0, background );
//        QPalette pal = this->palette();
//        pal.setBrush( QPalette::Window,
//                      QBrush( result ));
//        this->setPalette( pal );

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
        m_elapsedTime->setText( Tanyatu::Utils::getStringTime(
                                    msecEllapsed / 1000 ));
        m_remainingTime->setText(
                    Tanyatu::Utils::getStringTime(
                        track->duration() - ( msecEllapsed /1000 )));
    }
}


void AudioPlayerWidget::onChilliClicked( bool checked )
{
    if( checked ) {
        m_scroller->setScrollText(
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
    m_scroller->setScrollText( trackString );
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

