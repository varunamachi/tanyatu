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

#include <QPushButton>
#include <QStyleOption>
#include <QPainter>


namespace GreenChilli { namespace Widgets {

class ImagePushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ImagePushButton( QWidget *parent = 0 )
        : QPushButton( parent )
        , m_initialized( false )
    {
        init();
    }

    ImagePushButton( QIcon &defaultIcon,
                     QIcon &pressedIcon,
                     QWidget *parent = 0 )
        : QPushButton( parent )
        , m_default( defaultIcon )
        , m_pressed( pressedIcon )
        , m_initialized( true )
    {
        setIcon( m_default );
        init();
    }


public slots:
    void setIcons( QIcon defaultIcon, QIcon pressedIcon, QSize size )
    {
        m_default = defaultIcon;
        m_pressed = pressedIcon;
        m_initialized = true;
        if( this->isCheckable() ) {
            this->setIcon( this->isChecked() ? m_pressed : m_default );
        }
        else {
            this->setIcon( m_default );
        }
        this->setIconSize( size );
        this->resize( size );
    }


protected:
    void mousePressEvent( QMouseEvent *event )
    {
        this->setIcon( m_pressed );
        QPushButton::mousePressEvent( event );
    }

    void mouseReleaseEvent( QMouseEvent *event )
    {
        if( ! isCheckable() ) {
            this->setIcon( m_default );
        }
        QPushButton::mouseReleaseEvent( event );
    }

    void paintEvent(QPaintEvent *event )
    {
        Q_UNUSED( event )
        if( ! m_initialized ) {
            return;
        }
        QPainter painter(this);
        QPixmap pixmap = this->icon().pixmap( this->iconSize(),
                                              isEnabled() ? QIcon::Normal :
                                                            QIcon::Disabled );
        painter.drawPixmap( 0,
                            0,
                            this->iconSize().width(),
                            this->iconSize().height(),
                            pixmap );
    }


    void enterEvent( QEvent *event )
    {
        Q_UNUSED( event )
        m_mouseOver = true;
    }


    void leaveEvent( QEvent *event )
    {
        Q_UNUSED( event )
        m_mouseOver = false;
    }

private slots:
    void onCheckStateChange( bool newState )
    {
        this->setIcon( newState ? m_pressed : m_default );
    }

private:

    void init()
    {
        this->setStyleSheet( "background-color: rgba(255, 255, 255, 0);" );
        connect( this,
                 SIGNAL( toggled(bool) ),
                 this,
                 SLOT( onCheckStateChange( bool )));
    }

    QIcon m_default;

    QIcon m_pressed;

    bool m_initialized;

    bool m_mouseOver;

};

} }
