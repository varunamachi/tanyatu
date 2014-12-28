/*******************************************************************************
 * AlbumArtistView.h
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
#pragma once


#include <QTreeView>
#include <QAction>

#include <core/data/ArtistInfo.h>
#include <core/data/AlbumInfo.h>
#include <uicommon/itemmodels/AlbumItemModel.h>
#include <uicommon/itemmodels/ArtistItemModel.h>
#include <uicommon/itemmodels/AudioTrackModel.h>

#include "../widgets/SearchBox.h"
#include "../app/ChilliComponent.h"
#include "AudioLibSortFilter.h"

namespace GreenChilli { namespace Components {

class AudioLibView : public ChilliComponent
{
    Q_OBJECT
public:
    explicit AudioLibView(QWidget *parent = 0);

    QString componentId() const;

    QString componentName() const;

    const QIcon* icon() const;

    ~AudioLibView();

    static QString COMPONENT_ID;

    static QString COMPONENT_NAME;

protected:
//    void keyPressEvent( QKeyEvent *evt );
    
private slots:
    void onArtistAdded( Tanyatu::Data::ArtistInfo *artist );

    void onAlbumAdded( Tanyatu::Data::AlbumInfo *album );

    void onArtistRemoved( QString artistName );

    void onAlbumRemoved( QString artistName, QString albumName );

    void onArtistInfoChanged( Tanyatu::Data::ArtistInfo *artist );

    void onAlbumInfoChanged( Tanyatu::Data::AlbumInfo *album );

    void onTrackChanged();

    void trackRemovedFromLibrary();

    void onRemoveArtistAction();

    void onRemoveAlbumAction();

    void onRemoveTrackAction();

    void onLibraryCleared();

    void onArtistSelected();

    void onAlbumSelected();

    void onEnqueueArtistTracks();

    void onEnqueueAlbumTracks();

    void onEnqueueTracks();

    void addPlayFromArtist();

    void addPlayFromAlbum();

    void addPlayTracks();

    void onFilterBoxEntry();

    void onFilterBoxComitted();

    void reloadTracks();

    void reloadAlbums();

    void reloadArtists();
private:
    void setupUi();

    void setupActions();

    void setupConnections();

    void selectRow( QTreeView *view, int row );

    void clearTrackModel()
    {
        m_tracks->clear( m_tracks->getTrackList() != m_allTracks );
    }

    void clearAlbumModel()
    {
        m_albums->clear( m_albums->getAlbumList() != m_allAlbums );
    }

    void clearModels()
    {
        clearTrackModel();
        clearAlbumModel();
    }

    Widgets::SearchBox *m_filterBox;

    QTreeView *m_artistView;

    QTreeView *m_albumView;

    QTreeView *m_trackView;

    QAction *m_actPlayArtist;

    QAction *m_actPlayAlbum;

    QAction *m_actPlayTracks;

    QAction *m_actDeleteArtist;

    QAction *m_actDeleteAlbum;

    QAction *m_actDeleteTrack;

    Tanyatu::Ui::ArtistItemModel *m_artists;

    Tanyatu::Ui::AlbumItemModel *m_albums;

    Tanyatu::Ui::AudioTrackModel *m_tracks;

    ArtistSortFilter *m_artistProxy;

    AlbumSortFilter *m_albumProxy;

    BrowserTrackSortFilter *m_trackProxy;

    QList< Tanyatu::Data::AlbumInfo *> *m_allAlbums;

    QList< Tanyatu::Data::StoredAudio *> *m_allTracks;

    bool m_filtering;


};

} }
