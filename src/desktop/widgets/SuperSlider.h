/*******************************************************************************
 * SuperSlider.h
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
