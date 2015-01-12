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
#include <QResizeEvent>

#include "TextScroller.h"

namespace GreenChilli { namespace Widgets {

TextScroller::TextScroller( QColor bgColor,
                            QColor fgColor,
                            QWidget *parent )
    : QWidget( parent )
    , m_progression( Progression::Forward )
    , m_forceAnimate( false )
    , m_notAnimating( true )
    , m_mouseXPress( -1 )
    , m_pixelFactor( 1.0 )
    , m_bgColor( bgColor )
    , m_fgColor( fgColor )

{
    this->setSizePolicy( QSizePolicy::Expanding,
                         QSizePolicy::Expanding );
    setParameters( ScrollEffect::Bounce, 380, 60, 30 );
    m_fullPixmap = new QPixmap(  );
    m_currentPixmap = new QPixmap( m_displayLength, m_displayHeight );
    m_timer = new QTimer( this );
    m_timer->setInterval( 400 );
//    m_textColor = QColor( 255, 168, 88 );
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
    painter.fillRect( this->rect(), Qt::black );
    painter.drawPixmap( 0, 0, *m_currentPixmap );
}


void TextScroller::preparePixmap()
{
    delete m_currentPixmap;
    m_currentPixmap = new QPixmap( m_displayLength, m_displayHeight );
    m_currentPixmap->fill( m_bgColor );
    QPainter painter( m_currentPixmap );
    if(( m_imageWidth - m_xOffset * 2 ) < m_displayLength && ! m_forceAnimate )
    {
        int padding = ( m_displayLength - m_imageWidth ) / 2;
        painter.drawPixmap( padding,
                            0,
                            m_imageWidth,
                            m_displayHeight,
                            *m_fullPixmap );
        m_notAnimating = true;
    }
    else if( m_effect == ScrollEffect::Revolve )
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
                m_progression = Progression::Backward;
            }
            if ( m_startIndex <= 0 ){
                m_progression = Progression::Forward;
            }
        }
        else {
            painter.drawPixmap( m_startIndex, 0, *m_fullPixmap );
            if( m_startIndex + m_imageWidth >  m_displayLength * 1.5 ) {
                m_progression = Progression::Backward;
            }
            if( m_startIndex <= - m_imageWidth / 2 ) {
                m_progression = Progression::Forward;
            }
        }
        m_startIndex = ( m_progression == Progression::Forward
                                        ? m_startIndex + 1
                                        : m_startIndex - 1 );
        m_notAnimating = false;
    }
}


void TextScroller::processText( QString text ) {
    QFont font( QFont().defaultFamily(), 8 );
#ifdef Q_OS_WIN32
    font = QFont( "monospace", 8 );
#endif

    QPixmap dummy( 100, 100 );
    QPainter dummyPainter( &dummy );
    dummyPainter.setFont( font );
    QFontMetrics metric = dummyPainter.fontMetrics();
    m_imageWidth = metric.width( text ) + m_xOffset * 2;
    m_pixelFactor = metric.width( text );

    delete m_fullPixmap;

    m_fullPixmap = new QPixmap( m_imageWidth,
                                m_displayHeight );
    m_fullPixmap->fill( m_bgColor );
    QPainter pxPainter( m_fullPixmap );
    pxPainter.setFont( font );
    pxPainter.setRenderHint( QPainter::Antialiasing, true );
    pxPainter.setRenderHint(QPainter::HighQualityAntialiasing);
    pxPainter.setPen( m_fgColor );
    pxPainter.drawText( m_xOffset, metric.height(), text);


    m_progression = Progression::Forward;
    update();
}


void TextScroller::resizeEvent( QResizeEvent *event )
{
    m_displayLength = event->size().width();
    preparePixmap();
}

} }
