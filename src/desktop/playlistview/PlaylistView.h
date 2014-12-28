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

