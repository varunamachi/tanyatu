/*******************************************************************************
 * SuperSlider.cpp
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
#include <core/T.h>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>

#include "SuperSlider.h"


namespace GreenChilli { namespace Widgets {

SuperSlider::SuperSlider( QWidget *parent ) :
    QWidget( parent ),
    m_width( 100 ),
    m_height( 10 ),
    m_drawOffset( 16 ),
    m_halfDrawOffset( m_drawOffset / 2 ),
    m_drawingWidth( m_width + m_drawOffset ),
    m_drawingHeight( m_height + m_drawOffset ),
    m_maxVal( 100 ),
    m_curentVal( 0 ),
    m_brush( QBrush( QColor( 0xFF, 0x66, 0 ))),
    m_backgroundBrush( QBrush( QColor( 0x70, 0x80, 0x90 ))),
    m_enabled( true ),
    m_mouseOver( false )

{
    this->setMouseTracking( true );
    this->setFixedSize( m_drawingWidth, m_drawingHeight );
    this->setContentsMargins( 0, 0, 0, 0 );
}


void SuperSlider::setHeight( int height )
{
    m_height = height;
    m_drawingHeight = height + m_drawOffset;
    this->setFixedSize( m_drawingWidth, m_drawingHeight );
    update();
}


void SuperSlider::setWidth( int width )
{
    m_width = width;
    m_drawingWidth = width + m_drawOffset;
    this->setFixedSize( m_drawingWidth, m_drawingHeight );
    update();
}


void SuperSlider::setBrush( QBrush &brush )
{
    m_brush = brush;
    update();
}


void SuperSlider::setEnabled( bool value )
{
    m_enabled = value;
    QWidget::setEnabled( value );
    update();
}


void SuperSlider::setBackgroundBrush( QBrush &bgBrush )
{
    m_backgroundBrush = bgBrush;
}


void SuperSlider::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event )
    QPainter painter( this );
    QBrush brush = m_enabled ? m_brush : m_backgroundBrush;
    int curPos =  ( float( m_curentVal ) / m_maxVal ) * m_width;
    painter.fillRect( m_halfDrawOffset,
                      m_halfDrawOffset,
                      curPos,
                      m_height,
                      brush );
    painter.fillRect( curPos + m_halfDrawOffset + 1,
                      m_halfDrawOffset,
                      m_width - curPos,
                      m_height,
                      m_backgroundBrush );
    QColor color( brush.color() );
    color.setAlpha( m_mouseOver ? 220 : 150 );
    painter.setPen( Qt::NoPen );
    painter.setBrush( color );
    painter.setRenderHint( QPainter::Antialiasing );
    int radius = ( m_height + m_drawOffset ) / 2;
    int xPos = int( curPos + m_halfDrawOffset );
    xPos = xPos + radius > m_drawingWidth ? m_drawingWidth - radius
                                          : ( xPos - radius < 0 ? radius
                                                                : xPos );
    painter.drawEllipse( QPoint(  xPos,
                                  int( m_drawOffset + m_height ) / 2 ),
                         radius,
                         radius );
    color.setAlpha( 255 );
    painter.drawEllipse(
                QPoint( xPos , int( m_drawOffset + m_height ) / 2 ),
                radius / 2,
                radius / 2);

}


void SuperSlider::mousePressEvent( QMouseEvent *event )
{
    if( m_enabled )
    {
        int curPos = event->pos().x();
        qint64 curValue = curPos * ( m_maxVal / m_width );
        setCurrentValue( curValue );
        emit seeked( curValue );
        m_mouseOver = true;
        m_mousePos = event->pos();
        update();
    }
}


void SuperSlider::mouseMoveEvent( QMouseEvent *event )
{
    if( event->buttons() == Qt::LeftButton ) {
        int xOffset = event->x() - m_mousePos.x();
        m_mousePos = event->pos();
        qint64 curVal = m_curentVal + xOffset * int( m_maxVal / m_width );
        setCurrentValue( curVal );
        emit seeked( m_curentVal );
        update();
    }
}


void SuperSlider::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent( event );
}


void SuperSlider::enterEvent( QEvent *event )
{
    Q_UNUSED( event )
    m_mouseOver = true;
    update();
}


void SuperSlider::leaveEvent( QEvent *event )
{
    Q_UNUSED( event )
    m_mouseOver = false;
    update();
}



QSize SuperSlider::sizeHint() const
{
    return QSize( m_drawingWidth, m_drawingHeight );
}


void SuperSlider::wheelEvent( QWheelEvent *event )
{
    int delta = event->delta() / 60;
    qint64 newVal = m_curentVal + ( m_maxVal / m_width ) * delta;
    if( newVal <= m_maxVal )
    {
        emit seeked( newVal );
        setCurrentValue( newVal );
    }
}


void SuperSlider::setMaxValue( qint64 maxValue )
{
    m_maxVal = maxValue;
    update();
}

void SuperSlider::setCurrentValue( qint64 curValue )
{
    if( curValue <= m_maxVal && m_curentVal != curValue )
    {
        m_curentVal = curValue < 0 ? 0
                                   : ( curValue > m_maxVal ? m_maxVal
                                                           : curValue );
        update();
        emit valueChanged( m_curentVal );
    }
}


} }
