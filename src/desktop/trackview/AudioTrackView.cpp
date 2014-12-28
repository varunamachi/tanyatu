/*******************************************************************************
 * AudioLibView.cpp
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

#include <algorithm>

#include <QFileDialog>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QIcon>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QHeaderView>
#include <QPixmapCache>
#include <QMimeData>

#include <core/T.h>
#include <core/data/MediaItem.h>
#include <core/coreutils/JobManager.h>
#include <core/coreutils/Utils.h>


#include "../widgets/RatingWidget.h"
#include "../viewcommon/RatingDelegate.h"
#include "AudioTrackView.h"

namespace GreenChilli { namespace Components {

const QString AudioTrackView::COMPONENT_ID = "GreeChilli::Std::AudioLibView";
const QString AudioTrackView::COMPONENT_NAME = "Tracks";


AudioTrackView::AudioTrackView( QAbstractItemModel *model,
                            quint32 ratingCol,
                            QWidget *parent )
    : ChilliComponent( parent )
    , m_view( new QTreeView( this ))
    , m_toolBar( new QToolBar( this ))
    , m_sortFilter( new Views::TrackFilter( this ))
    , m_progress( new QProgressBar( this ))
    , m_status( new QLabel( this ))
    , m_filterEdit( new Widgets::SearchBox( this ))
    , m_loader( new Tanyatu::Worker::AudioLibraryLoader( this ))
{
    setupActions();
    setupUi( model, ratingCol );
    setupConnections();
    setAcceptDrops( true );
}


void AudioTrackView::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu menu( this );
    menu.addAction( m_actPlayTracks );
    menu.addAction( m_actPlayFromArtist );
    menu.addAction( m_actPlayFromAlbum );
    menu.addSeparator();
    menu.addAction( m_actAddToPlaylist );
    menu.addAction( m_actAddFromArtist );
    menu.addAction( m_actAddFromAlbum );
    menu.addSeparator();
    menu.addAction( m_actAddDir );
    menu.addAction( m_actAddFiles );
    menu.addSeparator();
    menu.addAction( m_actRemove );
    menu.addAction( m_actClear );
    menu.exec( event->globalPos() );
}


void AudioTrackView::dragEnterEvent(QDragEnterEvent *event)
{
    if( event->mimeData()->hasFormat( "text/uri-list" )) {
        event->acceptProposedAction();
    }
}


void AudioTrackView::dropEvent(QDropEvent *event)
{
    QList< QUrl > *urls = new QList< QUrl >();
    QList< Tanyatu::Data::StoredAudio *> *loadedList =
            new QList< Tanyatu::Data::StoredAudio *>();

    urls->append( event->mimeData()->urls() );
    if( ! m_loader->load( urls, loadedList )) {
        delete urls;
    }
}


void AudioTrackView::setupUi( QAbstractItemModel *model, quint32 ratingCol )
{
    QHBoxLayout *upperLayout = new QHBoxLayout();
    upperLayout->addWidget( new QLabel( tr( "Filter" ), this ));
    upperLayout->addWidget( m_filterEdit );
    upperLayout->addWidget( m_toolBar );

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget( m_status );
    bottomLayout->addWidget( m_progress );


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout( upperLayout );
    mainLayout->addWidget( m_view );
    mainLayout->addLayout( bottomLayout );

    this->setContentsMargins( 0, 0, 0, 0 );
    this->setLayout( mainLayout );

    m_progress->hide();
    m_status->hide();
    m_sortFilter = new Views::TrackFilter( this );
    m_sortFilter->setSourceModel( model );
    m_sortFilter->setDynamicSortFilter( true );
    m_sortFilter->sort( 1 );

    auto delegate = new Views::RatingDelegate( ratingCol, this );
    m_view->setModel( m_sortFilter );
    m_view->setItemDelegate( delegate );
    m_view->setSortingEnabled( true );
    m_view->setWordWrap( false );
    m_view->setAutoScroll( true );
    m_view->header()->setSectionResizeMode( QHeaderView::Stretch );
    m_view->setColumnWidth( 4, 16 );
    m_view->header()->setStretchLastSection( false );
    m_view->hideColumn( 0 );
    m_view->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_view->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_view->setFocusPolicy( Qt::WheelFocus );
    m_view->setEditTriggers(QAbstractItemView::CurrentChanged);
    m_view->setAlternatingRowColors(true);
    m_view->setFont( QFont( m_view->font().defaultFamily(), 8 ));

}


void AudioTrackView::setupActions()
{
    m_actRemove   = new QAction( QIcon( ":/images/remove" ),
                               tr( "Remove From library" ),
                               this );
    m_actClear    = new QAction( QIcon ( ":/images/clear"),
                               tr( "Clear Library" ),
                               this );
    m_actAddDir   = new QAction( QIcon( ":/images/addfolder" ),
                               tr( "Add Directory" ),
                               this );
    m_actAddFiles = new QAction( QIcon( ":/images/addfiles" ),
                               tr( "Add Files" ),
                               this );

    m_actPlayTracks = new QAction( tr( "Play" ), this );
    m_actPlayFromArtist = new QAction( tr( "Play from this artist" ), this );
    m_actPlayFromAlbum = new QAction( tr( "Play this album" ), this);
    m_actAddToPlaylist  = new QAction( tr( "Add to Playlist" ), this );
    m_actAddFromArtist = new QAction( tr( "Add from Artist" ), this );
    m_actAddFromAlbum  = new QAction( tr( "Add from Album" ), this );

    m_toolBar->addAction( m_actAddDir );
    m_toolBar->addAction( m_actAddFiles );
    m_toolBar->addAction( m_actRemove );
    m_toolBar->addAction( m_actClear );
}


void AudioTrackView::setupConnections()
{
    connect( m_actPlayTracks,
             SIGNAL( triggered() ),
             this,
             SLOT( playTracks() ));

    connect( m_actPlayFromArtist,
             SIGNAL( triggered() ),
             this,
             SLOT( playArtist() ));

    connect( m_actPlayFromAlbum,
             SIGNAL( triggered() ),
             this,
             SLOT( playAlbum() ));

    connect( m_actAddDir,
             SIGNAL( triggered() ),
             this,
             SLOT( addDirectory() ));

    connect( m_actAddFiles,
             SIGNAL( triggered() ),
             this,
             SLOT( addFiles() ) );

    connect( m_actAddToPlaylist,
             SIGNAL( triggered() ),
             this,
             SLOT( onTracksEnqueue() ));

    connect( m_actAddFromArtist,
             SIGNAL( triggered() ),
             this,
             SLOT( onArtistEnqueue() ));

    connect( m_actAddFromAlbum,
             SIGNAL( triggered() ),
             this,
             SLOT( onAlbumEnqueue() ));

    connect( m_actRemove,
             SIGNAL( triggered() ),
             this,
             SLOT( removeFromLibrary() ));

    connect( m_actClear,
             SIGNAL( triggered() ),
             this,
             SLOT( clearLibrary() ));

    connect( m_view,
             SIGNAL( activated( QModelIndex )),
             this,
             SLOT( playTracks() ));

    connect( m_filterEdit,
             SIGNAL( textChanged( QString )),
             m_sortFilter,
             SLOT( setExpression(QString) ));

    connect( m_loader,
             SIGNAL( loadingStarted( int )),
             this,
             SLOT( onLoadingStarted(int) ));

    connect( m_loader,
             SIGNAL( loadedItem( Tanyatu::Data::StoredAudio* )),
             this,
             SLOT( onAudioTrackLoaded( Tanyatu::Data::StoredAudio* )));

    connect( m_loader,
             SIGNAL( loadProgress( int, int, QString )),
             this,
             SLOT( showLoadProgress( int, int )));

    connect( m_loader,
             SIGNAL( loadingFinished(
                         QList<QUrl>*, QList< Tanyatu::Data::StoredAudio *> *)),
             this, SLOT( onLoadingFinished(
                         QList<QUrl>*, QList<Tanyatu::Data::StoredAudio *> *)));

    connect( AUDIO_LIB(),
             SIGNAL( ratingChanged( Tanyatu::Data::StoredAudio*)),
             this,
             SLOT( onRatingChanged() ));

    connect( AUDIO_LIB(),
             SIGNAL( itemChanged( Tanyatu::Data::StoredAudio* )),
             this,
             SLOT( onTrackInfoUpdated( Tanyatu::Data::StoredAudio* )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( artistAdded( Tanyatu::Data::ArtistInfo * )),
             this,
             SLOT( onArtistAdded( Tanyatu::Data::ArtistInfo * )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( albumAdded( Tanyatu::Data::AlbumInfo * )),
             this,
             SLOT( onAlbumAdded( Tanyatu::Data::AlbumInfo * )));

    connect( m_filterEdit,
             SIGNAL( returnPressed() ),
             this,
             SLOT( onFilterBoxCommitted() ));
}


void AudioTrackView::addDirectory()
{
    QString dirName = QFileDialog::getExistingDirectory(
                this,
                tr( "Add Folder" ),
                QStandardPaths::writableLocation(
                    QStandardPaths::MusicLocation ));
    if( ! dirName.isEmpty() ) {
        QList< QUrl > *urls = new QList< QUrl >();
        urls->append( QUrl::fromLocalFile( dirName ));
        QList< Tanyatu::Data::StoredAudio *> *loadedList =
                new QList< Tanyatu::Data::StoredAudio *>();
        if( ! m_loader->load( urls, loadedList )) {
            delete urls;
            delete loadedList;
        }
    }
}

void AudioTrackView::addFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(
                this,
                tr( "Select Music Files" ),
                QStandardPaths::writableLocation(
                    QStandardPaths::MusicLocation ));
    if( ! files.empty() ) {
        QList<QUrl> *urls = new QList< QUrl >();
        foreach( QString fileName, files ) {
            urls->append( QUrl::fromLocalFile( fileName ));
        }
        QList< Tanyatu::Data::StoredAudio *> *loadedList =
                new QList< Tanyatu::Data::StoredAudio *>();
        if( ! m_loader->load( urls, loadedList )) {
            delete urls;
            delete loadedList;
        }
    }
}


void AudioTrackView::onArtistEnqueue()
{
    QModelIndexList indexList = m_view->selectionModel()->selectedRows( 0 );
    if( ! indexList.isEmpty() ) {
        QString trackId;
        QSet< QString > seleactedArtists;
        QList< QString > trackIds;
        foreach( const QModelIndex &index, indexList ) {
            trackId = index.data().toString();
            Tanyatu::Data::StoredAudio *track = AUDIO_LIB()->item( trackId );
            if( ! seleactedArtists.contains( track->artist() ))  {
                seleactedArtists.insert( track->artist() );
                TRACKINF_MANAGER()->tracksFromArtist( track->artist(),
                                                      trackIds );
            }
        }
        QList< Tanyatu::Data::MediaItem *> itemsOut;
        AUDIO_LIB()->items( trackIds, itemsOut );
        PLAYQUEUE()->addItems( itemsOut );
    }
}


void AudioTrackView::onAlbumEnqueue()
{
    QModelIndexList indexList = m_view->selectionModel()->selectedRows( 0 );
    if( ! indexList.isEmpty() ) {
        QString trackId;
        QSet< QString > seleactedAlbums;
        QList< QString > trackIds;
        foreach( const QModelIndex &index, indexList ) {
            trackId = index.data().toString();
            Tanyatu::Data::StoredAudio *track = AUDIO_LIB()->item( trackId );
            QString albumId = Tanyatu::Data::AlbumInfo::getAlbumIdFrom(
                        track->artist(), track->album() );
            if( ! seleactedAlbums.contains( albumId ))
            {
                seleactedAlbums.insert( albumId );
                TRACKINF_MANAGER()->tracksFromAlbum( albumId,
                                                  trackIds );
            }
        }
        QList< Tanyatu::Data::MediaItem *> itemsOut;
        AUDIO_LIB()->items( trackIds, itemsOut );
        PLAYQUEUE()->addItems( itemsOut );
    }
}


void AudioTrackView::onTracksEnqueue()
{
    QModelIndexList indices = m_view->selectionModel()->selectedRows(0);
    if( ! indices.empty() ) {
        foreach( QModelIndex index, indices ) {
            QString trackId = index.data().toString();
            Tanyatu::Data::StoredAudio *track = AUDIO_LIB()->item( trackId );
            PLAYQUEUE()->addItem( track, false );
        }
    }
}


void AudioTrackView::playArtist()
{
    int playIndex = PLAYQUEUE()->numberOfItems();
    onArtistEnqueue();
    if( playIndex != PLAYQUEUE()->numberOfItems() ) {
        PLAYQUEUE()->selectIndex( playIndex );
    }
}


void AudioTrackView::playAlbum()
{
    int playIndex = PLAYQUEUE()->numberOfItems();
    onAlbumEnqueue();
    if( playIndex != PLAYQUEUE()->numberOfItems() ) {
        PLAYQUEUE()->selectIndex( playIndex );
    }
}


void AudioTrackView::playTracks()
{
    int playIndex = PLAYQUEUE()->numberOfItems();
    onTracksEnqueue();
    if( playIndex != PLAYQUEUE()->numberOfItems() ) {
        PLAYQUEUE()->selectIndex( playIndex );
    }
}


void AudioTrackView::removeFromLibrary()
{
    QMessageBox::StandardButton button = QMessageBox::question(
                this,
                tr( "Confirm Audio Tracks Deletion" ),
                tr( "This operation will remove tracks from the library "
                    "(The files themselves will not be deleted.) do you want "
                    "to continue?"),
                    QMessageBox::Cancel | QMessageBox::Ok );
    if( button & QMessageBox::Ok )  {
        QModelIndexList indices = m_view->selectionModel()->selectedRows(0);
        QList< QString > remIds;
        foreach( QModelIndex index, indices ) {
            remIds.append( index.data().toString() );
        }
        AUDIO_LIB()->remove( remIds );
    }
}


void AudioTrackView::clearLibrary()
{
    QMessageBox::StandardButton button = QMessageBox::question(
                this,
                tr( "Confirmation: Removal from library" ),
                tr( "This operation will clear all the content from the library"
                    "(The files themselves will not be deleted.) do you want "
                    "to continue?" ),
                    QMessageBox::Cancel | QMessageBox::Ok );
    if( button & QMessageBox::Ok ) {
        AUDIO_LIB()->clearLibrary();
    }
}


void AudioTrackView::onLoadingFinished(
        QList< QUrl > *inputUrls,
        QList< Tanyatu::Data::StoredAudio *> *loadedTracks )
{
    AUDIO_LIB()->add( *loadedTracks );
    delete inputUrls;
    m_progress->setVisible( false );
    clearStatus();
}

void AudioTrackView::onRatingChanged()
{
    m_view->repaint();
}

void AudioTrackView::onTrackInfoUpdated( Tanyatu::Data::StoredAudio *track )
{
    if( track ) {
        AUDIO_LIB()->updateItem( track->trackId(), *track );
    }
}


void AudioTrackView::onLoadingStarted( int numFilesToLoad ) {
    m_progress->setVisible( true );
    setStatus( tr("Loading...") );
    m_addedObjects = new QList< Tanyatu::Data::StoredObject *>();
    m_progress->setMaximum( numFilesToLoad );
}


void AudioTrackView::setStatus( QString text ) {
    m_status->setText( text );
    m_status->setVisible( true );
}


void AudioTrackView::clearStatus() {
    m_status->hide();
}

void AudioTrackView::showLoadProgress( int numFilesLoaded,
                                       int totalFilesToLoad )
{
    Q_UNUSED( totalFilesToLoad );
    m_progress->setValue( numFilesLoaded );
}

void AudioTrackView::onAudioTrackLoaded( Tanyatu::Data::StoredAudio *track )
{
    AUDIO_LIB()->add( track );
    m_view->repaint();
}


const QIcon* AudioTrackView::icon() const
{
    return 0;
}


QString AudioTrackView::componentName() const
{
    return COMPONENT_NAME;
}


QString AudioTrackView::componentId() const
{
    return COMPONENT_ID;
}


void AudioTrackView::onArtistAdded( Tanyatu::Data::ArtistInfo *artist )
{
    if( artist ) {
        m_addedObjects->append( artist );
    }
}


void AudioTrackView::onAlbumAdded( Tanyatu::Data::AlbumInfo *album )
{
    if( album ) {
        m_addedObjects->append( album );
    }
}

void AudioTrackView::onFilterBoxCommitted()
{
    m_view->selectAll();
    playTracks();
}

} }
