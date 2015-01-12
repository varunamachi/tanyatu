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

#include <QString>
#include <QFileInfo>
#include <QLabel>
#include <QDropEvent>
#include <QMimeData>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <core/T.h>
#include <core/data/AudioTrack.h>
#include <core/coreutils/Utils.h>

#include "AudioPlayerWidget.h"


namespace GreenChilli {

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
    m_scroller      = new GreenChilli::Widgets::TextScroller(
                                QColor( 0x10, 0x10, 0x10 ),
                                QColor( 255, 168, 88 ),
                                this );
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
    m_totalTime = new QLabel( "--:--" );
    m_bitrate       = new QLabel( "    " );
    m_sampleFreq    = new QLabel( "    " );
    m_format        = new QLabel( "    " );

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

    auto lytPlayCtrl = new QHBoxLayout();
    lytPlayCtrl->addWidget( m_prevButton );
    lytPlayCtrl->addWidget( m_stopButton );
    lytPlayCtrl->addWidget( m_playButton );
    lytPlayCtrl->addWidget( m_nextButton );

    auto lytVolume = new QHBoxLayout();
    lytVolume->addWidget( m_muteButton );
    lytVolume->addWidget( m_volumeSlider );
    lytVolume->addWidget( m_volumeLable );
    lytVolume->setSpacing( 2 );
//    volumeSlider->setHeight( 19 );
    m_muteButton->setCheckable( true );
    auto volWidget = new QWidget( this );
    volWidget->setLayout( lytVolume );
    volWidget->setMaximumWidth( 140 );

    auto lytSeeker = new QHBoxLayout();
    lytSeeker->addWidget( m_seekSlider );
    lytSeeker->addWidget( m_elapsedTime );
    lytSeeker->addWidget( new QLabel( " | " ));
    lytSeeker->addWidget( m_totalTime );
    m_seekSlider->setWidth( 200 );
    m_seekSlider->setHeight( 20 );


    auto lytInfo = new QVBoxLayout();
    lytInfo->addWidget( m_bitrate );
    lytInfo->addWidget( m_sampleFreq );
    lytInfo->addWidget( m_format );
    lytInfo->addStretch();
    lytInfo->setSpacing( 1 );
    lytInfo->setContentsMargins( QMargins() );

    auto lytScrollerSeeker = new QHBoxLayout();
    lytScrollerSeeker->addWidget( m_scroller );
    lytScrollerSeeker->addLayout( lytInfo );
    lytScrollerSeeker->setSpacing( 0 );
    m_scroller->setScrollText( " G R E E N C H I L L I ", true );
    m_scroller->setParameters( Widgets::TextScroller::ScrollEffect::Bounce,
                               100,
                               34,
                               0 );
    lytScrollerSeeker->setContentsMargins( QMargins() );


    auto lytMiddle = new QVBoxLayout();
    lytMiddle->addLayout( lytScrollerSeeker );
    lytMiddle->addLayout( lytSeeker );
    lytMiddle->setContentsMargins( QMargins( 5, 5, 2, 5 ));
//    lytMiddle->setSpacing( 0 );

    QWidget *middle = new QWidget( this );
    middle->setLayout( lytMiddle );
    middle->setStyleSheet( "border-radius: 5px;"
                           "background-color: #101010;" );
//    middle->setContentsMargins( QMargins() );



    QPushButton *closeButton = new QPushButton( "X", this );
    closeButton->setFlat( true );
    closeButton->setToolTip( tr( "Exit" ));
    closeButton->setContentsMargins( QMargins() );

    QPushButton *minimizeButton = new QPushButton( "--", this );
    minimizeButton->setFlat( true );
    minimizeButton->setToolTip( tr( "Minimize" ));
    minimizeButton->setContentsMargins( QMargins() );

    QString winButtonCss = " QPushButton {"
                           "     border-radius: 5px;"
                           "     max-width: 32px;"
                           "     max-height:32px;"
                           "     min-width: 32px;"
                           "     min-height: 32px;"
                           "     font-size: 14px;"
                           "     background-color: black;"
                           " }"
                           " QPushButton:checked {"
                           "     background-color: #FFA858;"
                           "     color: #202020;"
                           " }"
                           " QPushButton:hover{"
                           "     background-color: #FFA858;"
                           "     color: red;"
                           " }";
    minimizeButton->setStyleSheet( winButtonCss );
    closeButton->setStyleSheet( winButtonCss );
    connect( minimizeButton,
             SIGNAL( clicked() ),
             this,
             SIGNAL( minimize() ));
    connect( closeButton,
             SIGNAL( clicked() ),
             this,
             SIGNAL( exit() ));


    auto lytWin = new QHBoxLayout();
    lytWin->addWidget( minimizeButton );
    lytWin->addWidget( closeButton );

    auto lytRepShfl = new QHBoxLayout();
    lytRepShfl->addWidget( m_repeatButton );
    lytRepShfl->addWidget( m_shuffleButton );
    m_shuffleButton->setCheckable( true );
    m_repeatButton->setCheckable( true );
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

    auto lytRight = new QVBoxLayout();
    lytRight->addLayout( lytWin );
    lytRight->addLayout( lytRepShfl );

    auto lytMaster = new QHBoxLayout();
    lytMaster->addLayout( lytPlayCtrl );
    lytMaster->addWidget( volWidget );
    lytMaster->addWidget( middle );
    lytMaster->addLayout( lytRight );
    this->setLayout( lytMaster );
    lytMaster->setContentsMargins( QMargins( 0, 0, 0, 4 ));

//    QPalette pal = this->palette();
//    this->setAutoFillBackground( true );
//    pal.setBrush( QPalette::Window, QBrush( QImage( ":/images/background" )));
//    pal.setBrush( QPalette::Window, Qt::black );
//    pal.setBrush( QPalette::Window, Qt::black );
//    this->setPalette( pal );
    this->setAcceptDrops( true );
    setScrollingText();
    this->setFixedHeight( 70 );

    QString labelCss =
            "QLabel {"
            "   color: gray;"
            "   font-size: 7px;"
            "   font: monospace;"
            "}";
    m_sampleFreq->setStyleSheet( labelCss );
    m_bitrate->setStyleSheet( labelCss );
    m_format->setStyleSheet( labelCss );
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
        m_repeatButton->setChecked( false );
        break;

    case Tanyatu::IPlayQueue::RepeatType_RepeatAll:
        PLAYQUEUE()->setRepeat(
                    Tanyatu::IPlayQueue::RepeatType_RepeatOne );
        m_repeatButton->setText( tr( "ALL" ));
        m_repeatButton->setChecked( true );
        break;

    case Tanyatu::IPlayQueue::RepeatType_RepeatOne:
        PLAYQUEUE()->setRepeat(
                    Tanyatu::IPlayQueue::RepeatType_NoRepeat );
        m_repeatButton->setText( tr( "ONE" ));
        m_repeatButton->setChecked( true );
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
        m_bitrate->setText( QString( "%1K" ).arg(
                                track->bitRate() ));
        m_sampleFreq->setText(
                    QString( "%1.%2K" )
                    .arg( track->sampleRate() / 1000 )
                    .arg( track->sampleRate() % 1000 / 100 ));
        QFileInfo info( track->url().toLocalFile() );
        m_format->setText( info.suffix().toUpper() );
        m_elapsedTime->setText( "00:00" );
        m_totalTime->setText(
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
        m_elapsedTime->setText( Tanyatu::Utils::getStringTime(
                                    msecEllapsed / 1000 ));
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

}
