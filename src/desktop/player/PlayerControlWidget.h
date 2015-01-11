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
#pragma once

#include <QWidget>

class QPushButton;
class QLabel;
namespace GreenChilli { namespace Widgets {
   class ImagePushButton;
   class SuperSlider;
   class TextScroller;
} }


namespace  GreenChilli {

class PlayerControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlayerControlWidget( QWidget *parent = 0 );

signals:

public slots:

private:
    GreenChilli::Widgets::TextScroller *m_scroller;

    GreenChilli::Widgets::SuperSlider *m_seekSlider;

    GreenChilli::Widgets::SuperSlider *m_volumeSlider;

    GreenChilli::Widgets::ImagePushButton *m_muteButton;


    GreenChilli::Widgets::ImagePushButton *m_prevButton;

    GreenChilli::Widgets::ImagePushButton *m_stopButton;

    GreenChilli::Widgets::ImagePushButton *m_playButton;

    GreenChilli::Widgets::ImagePushButton *m_nextButton;

    QPushButton *m_shuffleButton;

    QPushButton *m_repeatButton;

    QLabel *m_volumeLable;

    QLabel *m_elapsedTime;

    QLabel *m_remainingTime;

    QLabel *m_bitrate;

    QLabel *m_sampleFreq;

    QLabel *m_format;

};

}
