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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <QMenu>
#include <QContextMenuEvent>

#include <core/T.h>
#include <uicommon/itemmodels/CommonTrackModel.h>

#include "../viewcommon/RatingDelegate.h"
#include "../desktoputils/ChilliUtils.h"
#include "PlaylistView.h"
#include "PlaylistProxyModel.h"
#include "AddTrackDialog.h"
#include "AddPlaylistDialog.h"

const int PL_SERIALIZATION_CHANGE_THREASHOLD = 5;

namespace GreenChilli { namespace Components {

const QString PlaylistView::COMPONENT_ID = "GreeChilli::Std::PlaylistView";
const QString PlaylistView::COMPONENT_NAME = "Playlists";
//const QIcon *PlaylistView::COMPONENT_ICON = 0;

PlaylistView::PlaylistView( QWidget *parent ) :
    GreenChilli::ChilliComponent( parent )
  , m_contentDialog( 0 )
  , m_createPlDailog( 0 )
{
    m_playlistFilter = new Widgets::SearchBox( this );
    m_playlistToolbar = new QToolBar( this );
    QHBoxLayout *pltoolbarLayout = new QHBoxLayout();
    pltoolbarLayout->addWidget( new QLabel( tr( "Filter Playlists " ), this ));
    pltoolbarLayout->addWidget( m_playlistFilter );
    pltoolbarLayout->addWidget( m_playlistToolbar );
    QMargins margins = pltoolbarLayout->contentsMargins();
    margins.setRight( 0 );
    pltoolbarLayout->setContentsMargins( margins );

    m_view = new QTreeView( this );
    m_plProxy = new Playlists::PlaylistProxyModel( this );
//    m_plProxy->setSourceModel( PL_MANAGER() );
    m_view->setModel( m_plProxy );
    m_view->setItemDelegate( new Views::RatingDelegate( 3, m_view ));
    m_view->setSortingEnabled( true );
    m_view->setWordWrap( false );
    m_view->setAutoScroll( true );
    m_view->header()->setSectionResizeMode( QHeaderView::Stretch );
//    m_view->header()->setResizeMode( 1, QHeaderView::Stretch );
//    m_view->header()->setResizeMode( 2, QHeaderView::Stretch );
    m_view->setColumnWidth( 3, 16 );
    m_view->header()->setStretchLastSection( false );
    m_view->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_view->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_view->setFocusPolicy( Qt::WheelFocus );
    m_view->setEditTriggers( QAbstractItemView::CurrentChanged );
    m_view->setAlternatingRowColors(true);
    m_view->setFont( QFont( m_view->font().defaultFamily(), 8 ));
    m_view->setRootIsDecorated( false );
    m_view->setContentsMargins( QMargins() );
    m_view->setContentsMargins( QMargins() );

    m_trackFilter = new Widgets::SearchBox( this );
    m_tracksToolbar = new QToolBar( this );
    QHBoxLayout *trackToolbarLayout = new QHBoxLayout();
    trackToolbarLayout->addWidget( new QLabel( tr( "Filter Tracks " ), this));
    trackToolbarLayout->addWidget( m_trackFilter );
    trackToolbarLayout->addWidget( m_tracksToolbar );
    margins = trackToolbarLayout->contentsMargins();
    margins.setRight( 0 );
    trackToolbarLayout->setContentsMargins( margins );

    m_trackProxy = new Views::TrackFilter( this );
    m_trackModel = new Tanyatu::Ui::CommonTrackModel( 5, this );
    m_trackProxy->setSourceModel( m_trackModel );
    m_trackView = new Views::CommonAudioTrackView( m_trackProxy,
                                             new Views::RatingDelegate( 4 ),
                                             this );
    m_trackFilter->setContentsMargins( QMargins() );


    QVBoxLayout *plLayout = new QVBoxLayout();
    plLayout->addLayout( pltoolbarLayout );
    plLayout->addWidget( m_view );
    plLayout->setSpacing( 0 );
    plLayout->setContentsMargins( QMargins() );
    QWidget *plWidget = new QWidget( this );
    plWidget->setContentsMargins( 0, 0, 0, 0 );
    plWidget->setLayout( plLayout );

    QVBoxLayout *trackLayout = new QVBoxLayout();
    trackLayout->addLayout( trackToolbarLayout );
    trackLayout->addWidget( m_trackView );
    trackLayout->setContentsMargins( QMargins() );
    trackLayout->setSpacing( 0 );
    QWidget *trackWidget = new QWidget( this );
    trackWidget->setContentsMargins( 0, 0, 0, 0 );
    trackWidget->setLayout( trackLayout );


    QSplitter *splitter = new QSplitter( Qt::Vertical, this );
    splitter->addWidget( plWidget );
    splitter->addWidget( trackWidget );
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget( splitter );
    margins = mainLayout->contentsMargins();
    margins.setTop( 0 );
    mainLayout->setContentsMargins( margins );
    this->setLayout( mainLayout );

    setupActions();
    setupConnections();
    onUiStateChanged();
}


PlaylistView::~PlaylistView()
{
    for( auto itr = m_plNumberOfChanges.begin();
         itr != m_plNumberOfChanges.end();
         ++ itr ) {
        QString plName = itr.key();
        DATA_UPDATER()->savePlaylist( plName ,
                                      PL_MANAGER()->trackIdsIn( plName ));
    }
    m_plNumberOfChanges.clear();
    m_trackView->setContextMenu( 0 );
    delete m_trackContextMenu;
}


void PlaylistView::setupActions()
{
    m_actAddPlaylist = new QAction( QIcon( ":/images/addfolder" ),
                                    tr( "Create a playlist" ),
                                    this );
    m_actRemovePlaylist = new QAction( QIcon( ":/images/remove" ),
                                       tr( "Delete selected playlist(s)" ),
                                       this );
    m_actAddToPlayQueue = new QAction( tr( "Add to Play Queue" ),
                                       this );
    m_actAddTracks = new QAction( QIcon( ":/images/addfiles" ),
                                  tr( "Add tracks to playlist" ),
                                  this );

    m_actAddTracksToPlayQueue = new QAction( "Add to play queue",
                                       this );
    m_actRemoveTracks = new QAction( QIcon( ":/images/remove" ),
                                   tr( "Remove selected tracks from playlist" ),
                                   this );
    m_actClearPlaylist = new QAction( QIcon( ":/images/clear"),
                                      tr( "Remove all tracks from playlist" ),
                                      this );
    m_actAddTracksToPlayQueue = new QAction( tr( "Add to play queue" ), this );

    m_playlistToolbar->addAction( m_actAddPlaylist );
    m_playlistToolbar->addAction( m_actRemovePlaylist );
    m_playlistToolbar->addAction( m_actAddTracks );
//    m_playlistToolbar->addAction( m_actAddToPlayQueue );

    m_tracksToolbar->addAction( m_actRemoveTracks );
    m_tracksToolbar->addAction( m_actClearPlaylist );



    connect( m_actAddPlaylist, SIGNAL( triggered() ),
             this, SLOT( onCreatePlaylist() ));
    connect( m_actRemovePlaylist, SIGNAL( triggered()),
             this, SLOT( onDeletePlaylist() ));
    connect( m_actAddToPlayQueue, SIGNAL( triggered() ),
             this, SLOT( onAddPlaylistsToQueue() ));
    connect( m_actAddTracks, SIGNAL( triggered() ),
             this, SLOT( onTrackDialogRequested() ));
    connect( m_actRemoveTracks, SIGNAL( triggered()),
             this, SLOT( onRemoveTracksFromPlaylist() ));
    connect( m_actAddToPlayQueue, SIGNAL( triggered() ),
             this, SLOT( onAddTracksToPlayQueue() ));
    connect( m_actClearPlaylist, SIGNAL( triggered() ),
             this, SLOT( onClearPlaylist() ));
    connect( m_actAddTracksToPlayQueue, SIGNAL( triggered()),
             this, SLOT( onAddTracksToPlayQueue() ));

    m_trackContextMenu = new QMenu();
    m_trackContextMenu->addAction( m_actAddTracksToPlayQueue );
    m_trackContextMenu->addSeparator();
    m_trackContextMenu->addAction( m_actClearPlaylist );
    m_trackContextMenu->addAction( m_actRemoveTracks );
    m_trackView->setContextMenu( m_trackContextMenu );
}

void PlaylistView::setupConnections()
{
    connect( m_view, SIGNAL( doubleClicked( QModelIndex )),
             this, SLOT( onAddPlaylistToQueue( QModelIndex )));
    connect( m_trackView, SIGNAL( activated( QString )),
             this, SLOT( onTrackSelected( QString )));
    connect( m_playlistFilter, SIGNAL( textChanged( QString )),
             m_plProxy, SLOT( setExpression( QString )));
    connect( m_trackFilter, SIGNAL( textChanged( QString )),
             m_trackProxy, SLOT( setExpression(QString )));
    connect( m_view, SIGNAL( activated( QModelIndex )),
             this, SLOT( onUiStateChanged() ));
    connect( PL_MANAGER(),
             SIGNAL( playlistCreated(Tanyatu::Data::SavedPlaylist*)),
             this,
             SLOT( onUiStateChanged() ));
    connect( PL_MANAGER(),
             SIGNAL( playlistContentModified( Tanyatu::Data::SavedPlaylist* )),
             this,
             SLOT( onUiStateChanged() ));
    connect( m_trackView, SIGNAL( uiStateChanged() ),
             this, SLOT( onUiStateChanged() ));
    connect( m_view->selectionModel(),
             SIGNAL( selectionChanged( QItemSelection,QItemSelection )),
             this,
             SLOT( onPlaylistsSelected() ));
    connect( m_view->selectionModel(),
             SIGNAL( selectionChanged( QItemSelection,QItemSelection )),
             this,
             SLOT( onUiStateChanged() ));
}

QString PlaylistView::componentId() const
{
    return COMPONENT_ID;
}


QString PlaylistView::componentName() const
{
    return COMPONENT_NAME;
}


const QIcon * PlaylistView::icon() const
{
//    return COMPONENT_ICON;
    return 0;
}



void PlaylistView::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu menu;
    menu.addAction( m_actAddToPlayQueue );
    menu.addSeparator();
    menu.addAction( m_actAddTracks );
    menu.addAction( m_actRemovePlaylist );
    menu.addAction( m_actClearPlaylist );
    menu.addAction( m_actAddTracks );
    menu.exec( event->globalPos() );
}



void PlaylistView::onTrackDialogRequested()
{
    if( ! m_contentDialog) {
        m_contentDialog = new Playlists::AddTrackDialog( this );
        m_contentDialog->setMinimumSize( 400, 400 );
    }
    if( m_contentDialog->exec() == QDialog::Accepted ) {
        QModelIndexList indexList = m_view->selectionModel()->selectedRows();
        if( ! indexList.isEmpty() ) {
            QList< Tanyatu::Data::StoredAudio *> list;
            m_contentDialog->selectedTracks( list );
            m_contentDialog->clear();
            QString playlistName = indexList.at( 0 ).data().toString();
            foreach( Tanyatu::Data::AudioTrack *track, list ) {
                PL_MANAGER()->appendToPlaylist( playlistName,
                                                track->trackId() );
            }
            Common::ChilliUtils::selectRow( m_view, indexList.at( 0 ).row() );
            auto task = [ playlistName ]() -> Tanyatu::RespFunc
            {
                //It could be moved to playlist manager itself, to conform to
                //the idea of moving all data source oprn to core.
                DATA_UPDATER()->savePlaylist(
                        playlistName,
                        PL_MANAGER()->trackIdsIn( playlistName ));
                return nullptr;
            };
            JOB_MANAGER()->addJob( "save_playlist", task );
        }
    }
}

void PlaylistView::onCreatePlaylist()
{
    if( ! m_createPlDailog ) {
        m_createPlDailog = new Playlists::AddPlaylistDialog( this );
    }
    if( m_createPlDailog->exec() == QDialog::Accepted ) {
        QString newPlName = m_createPlDailog->newPlaylistName();
        m_createPlDailog->clear();
        PL_MANAGER()->createPlaylist( newPlName );
        DATA_UPDATER()->add( PL_MANAGER()->playlist( newPlName ));
    }
}


void PlaylistView::onDeletePlaylist()
{
    QModelIndexList selection = m_view->selectionModel()->selectedRows();
    if( selection.isEmpty() ) { return; }
    bool result = QMessageBox::question( this,
            tr( "Playlist Deletion Confirmation "),
            tr( "Are you sure you want to delete the selected playlist(s)?" ),
            QMessageBox::Ok,
            QMessageBox::Cancel ) == QMessageBox::Ok;
    if( result ) {
        foreach( QModelIndex index, selection ) {
            QString plName = index.data().toString();
            PL_MANAGER()->deletePlaylist(  plName );
            m_trackModel->clear( true );
        }
    }
}


void PlaylistView::onAddPlaylistsToQueue()
{
    QModelIndexList selection = m_view->selectionModel()->selectedRows();
    QList< QString > trackIds;
    if( ! selection.empty() ) {
        foreach( QModelIndex index, selection ) {
            QString plName = index.data().toString();
            trackIds.append( PL_MANAGER()->trackIdsIn( plName ));
            PL_MANAGER()->onPlaylistSelected( plName );
        }
        QList< Tanyatu::Data::MediaItem *> playlistContent;
        AUDIO_LIB()->items( trackIds, playlistContent );
        PLAYQUEUE()->addItems( playlistContent );
    }
}


void PlaylistView::onRemoveTracksFromPlaylist()
{
    QModelIndexList pls = m_view->selectionModel()->selectedRows();
    if( pls.isEmpty() || pls.size() > 1 ) return;
    QString playlistName = pls.at( 0 ).data().toString();
    QList< int > indexList;
    m_trackView->selectedIndexes( indexList );
    PL_MANAGER()->removeTracksFromPlaylist( playlistName, indexList );
    DATA_UPDATER()->savePlaylist( playlistName,
                                  PL_MANAGER()->trackIdsIn( playlistName ));
    Common::ChilliUtils::selectRow( m_view, pls.at( 0 ).row() );
    m_trackView->repaint();
}



void PlaylistView::onClearPlaylist()
{
    QModelIndexList selection = m_view->selectionModel()->selectedRows();
    foreach( QModelIndex index, selection ) {
        QString plName = index.data().toString();
        PL_MANAGER()->clearPlaylist( plName );
    }
    Common::ChilliUtils::selectRow( m_view,
                                   selection.at( selection.size() - 1 ).row() );
    m_trackView->repaint();
}



void PlaylistView::onAddTracksToPlayQueue()
{
    QList< Tanyatu::Data::MediaItem *> list;
    m_trackView->selectedItems( list );
    PLAYQUEUE()->addItems( list );
}



void PlaylistView::onAddPlaylistToQueue( QModelIndex index )
{
    Q_UNUSED( index );
    int newIndex = PLAYQUEUE()->numberOfItems();
    QModelIndexList list = m_view->selectionModel()->selectedRows();
    QString plName = list.at( 0 ).data().toString();
    QList< QString > ids = PL_MANAGER()->trackIdsIn( plName );
    QList< Tanyatu::Data::MediaItem *> items;
    AUDIO_LIB()->items( ids, items );
    PLAYQUEUE()->addItems( items );
    PL_MANAGER()->onPlaylistSelected( plName );
    if( ! items.empty() ) {
        PLAYQUEUE()->selectIndex( newIndex );
    }
}

void PlaylistView::onPlaylistsSelected()
{
    QModelIndexList selected = m_view->selectionModel()->selectedRows( 0 );
    m_trackModel->clear( false );
    auto tracks = m_trackModel->getTrackList();
    if( ! tracks ) {
        tracks = new QList< Tanyatu::Data::StoredAudio *>();
    }
    foreach( QModelIndex index, selected ) {
        QString plName = index.data().toString();
        QList< QString > ids = PL_MANAGER()->trackIdsIn( plName );
        AUDIO_LIB()->items( ids, *tracks );
    }
    m_trackModel->setTrackList( tracks );
}


void PlaylistView::onTrackSelected(QString trackId )
{
    Tanyatu::Data::StoredAudio *track = AUDIO_LIB()->item( trackId );
    if( track ) {
        PLAYQUEUE()->addItem( track, true );
    }
}


void PlaylistView::onPlaylistContentChanged( QString playlistName )
{
    //Currently we dont need delayed updates
    Q_UNUSED( playlistName )
//    if( m_plNumberOfChanges.contains( playlistName )) {
//        m_plNumberOfChanges.insert( playlistName, 1 );
//    }
//    else {
//        int value = m_plNumberOfChanges.take( playlistName );
//        value ++;
//        m_plNumberOfChanges.insert( playlistName, value );
//        if( value > PL_SERIALIZATION_CHANGE_THREASHOLD ) {
//            DATA_UPDATER()->savePlaylist( playlistName,
//                                      PL_MANAGER()->trackIdsIn( playlistName ));
//        }
//    }
//    m_trackView->update();
}


void PlaylistView::onUiStateChanged()
{
    QModelIndexList selection = m_view->selectionModel()->selectedRows();
    bool plval = ! selection.empty();
    m_actRemovePlaylist->setEnabled( plval );
    m_actAddToPlayQueue->setEnabled( plval );
    m_actClearPlaylist->setEnabled( plval );
    m_actAddTracks->setEnabled( plval && selection.size() == 1 );
    QList< int > trackSelection;
    m_trackView->selectedIndexes( trackSelection );
    m_actRemoveTracks->setEnabled( ! trackSelection.empty() );
}


} // namespace PlaylistComp
} // namespace GreenChilli
