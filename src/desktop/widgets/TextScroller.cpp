/*******************************************************************************
 * TextScroller.cpp
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


#include <QPixmap>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

#include "TextScroller.h"

namespace GreenChilli { namespace Widgets {

TextScroller::TextScroller( QWidget *parent )
    : QWidget( parent ),
      m_progression( Progress_Forward ),
      m_forceAnimate( false ),
      m_notAnimating( true ),
      m_mouseXPress( -1 ),
      m_pixelFactor( 1.0 )
{
    setParameters( Effect_Bounce, 380, 60, 30 );
    m_fullPixmap = new QPixmap(  );
    m_currentPixmap = new QPixmap(m_displayLength, m_displayHeight);
    m_timer = new QTimer( this );
    m_timer->setInterval( 40 );
    m_textColor = QColor( 255, 168, 88 );
    connect( m_timer, SIGNAL( timeout() ),
             this, SLOT( onTimeOut() ));
    processText( " **** " );
    preparePixmap();
    onTimeOut();
}


void TextScroller::setParameters( ScrollEffect effect,
                                  int displayLength,
                                  int displayHeight,
                                  int xOffset )
{
    m_effect = effect;
    m_displayLength = displayLength;
    m_displayHeight = displayHeight;
    m_xOffset = xOffset;
    m_startIndex = 0;
}


TextScroller::~TextScroller() {
    delete m_fullPixmap;
}


void TextScroller::stop() {
    preparePixmap();
    update();
}


void TextScroller::setScrollText( QString text, bool forceAnimation ) {
    m_forceAnimate = forceAnimation;
    processText( text );
    m_timer->start();
    preparePixmap();
}


void TextScroller::onTimeOut()
{
    preparePixmap();
    update();
}


QSize TextScroller::sizeHint() const{
    return QSize( m_displayLength, m_displayHeight );
}


void TextScroller::paintEvent( QPaintEvent *event )
{
    Q_UNUSED( event )
    QPainter painter( this );
    painter.drawPixmap( 0, 0, *m_currentPixmap );
}


void TextScroller::preparePixmap()
{
//    m_currentPixmap->fill( QColor( 0, 0, 0, 0 ));
    m_currentPixmap->fill( QColor( 0, 0, 0, 60 ));
    QPainter painter( m_currentPixmap );
    if(( m_imageWidth - m_xOffset * 2 ) < m_displayLength && ! m_forceAnimate )
    {
        int padding = ( m_displayLength - m_imageWidth ) / 2;
        QPixmap actualPixmap = m_fullPixmap->copy( 0, 0,
                                               m_imageWidth, m_displayHeight);
        painter.drawPixmap( padding,
                            0,
                            m_imageWidth,
                            m_displayHeight,
                            actualPixmap );
        m_notAnimating = true;
    }
    else if( m_effect == Effect_Revolve )
    {
        if( m_startIndex + m_displayLength > m_imageWidth ) {
            QPixmap first = m_fullPixmap->copy( m_startIndex,
                                            0,
                                            (m_imageWidth - m_startIndex),
                                            m_displayHeight );
            painter.drawPixmap( 0, 0, first );
            QPixmap second = m_fullPixmap->copy(
                        0,
                        0,
                        ( m_displayLength - ( m_imageWidth - m_startIndex )),
                        m_displayHeight);
            painter.drawPixmap(( m_imageWidth - m_startIndex ), 0, second );
        }
        else {
            QPixmap show = m_fullPixmap->copy( m_startIndex,
                                           0,
                                           m_displayLength,
                                           m_displayHeight );
            painter.drawPixmap( 0, 0, show );
        }
        m_startIndex = ( m_startIndex + 1 ) % m_imageWidth;
        m_notAnimating = false;
    }
    else
    {
        if( m_imageWidth <= m_displayLength ) {
            painter.drawPixmap( m_startIndex, 0, *m_fullPixmap );
            if( m_startIndex + m_imageWidth > m_displayLength ) {
                m_progression = Progress_Backward;
            }
            if ( m_startIndex == 0 ){
                m_progression = Progress_Forward;
            }
        }
        else {
            QPixmap show = m_fullPixmap->copy( m_startIndex,
                                           0,
                                           m_displayLength,
                                           m_displayHeight );
            painter.drawPixmap( 0, 0, show );
            if( m_startIndex + m_displayLength > m_imageWidth ) {
                m_progression = Progress_Backward;
            }
            if( m_startIndex == 0 ) {
                m_progression = Progress_Forward;
            }
        }
        m_startIndex = ( m_progression == Progress_Forward ? m_startIndex + 1
                                                           : m_startIndex - 1 );
        m_notAnimating = false;
    }
}


void TextScroller::processText( QString text ) {
    delete m_fullPixmap;
    m_fullPixmap = new QPixmap( text.size() * 16,
                                m_displayHeight );
    m_fullPixmap->fill( qRgba( 0, 0, 0, 0 ));

    ///TODO something else...
//    QPixmap copy = m_fullPixmap->copy( m_fullPixmap->rect() );
//    m_fullPixmap->setAlphaChannel( copy );

    QPainter pxPainter( m_fullPixmap );
    pxPainter.setRenderHint( QPainter::Antialiasing, true );
#ifdef Q_OS_LINUX
    pxPainter.setFont( QFont( QFont().defaultFamily(), 10 ));
#else
    pxPainter.setFont( QFont( "monospace", 10 ));
#endif
    pxPainter.setPen( m_textColor );
    pxPainter.drawText( m_xOffset, 20, text);

    QFontMetrics metric = pxPainter.fontMetrics();
    m_imageWidth = metric.width( text ) + m_xOffset * 2;
    m_pixelFactor = metric.width( text );
    m_progression = Progress_Forward;
}


void TextScroller::mouseMoveEvent( QMouseEvent *event )
{
//    if( ! m_notAnimating && m_mouseXPress != -1 )
//    {
//        m_startIndex +=  - ( event->x() - m_mouseXPress );
//        onTimeOut();
//    }
}

void TextScroller::mousePressEvent(QMouseEvent *event)
{
    m_timer->stop();
    m_mouseXPress = event->x();
}

void TextScroller::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED( event )
    m_timer->start();
    m_mouseXPress = -1;
}

} }
