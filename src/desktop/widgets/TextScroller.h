/*******************************************************************************
 * TextScroller.h
 *
 * Copyright (c) 2013, Varuna L Amachi. All rights reserved.
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


