/*******************************************************************************
 * AudioLibView.h
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
#include <QPushButton>
#include <QToolBar>
#include <QAction>
#include <QProgressBar>
#include <QLabel>
#include <QToolBar>
#include <QIcon>
#include <QThread>
#include <QQueue>
#include <QAbstractItemModel>

#include <uicommon/uiutils/AudioLibraryLoader.h>

#include "../widgets/SearchBox.h"
#include "../viewcommon/TrackFilter.h"
#include "../app/ChilliComponent.h"

namespace GreenChilli { namespace Components {

class AudioTrackView : public ChilliComponent
{
    Q_OBJECT
public:
    explicit AudioTrackView( QAbstractItemModel *model,
                           quint32 ratingCol,
                           QWidget *parent = nullptr );

    QString componentId() const;

    QString componentName() const;

    const QIcon* icon() const;
    
protected:
    void contextMenuEvent( QContextMenuEvent *event );

    void dragEnterEvent( QDragEnterEvent *event );

    void dropEvent( QDropEvent *event );

private slots:
    void addDirectory();

    void addFiles();

    void onArtistEnqueue();

    void onAlbumEnqueue();

    void onTracksEnqueue();

    void playArtist();

    void playAlbum();

    void playTracks();

    void removeFromLibrary();

    void clearLibrary();

    void setStatus( QString text );

    void clearStatus();

    void showLoadProgress( int numFilesLoaded, int totalFilesToLoad );

    void onAudioTrackLoaded( Tanyatu::Data::StoredAudio *track );

    void onLoadingStarted( int numFilesToLoad );

    void onLoadingFinished( QList< QUrl > *inputUrls,
                            QList< Tanyatu::Data::StoredAudio *> *loadedTracks);

    void onRatingChanged();

    void onTrackInfoUpdated( Tanyatu::Data::StoredAudio *track );

    void onArtistAdded( Tanyatu::Data::ArtistInfo *artist );

    void onAlbumAdded( Tanyatu::Data::AlbumInfo *album );

    void onFilterBoxCommitted();

private:
    void setupUi( QAbstractItemModel *model, quint32 ratingCol );

    void setupActions();

    void setupConnections();

    QTreeView *m_view;

    QPixmap *m_icon;

    QToolBar *m_toolBar;

    Views::TrackFilter *m_sortFilter;

    QProgressBar *m_progress;

    QLabel *m_status;

    GreenChilli::Widgets::SearchBox *m_filterEdit;

    Tanyatu::Worker::AudioLibraryLoader *m_loader;

    QAction *m_actRemove;

    QAction *m_actAddDir;

    QAction *m_actAddFiles;

    QAction *m_actClear;

    QAction *m_actAddToPlaylist;

    QAction *m_actAddFromArtist;

    QAction *m_actAddFromAlbum;

    QAction *m_actPlayFromArtist;

    QAction *m_actPlayFromAlbum;

    QAction *m_actPlayTracks;

    QList< Tanyatu::Data::StoredObject *> *m_addedObjects;

    static const QString COMPONENT_ID;

    static const QString COMPONENT_NAME;
    
};

} }
