/*******************************************************************************
 * PlaylistWidget.h
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

#include <QWidget>
#include <QTreeView>
#include <QWidget>
//#include <phonon/MediaObject>
#include <QTreeView>
#include <QToolBar>
#include <QLabel>
#include <QProgressBar>

#include <core/interfaces/IPlayQueue.h>
#include <core/data/MediaItem.h>
#include <uicommon/uiutils/AudioLoader.h>
#include <uicommon/itemmodels/MediaItemModel.h>


namespace GreenChilli {

class PlayQueueTrackModel : public Tanyatu::Ui::MediaItemModel
{
public:
    explicit PlayQueueTrackModel( QObject *parent )
        : Tanyatu::Ui::MediaItemModel( 2, parent )
    {

    }

    QVariant dataForTrack( Tanyatu::Data::MediaItem *item,
                           int column ) const
    {
        if( item != nullptr
                && item->type() == Tanyatu::Data::Media_LocalAudio ) {
            Tanyatu::Data::AudioTrack *audio =
                    static_cast< Tanyatu::Data::AudioTrack *>( item );
            switch( column ) {
            case 0: return audio->trackId();
            case 1: return audio->title();
            case 2: return audio->duration();
            }
        }
        return "";
    }


    QString headerForColumn( int column ) const
    {
        switch( column ) {
        case 0: return tr( "Track Id" );
        case 1: return tr( "Title" );
        case 2: return tr( "Len" );
        }
        return "";
    }
};

class PlaylistWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistWidget(QWidget *parent = 0);


public slots:
    void addFolder();

    void addFiles();

    void removeItems();

    void addUrls( QList< QUrl > urls );

    void setSelectedAsCurrent();


protected:
    void dragEnterEvent(QDragEnterEvent * event);

    void dropEvent(QDropEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void indexChanged( Tanyatu::Data::MediaItem *dummy, int index);

    void onPlaylistChanged();

    void setStatus(QString text);

    void clearStatus();

    void setupUi();

    void setupConnections();

    void setupToolbar();

    void showLoadProgress( int numFilesLoaded, int totalFilesToLoad );

    void onAudioTrackLoaded( Tanyatu::Data::AudioTrack *track );

    void onLoadingStarted( int numFilesToLoad );

    void onLoadingFinished( QList< QUrl > *inputUrls,
                            QList< Tanyatu::Data::AudioTrack *> *loadedTracks );

    void rowSelected( QModelIndex index );

private:
    QTreeView *m_view;

    QToolBar *m_toolBar;

    Tanyatu::Worker::AudioLoader *m_loader;

    QProgressBar *m_progress;

    QLabel *m_status;

    Tanyatu::IPlayQueue *m_playlist;

    QAction *m_removeAction;

    QAction *m_removeAllAction;

    QAction *m_addFolderAction;

    QAction *m_addFilesAction;

    QAction *m_randomize;

    QAction *m_loopAction;

    PlayQueueTrackModel *m_model;
};


}

