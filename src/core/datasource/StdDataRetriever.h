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
