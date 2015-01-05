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

#include <QWidget>
#include <core/interfaces/IEngine.h>
#include <core/data/AudioTrack.h>

#include "../widgets/SuperSlider.h"
#include "ui_AudioPlayer.h"

namespace GreenChilli
{
class AudioPlayerWidget : public QWidget
{
    Q_OBJECT

public:

    explicit AudioPlayerWidget( QWidget *parent = nullptr );

    static const int AUDIO_PLAYER_WIDTH = 380;

    static const int AUDIO_PLAYER_HEIGHT = 200;

signals:
    void urlsDropped( QList< QUrl > urls );

    void playFromPlaylist();

protected:
    void dragEnterEvent( QDragEnterEvent * event );

    void dropEvent(QDropEvent *event);

private slots:
    void engineStateChanged( Tanyatu::IEngine::State newState,
                             Tanyatu::IEngine::State oldState );

    void setVolume( qint64 value );

    void setVolmeSliderValue( int volume );

    void setVolumeLabelText( qint64 value );

    void onPlayButtonClicked();

    void onRepeatButtonClicked();

    void updatePlayerActions();

    void onTrackSelected( Tanyatu::Data::MediaItem * item );

    void onPlayerTick( qint64 msecEllapsed );

    void onChilliClicked( bool checked );

    void gotoBoringMode()
    {
        setScrollingText();
        m_playerWidget->seekSlider->setEnabled( false );
        m_playerWidget->bitrate->setText( "" );
        m_playerWidget->sampleFreq->setText( "" );
//        QPalette pal = this->palette();
//        pal.setBrush( QPalette::Window, QBrush(
//                          QImage( ":/images/background" )));
//        this->setPalette( pal );
    }

    void setScrollingText( QString text = "G R E E N C H I L L I" )
    {
        m_playerWidget->scroller->setScrollText( text,
                                          text == "G R E E N C H I L L I" );
    }

private:
    void init();

    void showTrackDetails( Tanyatu::Data::AudioTrack *track );

    void setupButtons();

    Tanyatu::IEngine *m_engine;

    Ui::AudioPlayer *m_playerWidget;

};
}
