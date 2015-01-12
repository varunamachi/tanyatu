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
    return QSize( m_numValues * m_setIcon->width(), m_setIcon->height() );
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
            pxPaint.drawPixmap( i * m_setIcon->width(), 0, *m_unsetIcon );
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
