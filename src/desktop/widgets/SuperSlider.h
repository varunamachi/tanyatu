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
#include <QSize>
#include <QBrush>


namespace GreenChilli { namespace Widgets {

class SuperSlider : public QWidget
{
    Q_OBJECT
public:
    explicit SuperSlider( QWidget *parent = nullptr );

    virtual ~SuperSlider() { /*Nothing to do here*/ }

    QSize sizeHint() const;

public slots:
    void setMaxValue( qint64 maxValue );

    void setCurrentValue( qint64 curValue );

    void setWidth( int width );

    void setHeight( int height );

    void setBrush( QBrush &brush );

    void setBackgroundBrush( QBrush &bgBrush );

    void setEnabled( bool value );

signals:
    void valueChanged( qint64 value );

    void seeked( qint64 value );

protected:
    void paintEvent( QPaintEvent *event );

    void mousePressEvent( QMouseEvent *event );

    void mouseMoveEvent( QMouseEvent *event );

    void mouseReleaseEvent( QMouseEvent *event );

    void wheelEvent( QWheelEvent *event );

    void enterEvent( QEvent *event );

    void leaveEvent( QEvent *event );

private:
    quint32 m_width;

    quint32 m_height;

    quint8 m_drawOffset;

    quint8 m_halfDrawOffset;

    quint32 m_drawingWidth;

    quint32 m_drawingHeight;

    quint64 m_maxVal;

    quint64 m_curentVal;

    QBrush m_brush;

    QBrush m_backgroundBrush;

    QPoint m_mousePos;

    bool m_enabled;

    bool m_mouseOver;
};

} }
