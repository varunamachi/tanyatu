/*******************************************************************************
 * Copyright (c) 2015, Varuna L Amachi. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMargins>

#include "../widgets/ImagePushButton.h"
#include "../widgets/SuperSlider.h"
#include "../widgets/TextScroller.h"

#include "PlayerControlWidget.h"

#define NO_MARGINS( layout ) \
    layout->setContentsMargins( QMargins() ); \



namespace GreenChilli {

PlayerControlWidget::PlayerControlWidget( QWidget *parent )
    : QWidget( parent )
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

    this->setFixedHeight( 48 );
}

}
