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

namespace GreenChilli { namespace Widgets {

class TextScroller : public QWidget
{
    Q_OBJECT
public:
    enum ScrollEffect
    {
        Effect_Revolve,
        Effect_Bounce
    };

    TextScroller( QWidget *parent = 0 );

    void setParameters( ScrollEffect effect,
                  int displayLength,
                  int displayHeight,
                  int xOffset );

    ~TextScroller();

    QSize sizeHint() const;

public slots:
    void setScrollText( QString string, bool forceAnimation = false );

    void stop();

protected:
    void paintEvent( QPaintEvent *event );

    void mouseMoveEvent( QMouseEvent *event );

    void mousePressEvent( QMouseEvent *event );

    void mouseReleaseEvent( QMouseEvent *event );

private slots:
    void preparePixmap();

    void processText( QString text );

    void onTimeOut();

private:
    enum Progression {
        Progress_Forward,
        Progress_Backward
    };

    ScrollEffect m_effect;

    int m_displayLength;

    int m_displayHeight;

    int m_xOffset;

    bool m_forceAnimate;

    bool m_notAnimating;

    int m_mouseXPress;

    qreal m_pixelFactor;

    QColor m_textColor;

    Progression m_progression;

    QPixmap *m_fullPixmap;

    QPixmap *m_currentPixmap;

    QTimer* m_timer;

    int m_startIndex;

    int m_imageWidth;
};

} }


