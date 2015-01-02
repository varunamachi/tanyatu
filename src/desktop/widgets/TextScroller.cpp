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
    m_timer->setInterval( 400 );
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
