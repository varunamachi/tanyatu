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

#include <QObject>

#include "IComponent.h"
#include "../data/SavedPlaylist.h"
#include "../data/MediaItem.h"
#include "../data/ModelTypes.h"


namespace Tanyatu {


class TANYATU_CORE_EXPORT IPlaylistManager : public QObject,
                                             public IComponent
{
    Q_OBJECT
protected:
    IPlaylistManager( QObject *parent )
        : QObject( parent ) { /*NOTHING TO DO*/}

signals:
    void playlistContentModified( Tanyatu::Data::SavedPlaylist *playlist );

    void playlistModified( Tanyatu::Data::SavedPlaylist *playlist );

    void playlistCreated( Tanyatu::Data::SavedPlaylist *playlist );

    void aboutToRemovePlaylist( Tanyatu::Data::SavedPlaylist *playlist );

    void playlistRemoved( QString playlistName );

    void aboutToClear();

    void cleared();

    void aboutToChangePlaylistManager();

    void playlistManagerChanged();

public slots:
    virtual void appendToPlaylist( QString playlistName,
                                   QList< QString > &trackIds ) = 0;

    virtual void appendToPlaylist( QString playlistName,
                                   QString trackId ) = 0;

    virtual bool deletePlaylist( QString playlistName ) = 0;

    virtual bool createPlaylist( QString playlist) = 0;

    virtual bool setTrackIdList( QString playlistName,
                                 QList< QString > &list)  = 0;

    virtual bool clearPlaylist( QString playlistName ) = 0;

    virtual bool deleteAllPlaylists() = 0;

    virtual bool removeTracksFromPlaylist( QString playlistId,
                                           QList< int > &trackIndices ) = 0;

    virtual void clear() = 0;

    virtual void tagItem( QString playlistName, QString tag ) = 0;

    virtual void removeItemTag( QString playlistName, QString tag ) = 0;

    virtual void onPlaylistSelected( QString playlistName ) = 0;

    virtual void ratePlaylist( QString playlistName,
                               Tanyatu::Data::MediaRating rating ) = 0;

    virtual void onItemRemovalFromStorage( QString trackId ) = 0;

public:

    virtual bool hasPlaylist( QString playlistName ) = 0;

    virtual const QList< QString > trackIdsIn( QString playlistName ) = 0;

    virtual quint32 numTracksIn( QString playlistName ) const = 0;

    virtual quint32 allPlaylists(
                       QList< Data::SavedPlaylist *>& playlists ) = 0;

    virtual Tanyatu::Data::SavedPlaylist * playlist( QString plName ) const= 0;

    virtual ~IPlaylistManager() { /*NOTHING TO DO */}

};

}
