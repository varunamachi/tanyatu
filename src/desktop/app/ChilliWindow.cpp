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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QSizeGrip>
#include <QMargins>
#include <QTime>
#include <QThread>
#include <QPaintEvent>
#include <QMouseEvent>


#include <core/T.h>
#include <uicommon/uiutils/ChilliCache.h>

#include "ChilliWindow.h"
#include "ComponentManager.h"
#include "player/AudioPlayerWidget.h"
#include "playqueueview/PlayQueueView.h"
#include "thirdparty/CustomShadowEffect.h"


namespace GreenChilli {

static void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    QThread::currentThread()->sleep( 100 );
}



ChilliMainWidget::ChilliMainWidget( QWidget *parent )
    : QWidget( parent )
    , m_roundedRect( true )
{

    this->setObjectName( "chillimain" );
//    DATA_RETRIEVER()->getSavedPlayQueue( );
//    PLAYQUEUE()->addItems( list );
    QVBoxLayout *playerLayout = new QVBoxLayout();
    m_audioPlayer = new AudioPlayerWidget( this );
    m_playlist = new PlaylistWidget( this );
    m_playlist->setContentsMargins( 2, 0, 0, 0 );
    playerLayout->addWidget( m_audioPlayer );
    playerLayout->addWidget( m_playlist );

    QHBoxLayout *compLayout = new QHBoxLayout();
    compLayout->addWidget( ComponentManager::get() );

    compLayout->setContentsMargins( QMargins() );
    compLayout->setSpacing( 0 );


    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addLayout( compLayout );

    leftLayout->setContentsMargins( QMargins() );
    leftLayout->setSpacing( 0 );
    m_leftWidget = new QWidget( this );
    m_leftWidget->setLayout( leftLayout );
    m_leftWidget->setContentsMargins( QMargins() );

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout( playerLayout );
    mainLayout->addWidget( m_leftWidget );
    mainLayout->setContentsMargins( QMargins() );
    mainLayout->setSpacing( 0 );
    QString css = createStyleSheet();
    ComponentManager::get()->setStyleSheet( css );
    m_playlist->setStyleSheet( css );
    playerLayout->setContentsMargins( QMargins() );
    playerLayout->setSpacing( 0 );

    connect( m_audioPlayer, SIGNAL( urlsDropped( QList< QUrl > )),
             m_playlist, SLOT( addUrls( QList< QUrl > )));
    connect( m_audioPlayer, SIGNAL( playFromPlaylist() ),
             m_playlist, SLOT( setSelectedAsCurrent() ));
    connect( QCoreApplication::instance(),
             SIGNAL( aboutToQuit() ),
             this,
             SLOT( onAboutToQuit() ));
    this->setLayout( mainLayout );


}



void ChilliMainWidget::onAboutToQuit()
{
    while( JOB_MANAGER()->hasPendingJobs() ) {
        QThread::currentThread()->sleep( 1 );
    }
    const QList< Tanyatu::Data::MediaItem *> *list =
            PLAYQUEUE()->getAllItemsInOrder();
    DATA_UPDATER()->saveCurrentPlaylist( *list );
    ComponentManager::destroy();
    CHILLI_CACHE()->destroy();
}


void ChilliMainWidget::paintEvent( QPaintEvent *event )
{
    QPainter painter( this );
    if( m_roundedRect ) {
        QPainterPath path;
        path.addRoundedRect( this->rect(), 10, 10 );
        QBrush brush( Qt::black );
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.fillPath( path, brush );
    }
    else {
        painter.fillRect( this->rect(), QBrush( Qt::black ));
    }
}



QString ChilliMainWidget::createStyleSheet()
{
    return "QWidget{"
            "background-color: black; "
            "color: #FFA858;"
            "border-color: black;"
        "}"
        "QPushButton{"
            "background-color: #202020; "
            "color: #FFA858;"
            "border-width: 8px;"
            "border-color: black;"
        "}"
        "QToolButton{"
            "background-color: #323232; "
            "color: #FFA858;"
            "border-style: outset;"
            "border-color: black;"
            "border-radius: 5px;"
            "min-width: 40px;"
            "min-height: 20px;"
            "font-size: 10px;"
        "}"
         "QToolButton:pressed{"
           "background-color: #FFA858; "
           "color: #323232;"
           "border-style: outset;"
           "border-color: black;"
           "border-radius: 5px;"
           "min-width: 40px;"
           "min-height: 20px;"
        "}"
        "QToolButton:hover{"
             "color: red;"
             "border-style: outset;"
             "border-color: black;"
             "border-radius: 5px;"
             "min-width: 40px;"
             "min-height: 20px;"
        "}"
        "QToolBar{"
            "spacing: 3px;"
        "}"
        "QPushButton:clicked{"
            "background-color: #FFA858; "
            "color: white;"
            "border-width: 8px;"
            "border-color: black;"
        "}"
        "QTreeView{ "
            "background-color: #151515;"
            "alternate-background-color: #202020;"
        "}"
        "QTreeView::item:selected:active, QTreeView::item:selected:!active,"
        "QListView::item:selected:active, QListView::item:selected:!active{"
            "color: #151515; "
            "background-color: rgba( 255, 168, 48, 200 );"
        "}"
        "QHeaderView::section {"
            "background-color: #202020;"
            "color: white;"
        "}"
        "QTreeView::item:hover, QListView::item:hover { "
            "background-color: rgba( 255, 168, 48, 130 );"
        "}"
        "QProgressBar{ "
            "border-radius: 5px;"
            "color: white;"
            "text-align: center;"
        "}"
        "QProgressBar::chunk {"
            "background-color: #FFA858;"
        "}"
        "QLineEdit{ background-color: #444444;}"
        "QMenu{ background-color: #444444;}"
        "QMenu::item:selected{background-color: #696969; }";
}


ChilliWindow::ChilliWindow( QWidget *parent )
    : QMainWindow( parent )
    , m_chilliWidget( new ChilliMainWidget( this ))
    , m_maximised( false )
{
    this->setWindowFlags( Qt::FramelessWindowHint
                          | Qt::WindowMinimizeButtonHint );
//    m_sizeGrip = new QSizeGrip( this );
//    m_sizeGrip->setFixedSize( 6, 6 );
//    m_sizeGrip->setGeometry( m_chilliWidget->width() - 6, m_chilliWidget->height() - 6, 6, 6 );
//    m_sizeGrip->setStyleSheet( "background-color: yellow;");
//    m_sizeGrip->setToolTip( tr( "Drag to resize the window" ));
//    m_sizeGrip->setContentsMargins( QMargins() );
    m_containerWidget = new QWidget( this );
    m_containerWidget->setContentsMargins( 5, 5, 5, 5 );


    m_chilliWidget->setContentsMargins( 5, 5, 5, 5 );
    m_layout = new QHBoxLayout( m_containerWidget );
    m_layout->addWidget( m_chilliWidget );
    m_containerWidget->setAttribute( Qt::WA_TranslucentBackground, true );
    this->setAttribute( Qt::WA_TranslucentBackground, true );
    this->setCentralWidget( m_containerWidget );
    m_layout->setSpacing( 5 );
    m_layout->setContentsMargins( 5, 5, 5, 5 );

    CustomShadowEffect *effect = new CustomShadowEffect( this );
    effect->setBlurRadius( 10.0 );
    effect->setDistance( 3.0 );
    effect->setColor( QColor( 0xA0, 0x52, 0x2D, 0x80 ));
    m_chilliWidget->setGraphicsEffect( effect );
    connect( ComponentManager::get(),
             SIGNAL( exitRequested() ),
             QApplication::instance(),
             SLOT( quit() ));
    connect( ComponentManager::get(),
             SIGNAL( minimizeReqested()) ,
             this,
             SLOT( onMinimize() ));
    connect( m_chilliWidget,
             SIGNAL( minimize() ),
             this,
             SLOT( onMinimize() ));
}


void ChilliWindow::onMinimize()
{
    minimize();
}


void ChilliWindow::onMaximizeRestore()
{
    if( m_maximised ) {
        m_maximised = false;
        restore();
    }
    else {
        m_geometry = saveGeometry();
        m_maximised = true;
        maximize();
    }
}


void ChilliWindow::mousePressEvent( QMouseEvent* event )
{
    if( event->button() == Qt::LeftButton ) {
        m_moving = true;
        m_lastMousePosition = event->globalPos();
    }
}

void ChilliWindow::mouseMoveEvent( QMouseEvent* event )
{
    if( event->buttons().testFlag( Qt::LeftButton ) && m_moving ) {
        QPoint newPos = this->pos() + (
                    event->globalPos() - m_lastMousePosition );
        if( newPos.y() <= 0 ) {
            m_maximised = true;
            emit maximize();
        }
        else if( m_maximised && newPos.y() >= 10 ) {
            m_maximised = false;
            restore();
            move( event->globalX() - this->width() / 2, event->globalY() );
        }
        else {
            move( newPos );
        }
        m_lastMousePosition = event->globalPos();
    }
}


void ChilliWindow::mouseReleaseEvent( QMouseEvent* event )
{
    if( event->button() == Qt::LeftButton )
    {
        m_moving = false;
    }
}


void ChilliWindow::showEvent( QShowEvent *evt )
{
    if( m_geometry.isEmpty() ) {
        m_geometry = this->saveGeometry();
    }
    QWidget::showEvent( evt );
}


void ChilliWindow::resizeEvent( QResizeEvent *evt )
{
//    Q_UNUSED( evt )
//    m_sizeGrip->setGeometry( this->width() - 8,
//                             this->height() - 8,
//                             6,
//                             6 );
}



void ChilliWindow::maximize()
{
    QDesktopWidget *desktop = QApplication::desktop();
    // Because reserved space can be on all sides of the scren
    // you have to both move and resize the window
    m_layout->setSpacing( 0 );
    m_layout->setContentsMargins( QMargins() );
    m_containerWidget->setContentsMargins( QMargins() );
    m_chilliWidget->setRoundedRect( false );
    this->setGeometry( desktop->availableGeometry() );
}


void ChilliWindow::restore()
{
    if( ! m_geometry.isEmpty() ) {
        m_layout->setSpacing( 5 );
        m_layout->setContentsMargins( 5, 5, 5, 5 );
        m_containerWidget->setContentsMargins( 5, 5, 5, 5 );
        m_chilliWidget->setRoundedRect( true );
        this->restoreGeometry( m_geometry );
    }
}


void ChilliWindow::minimize()
{
    if( ! this->isMinimized() ) {
        this->showMinimized();
    }
}

}
