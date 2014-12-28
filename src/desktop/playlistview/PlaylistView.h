/*******************************************************************************
 * PlaylistView.h
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

#include <QToolBar>
#include <QAction>
#include <uicommon/itemmodels/AudioTrackModel.h>

#include "PlaylistProxyModel.h"
#include "AddPlaylistDialog.h"
#include "AddTrackDialog.h"
#include "../widgets/SearchBox.h"
#include "../viewcommon/TrackFilter.h"
#include "../viewcommon/CommonAudioTrackView.h"
#include "../app/ChilliComponent.h"

class QTreeView;
class QToolBar;


namespace GreenChilli { namespace Components {

class PlaylistView : public GreenChilli::ChilliComponent
{
    Q_OBJECT
public:
    explicit PlaylistView( QWidget *parent = nullptr );

    ~PlaylistView();

    QString componentId() const;

    QString componentName() const;

    const QIcon* icon() const;

protected:
    void contextMenuEvent( QContextMenuEvent *event );


private slots:
    void onTrackDialogRequested();

    void onCreatePlaylist();

    void onDeletePlaylist();

    void onAddPlaylistsToQueue();

    void onRemoveTracksFromPlaylist();

    void onAddPlaylistToQueue( QModelIndex index );

    void onPlaylistsSelected();

    void onTrackSelected( QString trackId );

    void onPlaylistContentChanged( QString playlistName );

    void onUiStateChanged();

    void onClearPlaylist();

    void onAddTracksToPlayQueue();

private:
    void setupUi();

    void setupActions();

    void setupConnections();

    Playlists::AddTrackDialog *m_contentDialog;

    Playlists::AddPlaylistDialog *m_createPlDailog;

    QTreeView *m_view;

    Playlists::PlaylistProxyModel *m_plProxy;

    Widgets::SearchBox *m_playlistFilter;

    QToolBar *m_playlistToolbar;

    QToolBar *m_tracksToolbar;

    QAction *m_actAddPlaylist;

    QAction *m_actRemovePlaylist;

    QAction *m_actAddToPlayQueue;

    QAction *m_actClearPlaylist;

    QAction *m_actAddTracksToPlayQueue;

    Views::CommonAudioTrackView *m_trackView;

    Tanyatu::Ui::AudioTrackModel *m_trackModel;

    Views::TrackFilter *m_trackProxy;

    Widgets::SearchBox *m_trackFilter;

    QAction *m_actAddTracks;

    QAction *m_actRemoveTracks;

    QMenu *m_trackContextMenu;

    /**
     * @brief m_plNumberOfChanges Stores the number of changes since the last
     * serialization of each playlist that has changes. Whenever the number of
     * changes exceed a predefined limit, the playlist is saved and
     * corresponding entry is removed from this hash map. The contents of this
     * hash map is also serialized just before application closes. This
     * mechanism has a side effect that if application is not properly closed
     * the un-serialized data is lost
     */
    QHash< QString, int > m_plNumberOfChanges;

    static const QString COMPONENT_ID;

    static const QString COMPONENT_NAME;

    static const QIcon *COMPONENT_ICON;
};

} }

