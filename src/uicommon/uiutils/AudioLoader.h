/*******************************************************************************
 * AudioLoader.h
 *
 * Copyright (c) 2012, Varuna L Amachi. All rights reserved.
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

#include <QThread>
#include <QSet>
#include <QFileInfo>
#include <QStringList>

#include <core/data/MediaItem.h>
#include <core/data/AudioTrack.h>

#include "../TanyatuCommonGlobal.h"

namespace Tanyatu { namespace Worker {
class TANYATU_GUI_EXPORT AudioLoader : public QThread
{
    Q_OBJECT
public:
    explicit AudioLoader( QObject *parent = 0 );

    static Data::MediaItem *readItem( QString trackId, QString filePath );

    virtual ~AudioLoader();

    virtual bool load( QList< QUrl > *urlList,
                       QList< Data::AudioTrack *> *trackListOut = 0 );

protected:

    QList< Data::AudioTrack *> *m_mediaList;

    QList< QUrl >* m_urls;

    QStringList m_supportedExt;

    void run();

    virtual void buildCandidateList( QString dirPath,
                                    QList< QString >& filesToLoadOut );

    virtual void createAndAddItemFrom( const QString &filePath );

    virtual bool isSupportedFile( QFileInfo file );

signals:

    void calculatingCandidateFiles();

    void finishedCalculatingCandidates();

    void loadingStarted( int itemsToLoad );

    void loadedItem( Tanyatu::Data::AudioTrack *item );

    void loadProgress( int filesLoaded, int totalFiles, QString prevFileLoaded);

    void error( const QString errorMsg );

    void loadingFinished( QList< QUrl > *inputUrls,
                          QList< Tanyatu::Data::AudioTrack *> *tracks );

protected slots:
    virtual void reset();
};
}
}
