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

#include <QList>
#include <QString>
#include <QDate>

#include "../data/StoredObject.h"
#include "../data/StoredAudio.h"
#include "../data/SavedPlaylist.h"
#include "../data/StoredMediaObject.h"

namespace Tanyatu {

class TANYATU_CORE_EXPORT IDataUpdater
{

public:
    virtual bool add( Data::StoredObject *stored ) = 0;

    virtual bool update( Data::StoredObject *stored ) = 0;

    virtual bool remove( Data::StoredObject *stored ) = 0;

    virtual bool add( QList< Data::StoredObject *> &objList ) = 0;

    virtual bool update( QList< Data::StoredObject *> &objList ) = 0;


    virtual bool remove( QList< Data::StoredObject *> &stored ) = 0;

    virtual bool savePlaylist( QString playlistName,
                               const QList< QString > &ids ) = 0;

    virtual bool addHistory( QString itemId, QDateTime time ) = 0;

    virtual bool setProperty( QString key,
                              QString type,
                              QString value ) = 0;

    virtual bool removeProperty( QString key ) = 0;

    virtual bool clearAudioLibrary() = 0;

    virtual bool clearVideoLibrary() = 0;

    virtual bool clearHistory() = 0;

    virtual bool clearHistory( QDateTime &from ) = 0;

    virtual bool clearPlaylist( QString playlistName ) = 0;

    virtual bool removePlaylist( QString playlistName ) = 0;

    virtual bool removeAudioTracks( QList< QString > &trackIds ) = 0;

    virtual bool removeAudioTrack( QString trackId ) = 0;

    virtual bool removeVideoTrack( QString trackId ) = 0;

    virtual bool removeVideoTracks( QList< QString > &trackId) = 0;

    virtual bool removeArtist( QString artistName ) = 0;

    virtual bool removeAlbum( QString albumId ) = 0;

    virtual bool removeHistoryItem( QString trackId ) = 0;

    virtual bool removeFromHistory( QList< QString > &trackIds ) = 0;

    virtual bool updateRating( Data::StoredMediaObject *stored ) = 0;

    virtual bool updatePlayCount( Data::StoredMediaObject *stored ) = 0;

    virtual bool updateTags( Data::StoredMediaObject *stored ) = 0;

    virtual bool saveCurrentPlaylist(
            const QList< Tanyatu::Data::MediaItem *> &list ) = 0;

    virtual ~IDataUpdater() { /*NOTHING TO DO*/ }

protected:
    IDataUpdater() { /*NOTHING TO DO*/ }
};

} //end of ns
