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
