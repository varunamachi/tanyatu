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
