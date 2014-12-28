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

#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QLabel>
#include <QTreeView>
#include <QContextMenuEvent>

#include <core/T.h>
#include <uicommon/itemmodels/AudioTrackModel.h>
#include <uicommon/itemmodels/CommonTrackModel.h>

#include "../viewcommon//RatingDelegate.h"
#include "AudioLibView.h"


namespace GreenChilli { namespace Components {

QString AudioLibView::COMPONENT_ID = "GreeChilli::Std::AudioBrowser";
QString AudioLibView::COMPONENT_NAME = "Audio";

AudioLibView::AudioLibView(QWidget *parent) :
    GreenChilli::ChilliComponent(parent),
    m_filtering( false )
{
    m_allTracks = new QList< Tanyatu::Data::StoredAudio *>();
    m_allAlbums = new QList< Tanyatu::Data::AlbumInfo *>();
    setupUi();
    setupActions();
    setupConnections();
    reloadTracks();
    reloadAlbums();
}


void AudioLibView::setupUi()
{

    m_artistView = new QTreeView( this );
    m_artistView->header()->setSectionResizeMode( QHeaderView::Stretch );
    m_artists = new Tanyatu::Ui::ArtistItemModel( 3, this );
    m_artistProxy = new ArtistSortFilter();
    m_artistProxy->setSourceModel( m_artists );
    m_artistView->setModel( m_artistProxy );
    m_artistView->setItemDelegate( new Views::RatingDelegate( 2, this ));
    m_artistView->setSortingEnabled( true );
    m_artistView->setWordWrap( false );
    m_artistView->setAutoScroll( true );
    m_artistView->setColumnWidth( 1, 48 );
    m_artistView->setColumnWidth( 2, 16 );
    m_artistView->header()->setStretchLastSection( false );
    m_artistView->setFont( QFont( m_artistView->font().defaultFamily(), 8 ));
    m_artistView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_artistView->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_artistView->setFocusPolicy( Qt::WheelFocus );
    m_artistView->setEditTriggers( QAbstractItemView::CurrentChanged );
    m_artistView->setAlternatingRowColors(true);
    m_artistView->setContextMenuPolicy( Qt::ActionsContextMenu );
    m_artistView->setRootIsDecorated(false);

    m_albumView = new QTreeView( this );
    m_albumView->header()->setSectionResizeMode( QHeaderView::Stretch );
    m_albums = new Tanyatu::Ui::AlbumItemModel( 4, this );
    m_albumProxy = new AlbumSortFilter();
    m_albumProxy->setSourceModel( m_albums );
    m_albumView->setModel( m_albumProxy );
    m_albumView->setItemDelegate( new Views::RatingDelegate( 3, this ));
    m_albumView->setSortingEnabled( true );
    m_albumView->setWordWrap( false );
    m_albumView->setAutoScroll( true );
    m_albumView->setColumnWidth( 3, 16 );
    m_albumView->header()->setStretchLastSection( false );
    m_albumView->hideColumn( 0 );
    m_albumView->setFont( QFont( m_albumView->font().defaultFamily(), 8 ));
    m_albumView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_albumView->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_albumView->setFocusPolicy( Qt::WheelFocus );
    m_albumView->setEditTriggers( QAbstractItemView::CurrentChanged );
    m_albumView->setAlternatingRowColors(true);
    m_albumView->setContextMenuPolicy( Qt::ActionsContextMenu );
    m_albumView->setRootIsDecorated(false);

    m_trackView = new QTreeView( this );
    m_trackView->header()->setSectionResizeMode( QHeaderView::Stretch );
    m_tracks = new Tanyatu::Ui::CommonTrackModel( 5, this );
    m_trackProxy = new BrowserTrackSortFilter();
    m_trackProxy->setSourceModel( m_tracks );
    m_trackView->setModel( m_trackProxy );
    m_trackView->setItemDelegate( new Views::RatingDelegate( 4, this ));
    m_trackView->setSortingEnabled( true );
    m_trackView->setWordWrap( false );
    m_trackView->setAutoScroll( true );
    m_trackView->setColumnWidth( 3, 16 );
    m_trackView->header()->setStretchLastSection( false );
    m_trackView->hideColumn( 0 );
    m_trackView->setFont( QFont( m_trackView->font().defaultFamily(), 8 ));
    m_trackView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_trackView->setSelectionBehavior( QAbstractItemView::SelectRows );
    m_trackView->setFocusPolicy( Qt::WheelFocus );
    m_trackView->setEditTriggers( QAbstractItemView::CurrentChanged );
    m_trackView->setAlternatingRowColors(true);
    m_trackView->setContextMenuPolicy( Qt::ActionsContextMenu );
    m_trackView->setRootIsDecorated(false);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    m_filterBox = new Widgets::SearchBox( this );
    filterLayout->addWidget( new QLabel( tr( "Filter" ), this ));
    filterLayout->addWidget( m_filterBox );

    QSplitter *upper = new QSplitter( this );
    upper->addWidget( m_artistView );
    upper->addWidget( m_albumView );

    QSplitter *mainLayout = new QSplitter( Qt::Vertical, this );
    mainLayout->addWidget( upper );
    mainLayout->addWidget( m_trackView );

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout( filterLayout );
    layout->addWidget( mainLayout );

    this->setLayout( layout );
}


void AudioLibView::setupActions()
{
    m_actPlayArtist = new QAction( tr( "Play" ), this );
    m_actPlayAlbum = new QAction( tr( "Play" ), this );
    m_actPlayTracks = new QAction( tr( "Play" ), this );

    m_actDeleteArtist = new QAction( tr( "Delete Artist(s)" ), this );
    m_actDeleteAlbum = new QAction( tr( "Delete Album(s)" ), this );
    m_actDeleteTrack = new QAction( tr( "Delete Track(s)" ), this );

    m_artistView->addAction( m_actPlayArtist );
    m_artistView->addAction( m_actDeleteArtist );

    m_albumView->addAction( m_actPlayAlbum );
    m_albumView->addAction( m_actDeleteAlbum );

    m_trackView->addAction( m_actPlayTracks );
    m_trackView->addAction( m_actDeleteTrack );

    QList< Tanyatu::Data::ArtistInfo * > *artists =
            new QList< Tanyatu::Data::ArtistInfo *>();
    TRACKINF_MANAGER()->allArtist( *artists );
    m_artists->setArtistList( artists );
}


void AudioLibView::selectRow( QTreeView *view, int row )
{
    if( view->model()->rowCount() > row ) {
        QItemSelectionModel *selection = view->selectionModel();
        QAbstractItemModel *model = view->model();
        selection->select( QItemSelection (
                                model->index ( row, 0 ),
                                model->index ( row, model->columnCount() - 1 )),
                            QItemSelectionModel::Select );
    }
}

void AudioLibView::reloadAlbums()
{
    clearAlbumModel();
    if( m_allAlbums ) m_allAlbums->clear();
    TRACKINF_MANAGER()->allAlbums( *m_allAlbums );
    m_artistProxy->invalidate();
    m_albumProxy->invalidate();
    m_trackProxy->invalidate();
}


void AudioLibView::reloadTracks()
{
    clearTrackModel();
    if( m_allTracks ) m_allTracks->clear();
    AUDIO_LIB()->allItems( *m_allTracks );
    m_artistProxy->invalidate();
    m_albumProxy->invalidate();
    m_trackProxy->invalidate();
}


void AudioLibView::reloadArtists()
{
    QList< Tanyatu::Data::ArtistInfo * > *allArtists  =
            new QList< Tanyatu::Data::ArtistInfo *>();
    TRACKINF_MANAGER()->allArtist( *allArtists  );

    QList< Tanyatu::Data::AlbumInfo *> *albums =
            new QList< Tanyatu::Data::AlbumInfo *>();
    m_artists->setArtistList( allArtists );
    m_artistProxy->invalidate();
    m_albumProxy->invalidate();
    m_trackProxy->invalidate();
    if( ! allArtists->isEmpty() ) {
        TRACKINF_MANAGER()->albumsFromArtist( allArtists->at( 0 )->name(),
                                              *albums );
        selectRow( m_artistView, 0 );
        if( ! albums->isEmpty() ) {
            m_albums->setAlbumList( albums );
            selectRow( m_albumView, 0 );
            onAlbumSelected();
        }
        else {
            delete albums;
            clearModels();
        }
    }
    else {
        m_artists->clear();
    }
}


QString AudioLibView::componentId() const
{
    return COMPONENT_ID;
}


QString AudioLibView::componentName() const
{
    return COMPONENT_NAME;
}


const QIcon* AudioLibView::icon() const
{
    return 0;
}


AudioLibView::~AudioLibView()
{
    if( ! m_filtering ) {
        delete m_allAlbums;
        delete m_allTracks;
    }
}


void AudioLibView::onArtistAdded( Tanyatu::Data::ArtistInfo *artist )
{
    if( artist ) {
        reloadArtists();
    }
}


void AudioLibView::onAlbumAdded( Tanyatu::Data::AlbumInfo *album )
{
    if( album ) {
        reloadAlbums();
    }
}


void AudioLibView::onArtistRemoved( QString artistName )
{
    Q_UNUSED( artistName )
    reloadArtists();
}


void AudioLibView::onAlbumRemoved( QString artistName, QString albumName )
{
    QString albumId( Tanyatu::Data::AlbumInfo::getAlbumIdFrom( artistName,
                                                               albumName ));
    reloadAlbums();

}


void AudioLibView::onArtistInfoChanged( Tanyatu::Data::ArtistInfo *artist )
{
    if( artist ) {
        this->update();
    }
}


void AudioLibView::onAlbumInfoChanged( Tanyatu::Data::AlbumInfo *album )
{
    if( album ) {
        this->update();
    }
}


void AudioLibView::onTrackChanged()
{
    if( this->isVisible() ) {
        this->update();
    }
}

void AudioLibView::trackRemovedFromLibrary()
{
    reloadTracks();
}


void AudioLibView::onRemoveArtistAction()
{
    QMessageBox::StandardButton button = QMessageBox::question(
                this,
                tr( "Confirm Artist Deletion" ),
                tr( "This operation will remove artist information and all the "
                    "audio tracks related to those artists from library. "
                    " do you want to continue?"),
                QMessageBox::Cancel | QMessageBox::Ok );
    if( button & QMessageBox::Ok ) {
        QModelIndexList indices =
                m_artistView->selectionModel()->selectedRows( 0 );
        if( ! indices.isEmpty() ) {
            for( const QModelIndex &index : indices ) {
                QString artistName = index.data().toString();
                TRACKINF_MANAGER()->removeArtist( artistName );
            }
        }
    }
}


void AudioLibView::onRemoveAlbumAction()
{
    QMessageBox::StandardButton button = QMessageBox::question(
                this,
                tr( "Confirm Album Deletion" ),
                tr( "This operation will remove album information and all the "
                    "audio tracks related to those albums from library. "
                    " do you want to continue?"),
                QMessageBox::Cancel | QMessageBox::Ok );
    if( button & QMessageBox::Ok ) {
        QModelIndexList indices =
                m_albumView->selectionModel()->selectedRows( 0 );
        if( ! indices.isEmpty() ) {
            for( const QModelIndex &index : indices ) {
                TRACKINF_MANAGER()->removeAlbum( index.data().toString() );
            }
        }
    }
}


void AudioLibView::onRemoveTrackAction()
{
    QMessageBox::StandardButton button = QMessageBox::question(
                this,
                tr( "Confirm Album Deletion" ),
                tr( "This operation will remove audio track(s) from audio "
                    "library do you want to continue?"),
                QMessageBox::Cancel | QMessageBox::Ok );
    if( button & QMessageBox::Ok ) {
        QModelIndexList indices =
                m_trackView->selectionModel()->selectedRows( 0 );
        if( ! indices.isEmpty() ) {
            QList< QString > trackIds;
            for( const QModelIndex &index : indices ) {
                trackIds.append( index.data().toString() );
            }
            AUDIO_LIB()->remove( trackIds );
            update();
        }
    }
}


void AudioLibView::onLibraryCleared()
{
    clearModels();
    m_artists->clear();
    update();
}


void AudioLibView::onArtistSelected()
{
    QModelIndexList selection = m_artistView->selectionModel()->selectedRows(
                0 );
    if( ! selection.isEmpty() ) {
        QList< Tanyatu::Data::AlbumInfo *> *albums =
                new QList< Tanyatu::Data::AlbumInfo *>();
        for( const QModelIndex &index : selection ) {
            TRACKINF_MANAGER()->albumsFromArtist( index.data().toString(),
                                                  *albums );
        }
        if( ! albums->empty() ) {
            clearModels();
            m_albums->setAlbumList( albums );
            m_albumView->selectAll();
//            selectRow( m_albumView, 0 );
            onAlbumSelected();
        }
        else {
            delete albums;
        }
    }
}


void AudioLibView::onAlbumSelected()
{
    QModelIndexList selection = m_albumView->selectionModel()->selectedRows(
                0 );
    if( ! selection.isEmpty() ) {
        QList< QString > trackIds;
        for( const QModelIndex &index : selection ) {
            TRACKINF_MANAGER()->tracksFromAlbum( index.data().toString(),
                                                 trackIds );
        }
        if( ! trackIds.empty() ) {
            QList< Tanyatu::Data::StoredAudio *> *tracks =
                    new QList< Tanyatu::Data::StoredAudio *>();
            AUDIO_LIB()->items( trackIds, *tracks );
            // A::L::L
            if( m_tracks->getTrackList() != m_allTracks ) {
                m_tracks->clear();
            }
            m_tracks->setTrackList( tracks );
        }
    }
}


void AudioLibView::onEnqueueArtistTracks()
{
    QModelIndexList selection =
            m_artistView->selectionModel()->selectedRows( 0 );
    if( ! selection.isEmpty() ) {
        QList< QString > trackIds;
        for( const QModelIndex &index : selection ) {
            TRACKINF_MANAGER()->tracksFromArtist( index.data().toString(),
                                                  trackIds );
        }
        QList< Tanyatu::Data::MediaItem *> tracksOut;
        AUDIO_LIB()->items( trackIds, tracksOut );
        int playIndex = PLAYQUEUE()->numberOfItems();
        PLAYQUEUE()->addItems( tracksOut );
        if( playIndex != PLAYQUEUE()->numberOfItems() ) {
            PLAYQUEUE()->selectIndex( playIndex );
        }
    }
}


void AudioLibView::onEnqueueAlbumTracks()
{
    QModelIndexList selection = m_albumView->selectionModel()->selectedRows(
                0 );
    if( ! selection.isEmpty() ) {
        QList< QString > trackIds;
        for( const QModelIndex &index : selection ) {
            TRACKINF_MANAGER()->tracksFromAlbum( index.data().toString(),
                                               trackIds );
        }
        QList< Tanyatu::Data::MediaItem *> tracksOut;
        AUDIO_LIB()->items( trackIds, tracksOut );
        int playIndex = PLAYQUEUE()->numberOfItems();
        PLAYQUEUE()->addItems( tracksOut );
        PLAYQUEUE()->selectIndex( playIndex );
    }
}


void AudioLibView::onEnqueueTracks()
{
    QModelIndexList selection = m_trackView->selectionModel()->selectedRows(
                0 );
    if( ! selection.isEmpty() ) {
        if( selection.size() == 1 ) {
            Tanyatu::Data::StoredAudio *track = AUDIO_LIB()->item(
                        selection.at( 0 ).data().toString() )    ;
            PLAYQUEUE()->addItem( track, true );
        }
        else {
            QList< QString > trackIds;
            for( const QModelIndex &index : selection ) {
                trackIds.append( index.data().toString() );
            }
            QList< Tanyatu::Data::MediaItem *> tracksOut;
            AUDIO_LIB()->items( trackIds, tracksOut );

            PLAYQUEUE()->addItems( tracksOut );

        }
    }
}

void AudioLibView::addPlayFromArtist()
{
    int playIndex = PLAYQUEUE()->numberOfItems();
    onEnqueueArtistTracks();
    if( playIndex != PLAYQUEUE()->numberOfItems() ) {
        PLAYQUEUE()->selectIndex( playIndex );
    }
}

void AudioLibView::addPlayFromAlbum()
{
    int playIndex = PLAYQUEUE()->numberOfItems();
    onEnqueueAlbumTracks();
    if( playIndex != PLAYQUEUE()->numberOfItems() ) {
        PLAYQUEUE()->selectIndex( playIndex );
    }
}

void AudioLibView::addPlayTracks()
{
    int playIndex = PLAYQUEUE()->numberOfItems();
    onEnqueueTracks();
    if( playIndex != PLAYQUEUE()->numberOfItems() ) {
        PLAYQUEUE()->selectIndex( playIndex );
    }
}


void AudioLibView::onFilterBoxEntry()
{
    QString text = m_filterBox->text();
    if( text.isEmpty() ) {
        m_filtering = false;
        m_tracks->clear( false );
        m_albums->clear( false );
    }
    else if( ! m_filtering ) {
        m_filtering = true;
        clearModels();
        m_tracks->setTrackList( m_allTracks );
        m_albums->setAlbumList( m_allAlbums );
    }
    m_artistProxy->setExpression( text );
    m_albumProxy->setExpression( text );
    m_trackProxy->setExpression( text );
}

void AudioLibView::onFilterBoxComitted()
{
    m_trackView->selectAll();
    onEnqueueTracks();
}


void AudioLibView::setupConnections()
{
    connect( TRACKINF_MANAGER(),
             SIGNAL( artistAdded( Tanyatu::Data::ArtistInfo * )),
             this,
             SLOT( onArtistAdded( Tanyatu::Data::ArtistInfo * )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( albumAdded( Tanyatu::Data::AlbumInfo * )),
             this,
             SLOT( onAlbumAdded( Tanyatu::Data::AlbumInfo * )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( artistRemoved( QString )),
             this,
             SLOT( onArtistRemoved( QString )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( albumRemoved( QString,QString )),
             this,
             SLOT( onAlbumRemoved( QString, QString )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( artistRemoved( QString )),
             this,
             SLOT( onArtistSelected() ));

    connect( TRACKINF_MANAGER(),
             SIGNAL( albumRemoved( QString, QString )),
             this,
             SLOT( onAlbumSelected() ));

    connect( TRACKINF_MANAGER(),
             SIGNAL( artistModified( Tanyatu::Data::ArtistInfo * )),
             this,
             SLOT( onArtistInfoChanged( Tanyatu::Data::ArtistInfo * )));

    connect( TRACKINF_MANAGER(),
             SIGNAL( albumModified( Tanyatu::Data::AlbumInfo * )),
             this,
             SLOT( onAlbumInfoChanged( Tanyatu::Data::AlbumInfo * )));

    connect( AUDIO_LIB(),
             SIGNAL( itemChanged( Tanyatu::Data::StoredAudio * )),
             this,
             SLOT( onTrackChanged() ));

    connect( AUDIO_LIB(),
             SIGNAL( aboutToClear() ) ,
             this,
             SLOT( onLibraryCleared() ));

    connect( AUDIO_LIB(),
             SIGNAL( itemRemoved( QString )),
             this,
             SLOT( trackRemovedFromLibrary() ));

    connect( m_actPlayArtist,
             SIGNAL( triggered() ),
             this,
             SLOT( onEnqueueArtistTracks() ));

    connect( m_actPlayAlbum,
             SIGNAL( triggered() ),
             this,
             SLOT( onEnqueueAlbumTracks() ));

    connect( m_actPlayTracks,
             SIGNAL( triggered() ),
             this,
             SLOT( onEnqueueTracks() ));

    connect( m_actDeleteArtist,
             SIGNAL( triggered() ),
             this,
             SLOT( onRemoveArtistAction() ));

    connect( m_actDeleteAlbum,
             SIGNAL( triggered() ),
             this,
             SLOT( onRemoveAlbumAction() ));

    connect( m_actDeleteTrack,
             SIGNAL( triggered() ),
             this,
             SLOT( onRemoveTrackAction() ));

    connect( m_artistView->selectionModel(),
             SIGNAL( selectionChanged( QItemSelection, QItemSelection )),
             this,
             SLOT( onArtistSelected() ));

    connect( m_artistView,
             SIGNAL( activated( QModelIndex )),
             this,
             SLOT( addPlayFromArtist() ));

    connect( m_albumView->selectionModel(),
             SIGNAL( selectionChanged( QItemSelection, QItemSelection )),
             this,
             SLOT( onAlbumSelected() ));

    connect( m_albumView,
             SIGNAL( activated( QModelIndex )),
             this,
             SLOT( addPlayFromAlbum() ));

    connect( m_trackView,
             SIGNAL( activated( QModelIndex )),
             this,
             SLOT( addPlayTracks() ));

    connect( m_filterBox,
             SIGNAL( textChanged( QString )),
             this,
             SLOT( onFilterBoxEntry() ));

    connect( m_filterBox,
             SIGNAL( returnPressed() ),
             this,
             SLOT( onFilterBoxComitted() ));
}


} }
