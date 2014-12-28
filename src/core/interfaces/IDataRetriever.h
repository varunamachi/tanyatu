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

#include <QHash>
#include <QSet>
#include <QVariant>

#include "../TanyatuCoreGlobal.h"
#include "../data/SavedPlaylist.h"
#include "../data/AlbumInfo.h"
#include "../data/ArtistInfo.h"
#include "../data/StoredAudio.h"

namespace Tanyatu {

class TANYATU_CORE_EXPORT IDataRetriever
{
public:
    virtual bool getStoredAudio(
            QHash< QString, Data::StoredAudio*> &items ) = 0;

    virtual bool getHistory(
            QList< QPair< QString, QDateTime > *> &recentIds) = 0;

    virtual bool getTrackIdsFromArtist( QString artistName,
                                        QSet< QString > &trackIds ) = 0;

    virtual bool getAlbumIdsFromArtist( QString artistName,
                                        QSet< QString > &albumIds ) = 0;

    virtual bool getTrackIdsFromAlbum( QString artistName,
                                       QString albumName,
                                       QSet< QString > &trackIds ) = 0;

    virtual bool getAllArtists(
            QHash< QString, Data::ArtistInfo *> &artist ) = 0;


    virtual bool getAllAlbums( QHash< QString, Data::AlbumInfo *> &albums) = 0;

    virtual bool getPlaylists(
            QHash< QString, Data::SavedPlaylist *> &playlists) = 0;

    virtual bool getPlaylistContent( QString playlistName,
                                     QList< QString > &trackIds ) = 0;

    virtual int getPlayCount( QString trackId ) = 0;

    virtual Data::MediaRating getRating( QString trackId )            = 0;

    virtual bool getProperty( QString key,
                              QString &typeOut,
                              QString &valueOut )      = 0;

    /**
     * @brief getSavedPlayQueue Gives the content of the play queue that was
     * saved when application exited last time
     * @param list of track_id - track_path pair - is an out param
     */
    virtual int getSavedPlayQueue(
            QList< QPair< QString, QString > > &trackInfoOut ) = 0;

    virtual ~IDataRetriever() { /*NOTHING TO DO*/ }

protected:
    IDataRetriever() { /*NOTHING TO DO*/ }
};

} //End of ns Tanyatu
