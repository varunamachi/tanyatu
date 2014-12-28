/*******************************************************************************
 * AudioLibraryLoader.h
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
#include <QStringList>
#include <QFileInfo>

#include <core/data/StoredAudio.h>
#include <core/T.h>

#include "../TanyatuCommonGlobal.h"

namespace Tanyatu { namespace Worker {

class TANYATU_GUI_EXPORT AudioLibraryLoader : public QThread
{
    Q_OBJECT

public:
    AudioLibraryLoader( QObject *parent = 0 )
        : QThread( parent ) {
        m_supportedExt = T::get()->audioEngine()->supportedFileExtentions();
        m_mediaList = 0;
        m_urls = 0;
        connect( this, SIGNAL( finished() ), this, SLOT( reset() ));
    }

    ~AudioLibraryLoader()
    {

    }


    virtual bool load( QList< QUrl > *urlList,
                       QList< Data::StoredAudio *> *trackListOut = 0 );

protected:
    void run();

    virtual void buildCandidateList( QString dirPath,
                                    QList< QString > &filesToLoadOut );

    virtual void createAndAddItemFrom( const QString &filePath );

    virtual bool isSupportedFile( QFileInfo file )
    {
        return m_supportedExt.contains( file.suffix() );
    }

    QList< Data::StoredAudio *> *m_mediaList;

    QList< QUrl >* m_urls;

    QStringList m_supportedExt;


signals:
    void calculatingCandidateFiles();

    void finishedCalculatingCandidates();

    void loadingStarted( int itemsToLoad );

    void loadedItem( Tanyatu::Data::StoredAudio *item );

    void loadProgress( int filesLoaded, int totalFiles, QString prevFileLoaded);

    void error( const QString errorMsg );

    void loadingFinished( QList< QUrl > *inputUrls,
                          QList< Tanyatu::Data::StoredAudio *> *tracks );
protected slots:
    virtual void reset();
};

} }

