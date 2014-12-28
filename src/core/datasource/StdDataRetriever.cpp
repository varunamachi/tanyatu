#include <QtSql/QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "StdDataRetriever.h"
#include "../T.h"
#include "../coreutils/FileSystemUtil.h"


namespace Tanyatu { namespace Impl {

bool StdDataRetriever::getStoredAudio(
        QHash< QString, Data::StoredAudio*> &items )
{
    QSqlQuery query( "SELECT * FROM AudioLibrary;" );
    int count = 0;
    T::logger()->info( "Retrieving audio library items." );
    bool result = query.exec();
    T::logger()->log( "retrieveAudioLibrary", query );
    if( result ) {
        while( query.next() ) {
            QString trackId     = query.value( 0  ).toString();
            QString trackUrl    = query.value( 1  ).toString();
            QString title       = query.value( 2  ).toString();
            QString artist      = query.value( 3  ).toString();
            QString album       = query.value( 4  ).toString();
            QString date        = query.value( 5  ).toString();
            QString genre       = query.value( 6  ).toString();
            QString desc        = query.value( 7  ).toString();
            int     trackNum    = query.value( 8  ).toInt();
            int     duration    = query.value( 9  ).toInt();
            int     bitRate     = query.value( 10 ).toInt();
            int     sampleRate  = query.value( 11 ).toInt();
            int     channels    = query.value( 12 ).toInt();
            int     playCount   = query.value( 14 ).toInt();
            QString tags        = query.value( 15 ).toString();
            Data::MediaRating rating  =
                 static_cast< Data::MediaRating >( query.value( 13 ).toInt() );

            if( QFile( QUrl( trackUrl ).toLocalFile() ).exists() ) {
                Data::StoredAudio *audio =
                        new Data::StoredAudio( trackId, trackUrl );
                audio->setMetadata( title,
                                    artist,
                                    album,
                                    date,
                                    genre,
                                    desc,
                                    trackNum,
                                    duration,
                                    bitRate,
                                    sampleRate,
                                    channels );
                audio->setPlayCount( playCount );
                audio->setRating( rating );
                addTags( audio, tags );
                items.insert( audio->trackId(), audio );
            }
            else {
                T::logger()->warning(
                            QString( "Track at %1 doesn't exist. Ignoring.." )
                            .arg( trackUrl ));
            }
        }
        T::logger()->info( QString( "%1 tracks retrieved successfully" )
                           .arg( count ));
        return true;
    }
    T::logger()->error( "Audio library retrieval failed" );
    return false;
}


bool StdDataRetriever::getHistory(
        QList< QPair< QString, QDateTime > *> &recentIds)
{
    QSqlQuery query( "SELECT * FROM PlayHistory;" );
    T::logger()->info( "Retrieving play history" );
    bool result = query.exec();
    T::logger()->log( "retrievePlayHistory", query );
    if( result ) {
        while( query.next() ) {
            quint64 mills   = query.value( 0 ).toULongLong();
            QString trackId = query.value( 1 ).toString();
            recentIds.append( new QPair< QString, QDateTime >(
                                  trackId,
                                  QDateTime::fromMSecsSinceEpoch( mills )));
        }
        T::logger()->info( "History retrieval successful" );
        return true;
    }
    T::logger()->error( "Play history retrieval failed." );
    return false;
}


bool StdDataRetriever::getTrackIdsFromArtist( QString artistName,
                                              QSet< QString > &trackIds )
{
    QSqlQuery query;
    query.prepare( "SELECT trackId FROM AudioLibrary WHERE artist = ?");
    query.addBindValue( artistName );
    T::logger()->info( QString( "Getting track from artist \"%1\"" )
                       .arg( artistName ));
    bool result = query.exec();
    T::logger()->log( "tracksForArtist", query );
    if( result ) {
        while( query.next() ) {
            trackIds.insert( query.value( 0 ).toString() );
        }
        T::logger()->info(
                    QString(" Tracks for artist \"%1\" retrieved successfully")
                    .arg( artistName ));
        return true;
    }
    T::logger()->error( QString(" Track retrieval for artist for "
                                "\"%1\" failed." )
                   .arg( artistName ));
    return false;
}


bool StdDataRetriever::getAlbumIdsFromArtist( QString artistName,
                                                   QSet< QString > &albumIds )
{
    QSqlQuery query;
    query.prepare( "SELECT album FROM AudioLibrary WHERE artist = ?");
    query.addBindValue( artistName );
    T::logger()->info( QString( "Getting albums from artist \"%1\"" )
                       .arg( artistName ));
    bool result = query.exec();
    T::logger()->log( "albumsForArtist", query );
    if( result ) {
        while( query.next() ) {
            albumIds.insert( Data::AlbumInfo::getAlbumIdFrom( artistName,
                                 query.value( 0 ).toString() ));
        }
        T::logger()->info(
                    QString(" Albums for artist \"%1\" retrieved successfully")
                    .arg( artistName ));
        return true;
    }
    T::logger()->error( QString(" Albums retrieval for artist \"%1\" failed." )
                   .arg( artistName ));
    return false;
}


bool StdDataRetriever::getTrackIdsFromAlbum( QString artistName,
                                                  QString albumName,
                                                  QSet< QString > &trackIds )
{
    QSqlQuery query;
    query.prepare( "SELECT trackId FROM AudioLibrary "
                   "WHERE artist = ? AND album = ?");
    query.addBindValue( artistName );
    query.addBindValue( albumName );
    T::logger()->info( QString( "Getting tracks from "
                                "album \"%1\" of artist \"%2\"" )
                       .arg( albumName ).arg( artistName ));
    bool result = query.exec();
    T::logger()->log( "tracksForAlbum", query );
    if( result ) {
        while( query.next() ) {
            trackIds.insert( query.value( 0 ).toString() );
        }
        T::logger()->info(
                    QString(" Tracks from album \"%1\""
                            " of artist \"%2\" retrieved successfully" )
                    .arg( albumName ).arg( artistName ));
        return true;
    }
    T::logger()->error( QString(" Track retrieval from album \"%1\" "
                                "of artist \"%2\" failed." )
                   .arg( albumName ).arg( artistName ));
    return false;
}


bool StdDataRetriever::getAllArtists(
        QHash< QString, Data::ArtistInfo *> &artists )
{
    QSqlQuery query;
    query.prepare( "SELECT * FROM ArtistInfo");
    T::logger()->info( "Getting list of artists" );
    bool result = query.exec();
    T::logger()->log( "allArtists", query );
    if( result ) {
        while( query.next() ) {
            QString name    = query.value( 0 ).toString();
            QString comment = query.value( 1 ).toString();
            int rating      = query.value( 2 ).toInt();
            int playCount   = query.value( 3 ).toInt();
            QString tags    = query.value( 4 ).toString();
            Data::ArtistInfo *artist = new Data::ArtistInfo( name );
            artist->setComment( comment );
            artist->setPlayCount( playCount );
            artist->setRating( static_cast< Data::MediaRating >( rating ));
            addTags( artist, tags );
            artists.insert( name, artist );
        }
        T::logger()->info( " Artist information retrieved successfuly. ");
        return true;
    }
    T::logger()->error( "Artist information retrieval failed" );
    return false;
}


bool StdDataRetriever::getAllAlbums(
        QHash< QString, Data::AlbumInfo *> &albums)
{
    QSqlQuery query;
    query.prepare( "SELECT * FROM AlbumInfo");
    T::logger()->info( "Getting list of albums" );
    bool result = query.exec();
    T::logger()->log( "allAlbums", query );
    if( result ) {
        while( query.next() ) {
            QString artist  = query.value( 1 ).toString();
            QString name    = query.value( 2 ).toString();
            QString comment = query.value( 3 ).toString();
            int rating      = query.value( 4 ).toInt();
            int playCount   = query.value( 5 ).toInt();
            QString tags    = query.value( 6 ).toString();
            Data::AlbumInfo *album = new Data::AlbumInfo( artist, name );
            album->setComment( comment );
            album->setPlayCount( playCount );
            album->setRating( static_cast< Data::MediaRating >( rating ));
            addTags( album, tags );
            albums.insert( album->id(), album );
        }
        T::logger()->info( " Album information retrieved successfuly. ");
        return true;
    }
    T::logger()->error( "Album information retrieval failed" );
    return false;
}


bool StdDataRetriever::getPlaylists(
        QHash< QString, Data::SavedPlaylist *> &playlists)
{
    QSqlQuery query;
    query.prepare( "SELECT * FROM SavedPlaylist");
    T::logger()->info( "Getting list of saved playlists" );
    bool result = query.exec();
    T::logger()->log( "allPlaylists", query );
    if( result ) {
        while( query.next() ) {
            QString name    = query.value( 0 ).toString();
            QString desc	= query.value( 1 ).toString();
            quint64 cTime   = query.value( 2).toULongLong();
            quint64 mTime   = query.value( 3 ).toULongLong();
            int     rating  = query.value( 4 ).toInt();
            int playCount   = query.value( 5 ).toInt();
            QString  tags   = query.value( 6 ).toString();
            Data::SavedPlaylist *plist = new Data::SavedPlaylist(
                        name, QDateTime::fromMSecsSinceEpoch( cTime ));
            plist->setDescription( desc );
            plist->setModifiedTime( QDateTime::fromMSecsSinceEpoch( mTime ));
            plist->setRating( static_cast< Data::MediaRating >( rating ));
            plist->setPlayCount( playCount );
            addTags( plist, tags );
            playlists.insert( plist->name(), plist );
        }
        T::logger()->info( " Playlist information retrieved successfuly. ");
        return true;
    }
    T::logger()->error( "Playlist information retrieval failed" );
    return false;
}



bool StdDataRetriever::getPlaylistContent( QString playlistName,
                                                 QList< QString > &trackIds)
{
    T::logger()->debug( QString( "Retrieving saved playlist %1" )
                        .arg( playlistName ));
    QFile plFile( FileSystemUtil::getPlaylistPath(
                      playlistName.append( ".tpls" )));
    if( plFile.exists() ) {
        plFile.open( QFile::ReadOnly );
        QTextStream plStream( &plFile );
        while( ! plStream.atEnd() ) {
            QString entry =  plStream.readLine( 600 );
            trackIds.append( entry );
        }
    }
    plFile.close();
    T::logger()->debug( QString( "%1 tracks loaded" ).arg( trackIds.size() ));
    return true;
}


int StdDataRetriever::getPlayCount( QString trackId )
{
    QSqlQuery query;
    query.prepare( "SELECT playCount FROM AudioLibrary WHERE trackId = ?" );
    query.addBindValue( trackId );
    T::logger()->info( QString("Getting playCount for track with id \"%1\"")
                       .arg( trackId ));
    bool result = query.exec();
    T::logger()->log( "playCountForTrack", query );
    if( result ) {
        T::logger()->info( "Play count retrieval successful" )    ;
        return query.value( 0 ).toInt();
    }
    T::logger()->error( QString( "Play count retrieval for track with id \"%1\""
                                 " failed" ).arg( trackId ));
    return -1;
}


Data::MediaRating StdDataRetriever::getRating( QString trackId )
{
    QSqlQuery query;
    query.prepare( "SELECT rating FROM AudioLibrary WHERE trackId = ?" );
    query.addBindValue( trackId );
    T::logger()->info( QString("Getting rating for track with id \"%1\"")
                       .arg( trackId ));
    bool result = query.exec();
    T::logger()->log( "ratingForTrack", query );
    if( result ) {
        T::logger()->info( "rating retrieval successful" )    ;
        return static_cast< Data::MediaRating >( query.value( 0 ).toInt() );
    }
    T::logger()->error( QString( "rating retrieval for track with id \"%1\""
                                 "failed" ).arg( trackId ));
    return Data::Rating_None;
}


bool StdDataRetriever::getProperty( QString key,
                                    QString &typeOut,
                                    QString &valueOut )
{
    QSqlQuery query( "SELECT * FROM PropertyStore" );
    T::logger()->debug( QString( "Retrieving property with key %1" ).arg(key ));
    bool result = query.exec();
    T::logger()->log( "retrievingProperty", query );
    if( result ) {
        T::logger()->info( QString( "Retrieved property %1 successfully" )
                           .arg( key ));
        typeOut = query.value( 1 ).toString();
        valueOut = query.value( 2 ).toString();
    }
    else {
        T::logger()->error( QString( "Error while retrieving property %1")
                            .arg( key ));
    }
    return result;
}


int StdDataRetriever::getSavedPlayQueue(
        QList< QPair< QString, QString > > &trackInfoOut )
{
    T::logger()->debug( "Retrieving saved current playlist" );
    QFile plFile( FileSystemUtil::getPlaylistPath( "#current.tpls" ));
    int numFound = 0;
    if( plFile.exists() ) {
        plFile.open( QFile::ReadOnly );
        QTextStream plStream( &plFile );
        while( ! plStream.atEnd() ) {
            QString entry =  plStream.readLine( 600 );
            QStringList entries = entry.split( "|" );
            if( entries.size() >= 2 ) {
                ++ numFound;
                trackInfoOut.append( QPair< QString, QString >( entries[ 0 ],
                                                                entries[ 1 ] ));
            }
        }
    }
    plFile.close();
    T::logger()->debug( QString( "%1 tracks loaded" ).arg(
                            trackInfoOut.size() ));
    return numFound;
}


void StdDataRetriever::addTags(Data::StoredMediaObject *obj,
                                     QString tags )
{
    if( tags.trimmed().size() ) {
        QStringList list = tags.split( ',' );
        foreach( QString tag, list ) {
            obj->addTag( tag.trimmed() );
        }
    }
}


} } //end of namespaces
