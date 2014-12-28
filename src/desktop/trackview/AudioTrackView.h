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
