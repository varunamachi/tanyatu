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

#include <QPair>

#include "../interfaces/IDataRetriever.h"

class QSqlQuery;

namespace Tanyatu { namespace Impl{

class TANYATU_CORE_EXPORT StdDataRetriever : public IDataRetriever
{
public:
    StdDataRetriever() { /* NOTHING_YET */}

    ~StdDataRetriever() { /* NOTHING_YET */ }

    bool getStoredAudio( QHash< QString, Data::StoredAudio*> &items );

    bool getHistory( QList< QPair< QString, QDateTime > *> &recentIds);

    bool getTrackIdsFromArtist( QString artistName, QSet< QString > &trackIds );

    bool getAlbumIdsFromArtist( QString artistName, QSet< QString > &albumIds );

    bool getTrackIdsFromAlbum( QString artistName, QString albumName,
                               QSet< QString > &trackIds );

    bool getAllArtists(QHash< QString, Data::ArtistInfo *> &artists );

    bool getAllAlbums( QHash< QString, Data::AlbumInfo *> &albums);

    bool getPlaylists( QHash< QString, Data::SavedPlaylist *> &playlists);

    bool getPlaylistContent(QString playlistName, QList< QString > &trackIds);

    int getPlayCount( QString trackId );

    Data::MediaRating getRating( QString trackId );

    bool getProperty( QString key, QString &typeOut, QString &valueOut );

    int getSavedPlayQueue( QList< QPair< QString, QString > > &trackInfoOut );

private:

    void addTags( Data::StoredMediaObject *obj, QString tags );
};

} } //end of namespaces
