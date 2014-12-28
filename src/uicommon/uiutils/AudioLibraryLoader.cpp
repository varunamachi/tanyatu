/*******************************************************************************
 * AudioLibraryLoader.cpp
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


#include <QDir>
#include <QList>
#include <QUuid>
#include <QDebug>
#include <QtWidgets/QMessageBox>

#include <taglib/audioproperties.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include <core/T.h>

#include "AudioLibraryLoader.h"


namespace Tanyatu { namespace Worker {


bool AudioLibraryLoader::load( QList< QUrl >* urlList,
                        QList< Data::StoredAudio *> *trackListOut )
{
    if( urlList == 0 ) {
        return false;
    }
    if( ! this->isRunning() ) {
        m_urls = urlList;
        m_mediaList = trackListOut;
        start();
        return true;
    }
    else {
        qDebug() << "Please wait till other process finish";
        return false;
    }
}


void AudioLibraryLoader::run()
{
    QList< QString > filesToLoad;
    emit calculatingCandidateFiles();
    foreach( QUrl url, (*m_urls) ) {
        QFileInfo info( url.toLocalFile() );
        if( info.exists() ) {
            if( info.isDir() ) {
                buildCandidateList( info.absoluteFilePath(), filesToLoad );
            }
            else if( info.isFile() ) {
                if( isSupportedFile( info )) {
                    filesToLoad.append( info.absoluteFilePath() );
                }
            }
        }
        else {
            // ?!?
        }
    }
    emit finishedCalculatingCandidates();
    int numFiles = filesToLoad.size();
    emit loadingStarted( numFiles );
    for( int i = 0; i < numFiles; i++ ) {
        createAndAddItemFrom( filesToLoad.at(i) );
        emit loadProgress( i, numFiles, filesToLoad.at(i) );
    }
    emit loadingFinished( m_urls, m_mediaList );
}




void AudioLibraryLoader::buildCandidateList( QString dirPath,
                                     QList<QString> &filesToLoadOut )
{

    QDir dir( dirPath );
    foreach( QString file, dir.entryList( m_supportedExt, QDir::Files )) {
        filesToLoadOut.append( dir.absoluteFilePath( file ));
    }
    foreach( QString dirName,
             dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot )) {
        buildCandidateList(dir.absoluteFilePath(dirName), filesToLoadOut);
    }
}


void AudioLibraryLoader::createAndAddItemFrom( const QString &filePath )
{
    QUrl url = QUrl::fromLocalFile( filePath );
    QString trackId = url.toString();
    Data::StoredAudio *track = AUDIO_LIB()->item( trackId );
    if( ! track ) {
        TagLib::FileRef fileRef( url.toLocalFile().toStdString().c_str() );
        if( fileRef.isNull() ) {
            return;
        }
        track = new Data::StoredAudio( trackId, url );
        QString title = TStringToQString( fileRef.tag()->title() );
        track->setMetadata( title.isEmpty() ? QFileInfo( filePath ).baseName()
                                            : title,
                            TStringToQString( fileRef.tag()->artist() ),
                            TStringToQString( fileRef.tag()->album() ),
                            QString( fileRef.tag()->year() ) ,
                            TStringToQString( fileRef.tag()->genre() ),
                            TStringToQString( fileRef.tag()->comment() ),
                            fileRef.tag()->track(),
                            fileRef.audioProperties()->length(),
                            fileRef.audioProperties()->bitrate(),
                            fileRef.audioProperties()->sampleRate(),
                            fileRef.audioProperties()->channels() );
        if( m_mediaList ) {
            m_mediaList->append( track );
        }
    }
    emit loadedItem( track );
}


void AudioLibraryLoader::reset()
{
    m_urls = 0;
    m_mediaList = 0;
}

} }
