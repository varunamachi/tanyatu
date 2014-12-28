/*******************************************************************************
 * AudioPlayerWidget.h
 *
 * Copyright (c) 2012, Varuna L Amachi. All rights reserved.
 *
 * This program/library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program/library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
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
        QPalette pal = this->palette();
        pal.setBrush( QPalette::Window, QBrush(
                          QImage( ":/images/background" )));
        this->setPalette( pal );
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
