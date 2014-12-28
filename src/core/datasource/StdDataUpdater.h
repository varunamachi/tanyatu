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

#include "../interfaces/IDataUpdater.h"
#include "../data/StoredVideo.h"
#include "../data/ArtistInfo.h"
#include "../data/AlbumInfo.h"


namespace Tanyatu { namespace Impl {

class TANYATU_CORE_EXPORT StdDataUpdater : public IDataUpdater
{
public:
    StdDataUpdater(){}

    ~StdDataUpdater(){}

    bool add( Data::StoredObject *stored );

    bool update( Data::StoredObject *stored );

    bool remove( Data::StoredObject *stored );

    bool add( QList< Data::StoredObject *> &objList );

    bool update( QList< Data::StoredObject *> &objList );

    bool remove(QList<Data::StoredObject *> &objList );

    bool savePlaylist( QString playlistName, const QList< QString > &ids );

    bool addHistory( QString itemId, QDateTime time );

    //Settings framework will come later, this for sample :P
    bool setProperty( QString key, QString type, QString value );

    bool removeProperty( QString key );

    bool clearAudioLibrary();

    bool clearVideoLibrary();

    bool clearHistory();

    bool clearHistory( QDateTime &from );

    bool clearPlaylist( QString playlistName );

    bool removePlaylist( QString playlistName );

    bool removeAudioTracks( QList< QString > &trackIds );

    bool removeAudioTrack( QString trackId );

    bool removeVideoTracks( QList< QString > &trackIds );

    bool removeVideoTrack( QString trackId );

    bool removeArtist( QString artistName );

    bool removeAlbum( QString albumId );

    bool removeHistoryItem( QString trackId );

    bool removeFromHistory( QList< QString > &trackIds );

    bool updateRating( Data::StoredMediaObject *stored );

    bool updatePlayCount( Data::StoredMediaObject *stored );

    bool updateTags( Data::StoredMediaObject *stored );

    bool saveCurrentPlaylist( const QList< Tanyatu::Data::MediaItem *> &list );

private:
    bool add( Data::StoredAudio *audio );

    bool add( Data::StoredVideo *video );

    bool add( Data::SavedPlaylist *playlist );

    bool add( Data::ArtistInfo *artist );

    bool add( Data::AlbumInfo *album );

    bool update( Data::StoredAudio *audio );

    bool update( Data::StoredVideo *video );

    bool update( Data::SavedPlaylist *playlist );

    bool update( Data::ArtistInfo *artist );

    bool update( Data::AlbumInfo *album );

    QString getTagString( const QSet<QString> &tags );

    QString getTableName( Data::StoredMediaObject *stored );

    QString getUniqueColumnName( Data::StoredMediaObject *stored );

};

} } // End of namespaces

