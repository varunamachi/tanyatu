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

#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include <QDesktopServices>
#include <QAction>
#include <QToolButton>
#include <QMenu>
#include <QPushButton>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QUrl>
#include <QPalette>
#include <QProgressBar>
#include <QHeaderView>
#include <QScrollBar>
#include <QMimeData>

#include <core/data/MediaItem.h>


#include "PlayQueueView.h"
#include "../player/AudioPlayerWidget.h"
#include "../desktoputils/ChilliUtils.h"

namespace GreenChilli {

PlaylistWidget::PlaylistWidget( QWidget *parent )
    : QWidget( parent ),
      m_view( new QTreeView( this )),
      m_toolBar( new QToolBar( this )),
      m_loader( new Tanyatu::Worker::AudioLoader( this )),
      m_progress( new QProgressBar( this )),
      m_status( new QLabel( this ))
{
    m_playlist = PLAYQUEUE();
    setupUi();
    setupToolbar();
    setupConnections();
    setAcceptDrops( true );
    this->setFixedWidth( AudioPlayerWidget::AUDIO_PLAYER_WIDTH );
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}



void PlaylistWidget::setupUi() {


//    this->setAutoFillBackground( true );
//    m_toolBar->setAutoFillBackground( true );
    m_view->setWordWrap( false );
    m_view->setAlternatingRowColors( true );
    m_view->setAutoScroll( true );
    m_view->setSortingEnabled( true );
    m_view->setRootIsDecorated( false );
    m_view->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_view->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_view->setFocusPolicy( Qt::WheelFocus );
    m_view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

    m_model = new PlayQueueTrackModel( this );
    m_view->setModel( m_model );
    m_view->setFont( QFont( m_view->font().defaultFamily(), 9 ));
    m_view->header()->setSectionResizeMode( 1, QHeaderView::Stretch );
    m_view->setColumnWidth( 2, 16 );
    m_view->header()->setStretchLastSection( false );
    m_view->hideColumn( 0 );
    m_view->header()->setVisible( false );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( m_view );
    mainLayout->addWidget( m_toolBar, 0, Qt::AlignCenter );

    QHBoxLayout* statusLayot  = new QHBoxLayout();
    statusLayot->addWidget( m_status );
    statusLayot->addWidget( m_progress );
    mainLayout->addLayout( statusLayot );
    m_progress->hide();
    clearStatus();
    setLayout( mainLayout );

    this->setContentsMargins( 0, 0, 0, 0);
    m_view->setContentsMargins( 0, 0, 0, 0);
    mainLayout->setContentsMargins( 0, 0, 0, 0 );
}


void PlaylistWidget::setupConnections()
{
    connect( m_playlist,
             SIGNAL( itemSelected( Tanyatu::Data::MediaItem *,int )),
             this,
             SLOT( indexChanged( Tanyatu::Data::MediaItem *, int )));
    connect( m_playlist,
             SIGNAL( playQueueChanged() ),
             this,
             SLOT( onPlaylistChanged() ));
    connect( m_view,
             &QTreeView::clicked,
             [ = ]()
    {
        m_removeAction->setEnabled(
                    m_view->selectionModel()->hasSelection() );
    });
    connect( m_loader,
             SIGNAL( loadProgress( int, int, QString )),
             this,
             SLOT( showLoadProgress( int, int )));
    connect( m_loader,
             SIGNAL( loadingFinished( QList<QUrl> *,
                                      QList< Tanyatu::Data::AudioTrack *> * )),
             this,
             SLOT( onLoadingFinished( QList< QUrl >*,
                                      QList< Tanyatu::Data::AudioTrack *> * )));
    connect( m_loader,
             SIGNAL( loadingStarted( int )),
             this,
             SLOT( onLoadingStarted( int ) ));
    connect( m_loader,
             SIGNAL( loadedItem( Tanyatu::Data::AudioTrack* )),
             this,
             SLOT( onAudioTrackLoaded( Tanyatu::Data::AudioTrack* )));
    connect( m_view,
             SIGNAL( activated( QModelIndex )),
             this,
             SLOT( rowSelected( QModelIndex )));

//#ifdef Q_OS_WIN32
//    //On Linux "activated" signal handles the double click
//    connect( m_view,
//             SIGNAL( doubleClicked( QModelIndex )),
//             this,
//             SLOT( rowSelected( QModelIndex )));
//#endif
    connect( AUDIO_ENGINE(),
             SIGNAL( finished( Tanyatu::Data::MediaItem* )),
             m_playlist,
             SLOT( selectNext() ));
}


void PlaylistWidget::indexChanged( Tanyatu::Data::MediaItem *track, int index )
{
    Q_UNUSED( track );
    Tanyatu::T::get()->audioEngine()->setSource( track );
    m_view->clearSelection();
    Common::ChilliUtils::selectRow( m_view, index );
    m_model->clearSelection( index );
    m_prevSelection = index;

}


void PlaylistWidget::rowSelected( QModelIndex index )
{
    if( index.isValid() ) {
        m_playlist->selectIndex( index.row() );
    }
}


void PlaylistWidget::setupToolbar()
{
    m_toolBar->setContentsMargins(10,0,10,0);

    m_addFolderAction = new QAction(
                /*QIcon( ":/images/addfolder" ),*/ tr( "+Folder" ), this );
    m_addFolderAction->setShortcut( tr( "Shift+Ctrl+A" ));
    m_toolBar->addAction( m_addFolderAction );

    m_addFilesAction  = new QAction(
                /*QIcon( ":/images/addfiles"),*/ tr( "+Files" ), this );
    m_addFilesAction ->setShortcut( tr( "Shift+Ctrl+S" ));
    m_toolBar->addAction( m_addFilesAction );

//    m_toolBar->addSeparator();

    m_removeAction = new QAction(
                /*QIcon( ":/images/remove" ),*/ tr( "Remove" ), this ) ;
    m_removeAction->setShortcut( QKeySequence::Delete );
    m_removeAction->setEnabled( false );
    m_toolBar->addAction( m_removeAction );

    m_removeAllAction = new QAction(
                /*QIcon( ":/images/clear" ),*/ tr( "Clear" ), this );
    m_removeAllAction->setShortcut( tr("Shift+Ctrl+X" ));
    m_toolBar->addAction(m_removeAllAction);
//    m_toolBar->layout()->setSpacing( 3 );


    connect( m_addFolderAction,
             SIGNAL( triggered() ),
             this,
             SLOT( addFolder() ));
    connect( m_addFilesAction,
             SIGNAL( triggered() ),
             this,
             SLOT( addFiles() ));
    connect( m_removeAction,
             SIGNAL( triggered() ),
             this,
             SLOT( removeItems() ));
    connect( m_removeAllAction,
             SIGNAL( triggered() ),
             m_playlist,
             SLOT( clear() ));
}


void PlaylistWidget::addFolder()
{
    QString dirName = QFileDialog::getExistingDirectory(
                this, tr( "Add Folder" ), QStandardPaths::writableLocation(
                    QStandardPaths::MusicLocation ));
    if( ! dirName.isEmpty() ) {
        QList< QUrl > *urls = new QList< QUrl >();
        urls->append( QUrl::fromLocalFile( dirName ));
        if( ! m_loader->load( urls )) {
            delete urls;
        }
    }
}



void PlaylistWidget::addFiles()
{
    QStringList files =
            QFileDialog::getOpenFileNames( this,
                                           tr( "Select Music Files" ),
                                           QStandardPaths::writableLocation(
                                             QStandardPaths::MusicLocation ));
    if( ! files.empty() ) {
        QList< QUrl > *urls = new QList< QUrl >();
        foreach( QString fileName, files ) {
            urls->append( QUrl::fromLocalFile( fileName ));
        }
        if( ! m_loader->load( urls )) {
            delete urls;
        }
    }
}




void PlaylistWidget::dragEnterEvent( QDragEnterEvent * event )
{
    if( event->mimeData()->hasFormat( "text/uri-list" )) {
        event->acceptProposedAction();
    }
}


void PlaylistWidget::dropEvent( QDropEvent *event )
{
    QList< QUrl > *urls = new QList< QUrl >();
    urls->append( event->mimeData()->urls() );
    if( ! m_loader->load( urls )) {
        delete urls;
    }
}




void PlaylistWidget::removeItems()
{
    QModelIndexList indices = m_view->selectionModel()->selectedRows();
    QList< int > list;
    if( ! indices.empty() ) {
        foreach( QModelIndex index, indices ) {
            list.append( index.row() );
        }
        m_playlist->removeItems(list);
    }
}

void PlaylistWidget::addUrls( QList<QUrl> urls )
{
    //This is a hack
    QList< QUrl > *urlList = new QList< QUrl >();
    urlList->append( urls );
    if( ! m_loader->load( urlList )) {
        delete urlList;
    }
}

void PlaylistWidget::setSelectedAsCurrent()
{
    if( PLAYQUEUE()->numberOfItems() ) {
        QItemSelectionModel* model = m_view->selectionModel();
        if( model && model->hasSelection() ) {
            int index = model->selectedRows().at( 0 ).row();
            PLAYQUEUE()->selectIndex( index );
        }
        else {
            PLAYQUEUE()->selectIndex( 0 );
        }
    }
}


void PlaylistWidget::onPlaylistChanged()
{
    QItemSelectionModel* model = m_view->selectionModel();
    if( model ) {
        int numSelection = model->selectedRows().size();
        m_removeAction->setEnabled(numSelection);
        const QList< Tanyatu::Data::MediaItem *> *allTracks
                = PLAYQUEUE()->getAllItemsInOrder();
        m_model->setItemList( allTracks );
    }
}


void PlaylistWidget::onLoadingFinished(
        QList< QUrl > *inputUrls,
        QList< Tanyatu::Data::AudioTrack *> *loadedTracks )
{
    if( loadedTracks ) {
        delete loadedTracks;
    }
    delete inputUrls;
    m_progress->setVisible( false );
    clearStatus();
    m_view->repaint();
}



void PlaylistWidget::onLoadingStarted( int numFilesToLoad ) {
    m_progress->setVisible( true );
    setStatus( tr("Loading...") );
    m_progress->setMaximum( numFilesToLoad );
}


void PlaylistWidget::setStatus( QString text ) {
    m_status->setText( text );
    m_status->setVisible( true );
}


void PlaylistWidget::clearStatus() {
    m_status->hide();
}



void PlaylistWidget::contextMenuEvent( QContextMenuEvent *event ) {
    QMenu menu( this );
    menu.addAction( m_addFolderAction );
    menu.addAction( m_addFilesAction );
    menu.addAction( m_removeAction );
    menu.addAction( m_removeAllAction );
    menu.exec( event->globalPos() );
}



void PlaylistWidget::showLoadProgress( int numFilesLoaded,
                                       int totalFilesToLoad )
{
    Q_UNUSED( totalFilesToLoad );
    m_progress->setValue( numFilesLoaded );
}

void PlaylistWidget::onAudioTrackLoaded( Tanyatu::Data::AudioTrack *track )
{
    m_playlist->addItem( track, false );
    m_view->repaint();
}

}
