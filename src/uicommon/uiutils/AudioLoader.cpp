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

#include "AudioLoader.h"

#include <QDir>
#include <QList>
#include <QMessageBox>
#include <QUuid>
#include <QDebug>
#include <taglib/audioproperties.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include <core/T.h>

namespace Tanyatu { namespace Worker {

AudioLoader::AudioLoader( QObject *parent) :
    QThread(parent)
{
    m_supportedExt = T::get()->audioEngine()->supportedFileExtentions();
    m_mediaList = 0;
    m_urls = 0;
    connect( this, SIGNAL( finished() ), this, SLOT( reset() ));
}


AudioLoader::~AudioLoader()
{

}


bool AudioLoader::load( QList< QUrl > *urlList,
                        QList< Data::AudioTrack *> *trackListOut )
{
    if( urlList == 0 ) {
        return false;
    }
    if(! this->isRunning()) {
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


void AudioLoader::run()
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




void AudioLoader::buildCandidateList( QString dirPath,
                                     QList<QString> &filesToLoadOut )
{
    QDir dir( dirPath );
    foreach( QString file, dir.entryList( m_supportedExt, QDir::Files )) {
        if( isSupportedFile( QFileInfo( file ))) {

            filesToLoadOut.append( dir.absoluteFilePath( file ));
        }
    }
    foreach( QString dirName,
             dir.entryList( QDir::Dirs | QDir::NoDotAndDotDot )) {
        buildCandidateList(dir.absoluteFilePath(dirName), filesToLoadOut);
    }
}


void AudioLoader::createAndAddItemFrom( const QString &filePath )
{
    QUrl url = QUrl::fromLocalFile( filePath );
    QString trackId = url.toString();
    Data::AudioTrack *track = 0;
    if( AUDIO_LIB() && AUDIO_LIB()->hasItem( trackId )) {
        track = AUDIO_LIB()->item( trackId );
    }
    else
    {
        TagLib::FileRef fileRef( url.toLocalFile().toStdString().c_str() );
        if( fileRef.isNull() ) {
            return;
        }
        track = new Data::AudioTrack( trackId, url );
        QString title = TStringToQString( fileRef.tag()->title() );
        track->setMetadata(  title.isEmpty() ? QFileInfo( filePath ).baseName()
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
    }
    if( m_mediaList )
    {
        m_mediaList->append( track );
    }
    emit loadedItem( track );
}


bool AudioLoader::isSupportedFile( QFileInfo file )
{
    return m_supportedExt.contains( QString( "*." ).append( file.suffix() ) );
}

void AudioLoader::reset()
{
    m_urls = 0;
    m_mediaList = 0;
}



Data::MediaItem *AudioLoader::readItem( QString trackId, QString filePath )
{
    QUrl url = QUrl::fromLocalFile( filePath );
    Data::AudioTrack *track = 0;
    if( AUDIO_LIB() && AUDIO_LIB()->hasItem( trackId )) {
        track = AUDIO_LIB()->item( trackId );
    }
    else
    {
        TagLib::FileRef fileRef( url.toLocalFile().toStdString().c_str() );
        if( fileRef.isNull() ) {
            return 0;
        }
        track = new Data::AudioTrack( trackId, url );
        QString title = TStringToQString( fileRef.tag()->title() );
        track->setMetadata(  title.isEmpty() ? QFileInfo( filePath ).baseName()
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
    }
    return track;
}

} }
