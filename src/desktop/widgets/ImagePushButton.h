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
