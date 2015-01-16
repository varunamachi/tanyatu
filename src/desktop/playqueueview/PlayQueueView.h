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

#include <QWidget>
#include <QTreeView>
#include <QWidget>
//#include <phonon/MediaObject>
#include <QTreeView>
#include <QToolBar>
#include <QLabel>
#include <QProgressBar>

#include <core/T.h>
#include <core/interfaces/IPlayQueue.h>
#include <core/data/MediaItem.h>
#include <core/coreutils/Utils.h>
#include <uicommon/uiutils/AudioLoader.h>
#include <uicommon/itemmodels/MediaItemModel.h>


namespace GreenChilli {

class PlayQueueTrackModel : public Tanyatu::Ui::MediaItemModel
{
public:
    explicit PlayQueueTrackModel( QObject *parent )
        : Tanyatu::Ui::MediaItemModel( 3, parent )
    {

    }


    QVariant data( const QModelIndex &index, int role ) const
    {
        if( role == Qt::BackgroundColorRole ) {
            if( PLAYQUEUE()->currentIndex() == index.row() ) {
                return QColor( 0xB2, 0x6B, 0x00 );
            }
        }
        if( role == Qt::TextAlignmentRole && index.column() == 2 ) {
            return Qt::AlignRight;
        }
        return Tanyatu::Ui::MediaItemModel::data( index, role );
    }

    QVariant dataForTrack( Tanyatu::Data::MediaItem *item,
                           int column ) const
    {
        if( item != nullptr
                && ( item->type() == Tanyatu::Data::Media_LocalAudio
                     || item->type() == Tanyatu::Data::Media_StoredAudio )) {
            Tanyatu::Data::AudioTrack *audio =
                    static_cast< Tanyatu::Data::AudioTrack *>( item );
            switch( column ) {
            case 0: return audio->trackId();
            case 1: return audio->title();
            case 2: return Tanyatu::Utils::getStringTime(
                            audio->duration() );
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

    void clearSelection( int row ) {
        beginResetModel();
        endResetModel();
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

    int m_prevSelection;
};


}

