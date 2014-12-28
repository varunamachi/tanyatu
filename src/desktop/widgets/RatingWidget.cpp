/*******************************************************************************
 * RatingWidget.cpp
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

#include <QPainter>
#include <QMouseEvent>
#include <math.h>

#include "RatingWidget.h"


namespace GreenChilli { namespace Widgets {

RatingWidget::RatingWidget( int numValues,
                            const QPixmap *unsetIcon,
                            const QPixmap *setIcon,
                            QWidget *parent,
                            int initValue )
    : QWidget( parent ),
      m_numValues( numValues ),
      m_unsetIcon( unsetIcon ),
      m_setIcon( setIcon )
{
    m_curValue = initValue < m_numValues ? initValue : 0;
    setMouseTracking( true );
}


RatingWidget::~RatingWidget()
{

}


QSize RatingWidget::sizeHint() const
{
    return QSize( m_numValues * m_unsetIcon->width(), m_unsetIcon->height() );
}


int RatingWidget::rating() const
{
    return m_curValue;
}


int RatingWidget::floatingRating() const
{
    return m_flotingValue;
}


void RatingWidget::setRating( int value )
{
    m_curValue = value;
    m_flotingValue = value;
    update();
}


void RatingWidget::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event )
    QPainter pxPaint( this );
    for( int i = 0; i < 5; ++i ) {
        if( i < m_flotingValue ) {
            pxPaint.drawPixmap( i * m_setIcon->width(), 0, *m_setIcon );
        }
        else {
            pxPaint.drawPixmap( i * m_unsetIcon->width(), 0, *m_unsetIcon );
        }
    }
}


void RatingWidget::mouseMoveEvent( QMouseEvent * event )
{
    float xpos = event->x();
    float ypos = event->y();
    float value = static_cast< int >(
                ceil(( xpos - 8 ) / m_setIcon->width() ));
    if( xpos >= sizeHint().width() - 4
            || xpos - 4 <= 0
            || ypos >= sizeHint().height() - 2
            || ypos - 2  <= 0 ) {
        m_flotingValue = value;
        update();
        return;
    }
    else {
        m_flotingValue = value;
        update();
    }
}


void RatingWidget::mouseReleaseEvent( QMouseEvent *event )
{
    Q_UNUSED( event )
    if( m_flotingValue != m_curValue ) {
        m_curValue = m_flotingValue;
        emit ratingSet( m_flotingValue );
    }
}


void RatingWidget::leaveEvent(QEvent *event)
{
    m_flotingValue = m_curValue;
    update();
}

} }
