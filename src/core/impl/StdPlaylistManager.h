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
#include <QList>

#include "../data/SavedPlaylist.h"
#include "../interfaces/IPlaylistManager.h"
#include "../data/AudioTrack.h"


namespace Tanyatu { namespace Impl {


class TANYATU_CORE_EXPORT StdPlaylistManager : public IPlaylistManager
{
    Q_OBJECT

public:
    StdPlaylistManager( QObject *parent = 0 );

    virtual ~StdPlaylistManager();

    void appendToPlaylist( QString playlistName, QList< QString > &trackIds );

    void appendToPlaylist( QString playlistName, QString trackId );

    bool deletePlaylist( QString playlistName );

    bool createPlaylist( QString playlistName);

    bool setTrackIdList( QString playlistName,
                       QList< QString > &list);

    bool clearPlaylist( QString playlistName );

    bool deleteAllPlaylists();

    bool removeTracksFromPlaylist( QString playlistId,
                                   QList< int > &trackIndices );

    void clear();

    void onPlaylistSelected( QString playlistName );

    void onItemRemovalFromStorage( QString trackId );

    void ratePlaylist( QString playlistName,
                               Data::MediaRating rating);

    void tagItem( QString playlistName, QString tag );

    void removeItemTag( QString playlistName, QString tag );


    bool hasPlaylist( QString playlistName );

    Tanyatu::Data::SavedPlaylist * playlist( QString plName ) const;

    const QList< QString > trackIdsIn( QString playlistName );


    quint32 numTracksIn( QString playlistName ) const;

    quint32 allPlaylists(
                       QList<Data::SavedPlaylist *> &playlists );

    QString uniqueName() const;

    QString module()  const;

    QString displayName() const;

    bool init();

    static QString COMPONENT_NAME;

    static QString COMPONENT_MODULE;

private:
    void loadPlaylist( const QString playlistName );

    QHash< QString, Data::SavedPlaylist *> m_playlists;

    QHash< QString, QList< QString > *> m_playlistToTrackIds;


};
}
}

