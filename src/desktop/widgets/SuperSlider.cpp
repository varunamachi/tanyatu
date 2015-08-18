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
#include <core/T.h>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>
#include <QResizeEvent>

#include "SuperSlider.h"


namespace GreenChilli { namespace Widgets {

SuperSlider::SuperSlider( QWidget *parent ) :
    QWidget( parent ),
//  m_width( 100 ),
    m_height( 20 ),
    m_drawOffset( 16 ),
    m_halfDrawOffset( m_drawOffset / 2 ),
    m_drawingWidth( this->width() - m_drawOffset ),
    m_drawingHeight( m_height - m_drawOffset ),
    m_maxVal( 100 ),
    m_curentVal( 0 ),
    m_brush( QBrush( QColor( 0xFF, 0x66, 0 ))),
    m_backgroundBrush( QBrush( QColor( 0x70, 0x80, 0x90 ))),
    m_enabled( true ),
    m_mouseOver( false )

{
    this->setMouseTracking( true );
    this->setFixedHeight( m_height );
    this->setContentsMargins( 0, 0, 0, 0 );
    this->setSizePolicy( QSizePolicy::Expanding,
                         QSizePolicy::Expanding );
}


void SuperSlider::setHeight( int height )
{
    m_height = height;
    m_drawingHeight = height - m_drawOffset;
    this->setFixedHeight( m_height );
    update();
}


void SuperSlider::setWidth( int width )
{
//    this->width() = width;

    m_drawingWidth = width - m_drawOffset;
    this->setMinimumWidth( width );
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
    int curPos =  m_curentVal != m_maxVal
            ? ( float( m_curentVal ) / m_maxVal ) * m_drawingWidth
            : m_drawingWidth;
    painter.fillRect( m_halfDrawOffset,
                      m_halfDrawOffset,
                      curPos,
                      m_drawingHeight,
                      brush );
    painter.fillRect( curPos + m_halfDrawOffset + 1,
                      m_halfDrawOffset,
                      m_drawingWidth - curPos,
                      m_drawingHeight,
                      m_backgroundBrush );
    QColor color( brush.color() );
    color.setAlpha( m_mouseOver ? 220 : 150 );
    painter.setPen( Qt::NoPen );
    painter.setBrush( color );
    painter.setRenderHint( QPainter::Antialiasing );
    int radius = m_halfDrawOffset;
    int xPos = int( curPos + m_halfDrawOffset );
    painter.drawEllipse( QPoint(  xPos,
                                  int( m_drawOffset + m_drawingHeight ) / 2 ),
                         radius,
                         radius );
    color.setAlpha( 255 );
    painter.drawEllipse(
                QPoint( xPos , int( m_drawOffset + m_drawingHeight) / 2 ),
                radius / 2,
                radius / 2);

}


void SuperSlider::mousePressEvent( QMouseEvent *event )
{
    if( m_enabled )
    {
        int curPos = event->pos().x();
        qint64 curValue = curPos * ( m_maxVal / m_drawingWidth );
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
        qreal addn = ( xOffset * m_maxVal ) / m_drawingWidth;
        int incr = m_maxVal < m_drawingWidth
                ? ( addn > 0.5 ? 1 : addn < -0.5 ? -1 : 0 )
                : addn;
        qint64 curVal = m_curentVal + incr;
        setCurrentValue( curVal );
        emit seeked( m_curentVal );
        update();
    }
}


void SuperSlider::wheelEvent( QWheelEvent *event )
{
    qreal addn = ( m_maxVal * event->delta()  ) / ( m_drawingWidth * 60 );
    int incr = m_maxVal < m_drawingWidth
            ? ( addn > 0.5 ? 1 : addn < -0.5 ? -1 : 0 )
            : addn;
    qint64 newVal = m_curentVal + incr;
    if( newVal <= m_maxVal ) {
        emit seeked( newVal );
        setCurrentValue( newVal );
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


void SuperSlider::resizeEvent( QResizeEvent *event )
{
//    setWidth( event->size().width() );
    QWidget::resizeEvent( event );
    m_drawingWidth = this->width() - m_drawOffset;
    update();
}



QSize SuperSlider::sizeHint() const
{
    return QSize( 100, m_height );
}




void SuperSlider::setMaxValue( qint64 maxValue )
{
    m_maxVal = maxValue;
    update();
}

void SuperSlider::setCurrentValue( qint64 curValue )
{
    if( curValue <= m_maxVal && m_curentVal != (quint64 ) curValue ) {
        m_curentVal = curValue < 0 ? 0
                                   : ( curValue > m_maxVal ? m_maxVal
                                                           : curValue );
        update();
        emit valueChanged( m_curentVal );
    }
}


} }
