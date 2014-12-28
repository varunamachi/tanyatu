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

#include <QVariant>
#include <QtSql/QSqlQuery>
#include <QTextStream>
#include <QFile>

#include "StdDataUpdater.h"
#include "../T.h"
#include "../data/AlbumInfo.h"
#include "../data/ArtistInfo.h"
#include "../data/StoredVideo.h"
#include "../coreutils/FileSystemUtil.h"

namespace Tanyatu { namespace Impl {


bool StdDataUpdater::add( Data::StoredObject *stored )
{
    if( ! stored ) {
        T::logger()->error( "add: NULL value given" );
        return false;
    }
    if( stored->storageId() == Data::StoredAudio::STORAGE_ID ) {
        return add( dynamic_cast< Data::StoredAudio *>( stored ));
    }
    else if( stored->storageId() == Data::StoredVideo::STORAGE_ID ) {
        return add( dynamic_cast< Data::StoredVideo *>( stored ));
    }
    else if( stored->storageId() == Data::ArtistInfo::STORAGE_ID ) {
        return add( dynamic_cast< Data::ArtistInfo *>( stored ));
    }
    else if( stored->storageId() == Data::AlbumInfo::STORAGE_ID ) {
        return add( dynamic_cast< Data::AlbumInfo *>( stored ));
    }
    else if( stored->storageId() == Data::SavedPlaylist::STORAGE_ID ) {
        return add( dynamic_cast< Data::SavedPlaylist *>( stored ));
    }
    T::logger()->error( QString( "Error while adding object to DB, "
                                 "Invalid storage type \"%1\" given" )
                        .arg( stored->storageId() ));
    return false;

}


bool StdDataUpdater::update(
        Data::StoredObject *stored )
{
    if( ! stored ) {
        T::logger()->error( "update: NULL value given" );
        return false;
    }
    if( stored->storageId() == Data::StoredAudio::STORAGE_ID ) {
        return update( dynamic_cast< Data::StoredAudio *>( stored ));
    }
    else if( stored->storageId() == Data::StoredVideo::STORAGE_ID ) {
        return update( dynamic_cast< Data::StoredVideo *>( stored ));
    }
    else if( stored->storageId() == Data::ArtistInfo::STORAGE_ID ) {
        return update( dynamic_cast< Data::ArtistInfo *>( stored ));
    }
    else if( stored->storageId() == Data::AlbumInfo::STORAGE_ID ) {
        return update( dynamic_cast< Data::AlbumInfo *>( stored ));
    }
    else if( stored->storageId() == Data::SavedPlaylist::STORAGE_ID ) {
        return update( dynamic_cast< Data::SavedPlaylist *>( stored ));
    }
    T::logger()->error( QString( "Error while updating object in the DB, "
                                 "Invalid storage type \"%1\" given" )
                        .arg( stored->storageId() ));
    return false;
}


bool StdDataUpdater::remove(Data::StoredObject *stored)
{
    if( ! stored ) {
        T::logger()->error( "remove: NULL value given" );
        return false;
    }
    if( stored->storageId() == Data::StoredAudio::STORAGE_ID ) {
        return removeAudioTrack( stored->id() );
    }
    else if( stored->storageId() == Data::StoredVideo::STORAGE_ID ) {
        return removeVideoTrack( stored->id() );
    }
    else if( stored->storageId() == Data::ArtistInfo::STORAGE_ID ) {
        return removeArtist( stored->id() );
    }
    else if( stored->storageId() == Data::AlbumInfo::STORAGE_ID ) {
        return removeAlbum( stored->id() );
    }
    else if( stored->storageId() == Data::SavedPlaylist::STORAGE_ID ) {
        return removePlaylist( stored->id() );
    }
    T::logger()->error( QString( "Error while removing object from the DB, "
                                 "Invalid storage type \"%1\" given" )
                        .arg( stored->storageId() ));
    return false;
}


bool StdDataUpdater::add( QList< Data::StoredObject *> &objList )
{
    quint32 failCount = 0;
    T::logger()->debug( QString( "Adding \"%1\" stored objects" )
                        .arg( objList.size() ));
    foreach( Data::StoredObject *stored, objList ) {
        if( ! add( stored ) ) {
            ++ failCount;
        }
    }
    if( ! failCount ) {
        T::logger()->info( QString( "Added \"%1\" stored objects" )
                           .arg( objList.size() ));
    }
    else {
        T::logger()->error( QString( "Could not add \"%1\" objects out of "
                                     "given \"%2\" objects")
                            .arg( failCount ).arg( objList.size() ));
    }
    return ! failCount;
}


bool StdDataUpdater::update( QList< Data::StoredObject *> &objList )
{
    quint32 failCount = 0;
    T::logger()->debug( QString( "Updating \"%1\" stored objects" ));
    foreach( Data::StoredObject *stored, objList ) {
        if( ! update( stored ) ) {
            ++ failCount;
        }
    }
    if( ! failCount ) {
        T::logger()->info( QString( "Updated \"%1\" stored objects" )
                           .arg( objList.size() ));
    }
    else {
        T::logger()->error( QString( "Could not update \"%1\" objects out of "
                                     "given \"%2\" objects")
                            .arg( failCount ).arg( objList.size() ));
    }
    return ! failCount;
}

bool StdDataUpdater::remove( QList< Data::StoredObject *> &objList )
{
    quint32 failCount = 0;
    T::logger()->debug( QString( "Removing \"%1\" stored objects" ));
    foreach( Data::StoredObject *stored, objList ) {
        if( ! remove( stored ) ) {
            ++ failCount;
        }
    }
    if( ! failCount ) {
        T::logger()->info( QString( "Removed \"%1\" stored objects" )
                           .arg( objList.size() ));
    }
    else {
        T::logger()->error( QString( "Could not remove \"%1\" objects out of "
                                     "given \"%2\" objects")
                            .arg( failCount ).arg( objList.size() ));
    }
    return ! failCount;
}






bool StdDataUpdater::addHistory( QString itemId, QDateTime time )
{
    QSqlQuery query;
    query.prepare( "INSERT INTO PlayHistory ("
                   "   lastPlayedTime,"
                   "   trackId"
                   ")"
                   "VALUES( ?, ? );" );
    T::logger()->debug( QString( "Adding item \"%1\" to history" )
                       .arg( itemId ));
    query.addBindValue( time.toMSecsSinceEpoch() );
    query.addBindValue( itemId );
    bool result = query.exec();
    T::logger()->log( "addToHistory", query );
    if( result ) {
        T::logger()->debug( QString( "Added track \"%1\" to history ")
                            .arg( itemId ));
    }
    else {
        T::logger()->error( QString( "Error while adding track \"%1\" to "
                                     "history").arg( itemId ));
    }
    return result;

}


bool StdDataUpdater::setProperty( QString key, QString type, QString value )
{
    bool result = false;
    QSqlQuery query;
    query.prepare( "REPLACE INTO PropertyStore ("
                   "    key,"
                   "    type,"
                   "    value )"
                   " VALUES( ?, ?, ? );" );
    T::logger()->debug( QString( "Adding property \"%1\"" ).arg( key ));
    query.addBindValue( key );
    query.addBindValue( type );
    query.addBindValue( value );
    result = query.exec();
    if( result ) {
        T::get()->logger()->debug( QString( "Added property [%1 = %2]" )
                                   .arg( key )
                                   .arg( value ));
    }
    else {
        T::get()->logger()->debug( QString( "Error while adding property [%1]" )
                                   .arg( key ));
    }
    return result;
}

bool StdDataUpdater::removeProperty(QString key)
{
    QSqlQuery query( QString( "DELETE FROM PropertyStore WHERE key = " )
                     + key );
    T::get()->logger()->debug( QString( "Removing property [%1]").arg( key ));
    bool result = query.exec();
    if( result ) {
        T::get()->logger()->info( QString( "Property [%1] removed").arg( key ));
    }
    else {
        T::get()->logger()->info( QString( "Could not remove property [%1]")
                                  .arg( key ));
    }
    return result;
}


bool StdDataUpdater::clearAudioLibrary()
{
    QSqlQuery query( "DELETE FROM AudioLibrary;" );
    QSqlQuery query1( "DELETE FROM PlaylistContent");
    QSqlQuery query2( "DELETE FROM PlayHistory" );
    QSqlQuery query3( "DELETE FROM ArtistInfo" );
    QSqlQuery query4( "DELETE FROM AlbumInfo" );
    QSqlQuery query5( "DELETE FROM SavedPlaylist" );
    T::logger()->debug( "Clearing audio library" );
    bool result = query.exec()
            && query1.exec()
            && query2.exec()
            && query3.exec()
            && query4.exec()
            && query5.exec();
    T::logger()->log( "clearAudioLibrary_LibItself", query );
    T::logger()->log( "clearAudioLibrary_Playlist_Content", query1 );
    T::logger()->log( "clearAudioLibrary_History", query2 );
    T::logger()->log( "clearAudioLibrary_Artist_Info", query3 );
    T::logger()->log( "clearAudioLibrary_Album_Info", query4 );
    T::logger()->log( "clearAudioLibrary_Saved_playlist", query5 );
    if( result ) {
        T::logger()->info( "Audio library successfully cleared" );
    }
    else {
        T::logger()->error( "Error while clearing the audio library" );
    }
    return result;
}


bool StdDataUpdater::clearVideoLibrary()
{
    QSqlQuery query( "DELETE FROM VideoLibrary;" );
    T::logger()->debug( "Clearing video library" );
    bool result = query.exec();
    T::logger()->log( "clearVideoLibrary", query );
    if( result ) {
        T::logger()->info( "Video library successfully cleared" );
    }
    else {
        T::logger()->error( "Error while clearing the video library" );
    }
    return result;
}


bool StdDataUpdater::clearHistory()
{
    QSqlQuery query( "DELETE FROM PlayHistory" );
    T::logger()->debug( "Clearing play history" );
    bool result = query.exec();
    T::logger()->log( "clearHistory", query );
    if( result ) {
        T::logger()->info( "Play history successfully cleared" );
    }
    else {
        T::logger()->error( "Error while clearing the play history" );
    }
    return result;
}


bool StdDataUpdater::clearHistory( QDateTime &from )
{
    QSqlQuery query;
    query.prepare( "DELETE FROM PlayHistory "
                   "WHERE lastPlayedTime <= ?" );
    T::logger()->debug( QString( "Clearing play history from \"%1\"")
                        .arg( from.toString() ));
    query.addBindValue( from.toMSecsSinceEpoch() );
    bool result = query.exec();
    T::logger()->log( "clearHistory", query );
    if( result ) {
        T::logger()->info( QString( "Cleared play history from \"%1\"")
                           .arg( from.toString() ));
    }
    else {
        T::logger()->error( QString( "Error while clearing play history from "
                                     "\"%1\"").arg( from.toString() ));
    }
    return result;

}


bool StdDataUpdater::removeAudioTracks( QList< QString > &trackIds )
{
    bool result = true;
    T::logger()->debug( QString( "Removing %1 audio tracks from library" )
                       .arg( trackIds.size() ));
    foreach( QString id, trackIds ) {
        result &= removeAudioTrack( id );
    }
    if( result ) {
        T::logger()->info( QString( "Removing %1 audio tracks from library" )
                           .arg( trackIds.size() ));
    }
    else {
        T::logger()->error( "Error occured while removing audio tracks from "
                           "library" );
    }
    return result;
}


bool StdDataUpdater::removeVideoTracks( QList< QString > &trackIds )
{
    bool result = true;
    T::logger()->debug( QString( "Removing %1 video tracks from library" )
                       .arg( trackIds.size() ));
    foreach( QString id, trackIds ) {
        result &= removeVideoTrack( id );
    }
    if( result ) {
        T::logger()->info( QString( "Removing %1 video tracks from library" )
                           .arg( trackIds.size() ));
    }
    else {
        T::logger()->error( "Error occured while removing video tracks from "
                           "library" );
    }
    return result;
}


bool StdDataUpdater::removeAudioTrack( QString trackId )
{
    QSqlQuery query;
    query.prepare( "DELETE FROM AudioLibrary "
                   "WHERE trackId = ?;" );

    T::logger()->debug( QString( "Removing audio track with trackId \"%1\"")
                        .arg( trackId ));
    query.addBindValue( trackId );
    bool result = query.exec();
    T::logger()->log( "removeAudioTrack", query );
    if( result ) {
        T::logger()->debug( QString( "Removed audio track with id \"%1\"" )
                            .arg( trackId ));
    }
    else {
        T::logger()->error( QString( "Error while removing audio track "
                                     "with id \"%1\"").arg( trackId ));
    }
    return result;
}


bool StdDataUpdater::removeVideoTrack( QString trackId )
{
    QSqlQuery query;
    query.prepare( "DELETE FROM VideoLibrary "
                   "WHERE vidId = ?;" );
    T::logger()->debug( QString( "Removing video track with trackId \"%1\"")
                        .arg( trackId ));
    query.addBindValue( trackId );
    bool result = query.exec();
    T::logger()->log( "removeVideoTrack", query );
    if( result ) {
        T::logger()->debug( QString( "Removed video track with id \"%1\"" )
                            .arg( trackId ));
    }
    else {
        T::logger()->error( QString( "Error while removing video track "
                                     "with id \"%1\"").arg( trackId ));
    }
    return result;
}


//bool StandardDataUpdater::removeTrackRefs( QString trackId ) {
//    QSqlQuery plmRemQuery, hmRemQuery;

//    hmRemQuery.prepare( "DELETE FROM PlayHistory "
//                        "WHERE trackId = ?" );
//    plmRemQuery.addBindValue( trackId );
//    hmRemQuery.addBindValue( trackId );
//    bool result = plmRemQuery.exec() && hmRemQuery.exec();
//    T::logger()->log( "removeTrackFromSavedPlaylist", plmRemQuery );
//    T::logger()->log( "removeTrackFromPlayHistory", hmRemQuery );
//    return result;
//}




bool StdDataUpdater::removeArtist( QString artistName )
{
    QSqlQuery query;
    query.prepare( "DELETE FROM ArtistInfo "
                   "WHERE name = ?;" );
    T::logger()->debug( QString( "Removing artist \"%1\"" )
                        .arg( artistName ));
    query.addBindValue( artistName );
    bool result = query.exec();
    T::logger()->log( "removeArtist", query );
    if( result ) {
        T::logger()->debug( QString( "Removed artist \"%1\"")
                            .arg( artistName ));
    }
    else {
        T::logger()->error( QString( "Error while removing artist \"%1\"")
                            .arg( artistName ));
    }
    return result;
}


bool StdDataUpdater::removeAlbum( QString albumId )
{
    QSqlQuery query;
    query.prepare( "DELETE FROM AlbumInfo "
                   "WHERE albumId = ?;" );
    T::logger()->debug( QString( "Removing album with id\"%1\"" )
                        .arg( albumId ));
    query.addBindValue( albumId );
    bool result = query.exec();
    T::logger()->log( "removeAlbum", query );
    if( result ) {
        T::logger()->debug( QString( "Removed album with id \"%1\"" )
                            .arg( albumId ));
    }
    else {
        T::logger()->error( QString( "Error while removing album with id "
                                     "\"%1\"").arg( albumId ));
    }
    return result;
}


bool StdDataUpdater::removeHistoryItem( QString trackId )
{
    QSqlQuery query;
    query.prepare( "DELETE FROM PlayHistory "
                   "WHERE trackId = ?;" );
    T::logger()->debug( QString( "Removing history item with id\"%1\"" )
                        .arg( trackId ));
    query.addBindValue( trackId );
    bool result = query.exec();
    T::logger()->log( "removeHistoryItem", query );
    if( result ) {
        T::logger()->debug( QString( "Removed history item with id \"%1\"" )
                            .arg( trackId ));
    }
    else {
        T::logger()->error( QString( "Error while removing history item with "
                                     "id \"%1\"").arg( trackId ));
    }
    return result;
}


bool StdDataUpdater::removeFromHistory( QList< QString > &trackIds )
{
    bool result = true;
    foreach( QString id, trackIds ) {
        result &= removeHistoryItem( id );
    }
    return result;
}


bool StdDataUpdater::add( Data::StoredAudio *audio )
{
    QSqlQuery query;
    query.prepare( "INSERT INTO AudioLibrary ("
                   "   trackId,"
                   "   url,"
                   "   title,"
                   "   artist,"
                   "   album ,"
                   "   cdate,"
                   "   genre ,"
                   "   description ,"
                   "   trackNumber,"
                   "   duration,"
                   "   bitRate,"
                   "   sampleRate,"
                   "   channels ,"
                   "   rating,"
                   "   playCount,"
                   "   tags )"
                   "VALUES ( ?, ?, ?, ?,"
                   "         ?, ?, ?, ?,"
                   "         ?, ?, ?, ?,"
                   "         ?, ?, ?, ? );" );
    T::logger()->debug( QString( "Adding audio track at %1 to library").arg(
                            audio->url().toString() ));
    query.addBindValue( audio->trackId() );
    query.addBindValue( audio->url() );
    query.addBindValue( audio->title() );
    query.addBindValue( audio->artist() );
    query.addBindValue( audio->album() );
    query.addBindValue( audio->date() );
    query.addBindValue( audio->genre() );
    query.addBindValue( audio->descreption() );
    query.addBindValue( audio->trackNumber() );
    query.addBindValue( audio->duration() );
    query.addBindValue( audio->bitRate() );
    query.addBindValue( audio->sampleRate() );
    query.addBindValue( audio->channels() );
    query.addBindValue( static_cast< int >( audio->rating() ));
    query.addBindValue( audio->playCount() );
    query.addBindValue( getTagString( audio->tags() ));
    bool result = query.exec();
    T::logger()->log( "addToAudioLib", query );
    if( result ) {
        T::logger()->debug( QString( "Audio track %1 is successfully added to "
                                    "library" ).arg( audio->url().toString() ));
    }
    else {
        T::logger()->error( QString("Error while adding audio track %1 to "
                         "library" ).arg( audio->url().toString() ));
    }
   return result;
}


bool StdDataUpdater::add( Data::StoredVideo *video )
{
    QSqlQuery query;
    query.prepare( "INSERT INTO VideoLibrary ("
                   "   vidId,"
                   "   url,"
                   "   name ,"
                   "   category,"
                   "   year,"
                   "   duration,"
                   "   descreption,"
                   "   creator,"
                   "   artistsRoles	,"
                   "   rating,"
                   "   playCount,"
                   "   tags )"
                   "VALUES ( ?, ?, ?, ?,"
                   "	 ?, ?, ?, ?,"
                   "	 ?, ?, ?, ? );");
    T::logger()->debug( QString( "Adding video track % 1 to library" ).arg(
                            video->url().toString() ));
    query.addBindValue( video->id() );
    query.addBindValue( video->url() );
    query.addBindValue( video->name() );
    query.addBindValue( video->category() );
    query.addBindValue( video->year() );
    query.addBindValue( video->duration() );
    query.addBindValue( video->description() );
    query.addBindValue( video->creator() );

    /**
     * @todo encode the values in the map to database. LATER....
     */
    query.addBindValue( "" );
    query.addBindValue( static_cast< int >( video->rating() ));
    query.addBindValue( video->playCount() );
    query.addBindValue( getTagString( video->tags() ));
    bool result = query.exec();
    T::logger()->log( "addToVideoLib", query );

    if( result ) {
        T::logger()->debug( QString( "Video track %1 is successfully added to "
                                    "library" ).arg( video->url().toString() ));
    }
    else {
        T::logger()->error( QString("Error while adding video track %1 to "
                         "library" ).arg( video->url().toString() ));
    }
    return result;
}


bool StdDataUpdater::add( Data::ArtistInfo *artist )
{
    QSqlQuery query;
    query.prepare( "INSERT INTO ArtistInfo ("
                   "   name,"
                   "   comment,"
                   "   rating,"
                   "   playCount,"
                   "   tags"
                   ")"
                   "VALUES ( ?, ?, ?,"
                   "         ?, ? );"
                   );
    T::logger()->debug( QString( "Adding artist \"%1\"to database" )
                       .arg( artist->name() ));
    query.addBindValue( artist->name() );
    query.addBindValue( artist->comment() );
    query.addBindValue( static_cast< int >( artist->rating() ));
    query.addBindValue( artist->playCount() );
    query.addBindValue( getTagString( artist->tags() ));
    bool result   = query.exec();
    T::logger()->log( "addArtist", query );
    if( result ) {
        T::logger()->debug( QString( "artist \"%1\" is successfully added to "
                                    "database" ).arg( artist->name() ));
    }
    else {
        T::logger()->error( QString("Error while adding artist \"%1\" to "
                         "database" ).arg( artist->name() ));
    }
    return result;
}


bool StdDataUpdater::add( Data::AlbumInfo *album )
{
    QSqlQuery query;
    query.prepare( "INSERT INTO AlbumInfo ("
                   "   albumId,"
                   "   artistName,"
                   "   name,"
                   "   comment,"
                   "   rating,"
                   "   playCount,"
                   "   tags"
                   ") "
                   "VALUES ( ?, ?, ?,"
                   "         ?, ?, ?, ? );"
                   );
    T::logger()->debug( QString( "Adding album \"%1\" from \"%2\" to database")
                       .arg( album->name() ).arg( album->artistName() ));
    query.addBindValue( album->id() );
    query.addBindValue( album->artistName() );
    query.addBindValue( album->name() );
    query.addBindValue( album->comment() );
    query.addBindValue( static_cast< int >( album->rating() ));
    query.addBindValue( static_cast< int >(album->playCount() ));
    query.addBindValue( getTagString( album->tags() ));

    bool result = query.exec();
    T::logger()->log( "addAlbum", query );
    if( result ) {
        T::logger()->debug( QString( "Album \"%1\" from \"%2\" is successfully "
                           "added to database" )
                           .arg( album->name() ).arg(album->artistName() ));
    }
    else {
        T::logger()->error( QString("Error while adding album \"%1\" from "
                         "\"%2\" to database").arg( album->name() )
                            .arg( album->artistName() ));
    }

    return result;
}





bool StdDataUpdater::update( Data::StoredAudio *audio )
{
    QSqlQuery query;
    query.prepare( "UPDATE AudioLibrary "
                   "SET title       = ?,"
                   "    artist      = ?,"
                   "    album       = ?,"
                   "    cdate       = ?,"
                   "    genre       = ?,"
                   "    description = ?,"
                   "    trackNumber = ?,"
                   "    duration    = ?,"
                   "    bitRate     = ?,"
                   "    sampleRate  = ?,"
                   "    channels    = ?,"
                   "    rating      = ?,"
                   "    playCount   = ?,"
                   "    tags        = ?"
                   "WHERE trackId = ?;" );
    T::logger()->debug( QString( "Updating track \"%1\"")
                        .arg( audio->trackId()) );
    query.addBindValue( audio->title() );
    query.addBindValue( audio->artist() );
    query.addBindValue( audio->album() );
    query.addBindValue( audio->date() );
    query.addBindValue( audio->genre() );
    query.addBindValue( audio->descreption() );
    query.addBindValue( audio->trackNumber() );
    query.addBindValue( audio->duration() );
    query.addBindValue( audio->bitRate() );
    query.addBindValue( audio->sampleRate() );
    query.addBindValue( audio->channels() );
    query.addBindValue( static_cast< int >( audio->rating() ));
    query.addBindValue( audio->playCount() );
    query.addBindValue( getTagString( audio->tags() ));
    query.addBindValue( audio->trackId() );
    bool result = query.exec();
    T::logger()->log( "updateAudioLib", query );
    if( result ) {
        T::logger()->debug( QString( "Updated track \"%1\"" )
                            .arg( audio->trackId()));
    }
    else {
        T::logger()->error( QString( "Error while updating track \"%1\"")
                            .arg( audio->trackId() ));
    }
    return result;
}

bool StdDataUpdater::update( Data::StoredVideo *video )
{
    QSqlQuery query;
    query.prepare( "UPDATE VideoLibrary "
                   "SET name	= ?,"
                   "    category	= ?,"
                   "    year	= ?,"
                   "    duration	= ?,"
                   "    descreption = ?,"
                   "    creator	= ?,"
                   "    artistsRoles= ?, "
                   "    rating	= ?,"
                   "    playCount	= ?,"
                   "    tags	= ?"
                   "WHERE vidId = ?;" );
    T::logger()->debug( QString( "Updating video track \"%1\"" ).arg(
                            video->id() ));
    query.addBindValue( video->name() );
    query.addBindValue( video->category() );
    query.addBindValue( video->year() );
    query.addBindValue( video->duration() );
    query.addBindValue( video->description() );
    query.addBindValue( video->creator() );

    /**
     * @todo encode the values in the map to database. LATER....
     */
    query.addBindValue( "" );
    query.addBindValue( static_cast< int >( video->rating() ));
    query.addBindValue( video->playCount() );
    query.addBindValue( getTagString( video->tags() ));
    bool result = query.exec();
    T::logger()->log( "updateVideoTrack", query );

    if( result ) {
        T::logger()->debug( QString( "Updated Video track \"%1\"")
                           .arg( video->id() ));
    }
    else {
        T::logger()->error( QString("Error while updating video track \"%1\" ")
                            .arg( video->id() ));
    }
    return result;
}


bool StdDataUpdater::update( Data::SavedPlaylist *playlist )
{
    QSqlQuery query;
    query.prepare( "UPDATE SavedPlaylist "
                   "SET description	 = ?,"
                   "    creationTime = ?,"
                   "    modifiedTime = ?,"
                   "    rating	 = ?,"
                   "    playCount 	 = ?,"
                   "    tags	 = ?"
                   "WHERE name = ?;" );
    T::logger()->debug( QString( "Updating playlist \"%1\"" )
                       .arg( playlist->name() ));
    query.addBindValue( playlist->description() );
    query.addBindValue( playlist->creationTime().toMSecsSinceEpoch() );
    query.addBindValue( playlist->modifiedTime().toMSecsSinceEpoch() );
    query.addBindValue( static_cast< int >( playlist->rating() ));
    query.addBindValue( playlist->playCount() );
    query.addBindValue( getTagString( playlist->tags() ));
    bool result = query.exec();
    T::logger()->log( "updatePlaylistInfo", query );
    if( result ) {
        T::logger()->debug( QString( "Updated playlist \"%1\"")
                            .arg( playlist->name() ));
    }
    else {
        T::logger()->error( QString("Error while updating playlist \"%1\"" )
                            .arg( playlist->name() ));
    }
    return result;
}


bool StdDataUpdater::update( Data::ArtistInfo *artist )
{
    QSqlQuery query;
    query.prepare( "UPDATE ArtistInfo "
                   "SET comment	= ?,"
                   "    rating	= ?,"
                   "    playCount = ?,"
                   "    tags	= ? "
                   "WHERE name = ?;" );
    T::logger()->debug( QString( "Updating artist information for \"%1\"" )
                       .arg( artist->name() ));
    query.addBindValue( artist->comment() );
    query.addBindValue( static_cast< int >( artist->rating() ));
    query.addBindValue( artist->playCount() );
    query.addBindValue( getTagString( artist->tags() ));
    query.addBindValue( artist->name() );
    bool result   = query.exec();
    T::logger()->log( "updateArtist", query );
    if( result ) {
        T::logger()->debug( QString( "updated artist information for \"%1\"" )
                                     .arg( artist->name() ));
    }
    else {
        T::logger()->error( QString("Error while updating info for artist "
                                    "\"%1\"").arg( artist->name() ));
    }
    return result;
}


bool StdDataUpdater::update( Data::AlbumInfo *album )
{
    QSqlQuery query;
    query.prepare( "UPDATE AlbumInfo "
                   "SET comment	= ?,"
                   "    rating	= ?,"
                   "    playCount 	= ?,"
                   "    tags	= ?"
                   " WHERE albumId = ?;" );
    T::logger()->debug( QString( "Updating album \"%1\" from \"%2\"")
                       .arg( album->name() ).arg( album->artistName() ));
    query.addBindValue( album->comment() );
    query.addBindValue( static_cast< int >( album->rating() ));
    query.addBindValue( album->playCount() );
    query.addBindValue( getTagString( album->tags() ));
    query.addBindValue( album->id() );
    bool result = query.exec();
    T::logger()->log( "updateAlbum", query );
    if( result ) {
        T::logger()->debug( QString( "Updated album \"%1\" from \"%2\"" )
                           .arg( album->name() ).arg(album->artistName() ));
    }
    else {
        T::logger()->error( QString("Error while updating album \"%1\" from "
                         "\"%2\"").arg( album->name() )
                            .arg( album->artistName() ));
    }

    return result;
}


bool StdDataUpdater::updateRating( Data::StoredMediaObject *stored )
{
    if( ! stored ) {
        T::logger()->error( "updateRating: NULL value given" );
        return false;
    }
    QString queryString( "UPDATE %1 "
                   "SET rating = ?"
                   "WHERE %2 = ?;" );
    QString tableName = getTableName( stored );
    queryString = queryString.arg( tableName ).arg(
                getUniqueColumnName( stored ));
    QSqlQuery query;
    query.prepare( queryString );
    T::logger()->debug( QString("Updating table %1 setting rating of object "
            "with id %2 to %3" ).arg( tableName ).arg( stored->id() )
             .arg( static_cast< int >( stored->rating() )));
    query.addBindValue( static_cast< int >( stored->rating() ));
    query.addBindValue( stored->id() );
    bool result = query.exec();
    T::logger()->log( "updateRating", query );
    if( result ) {
        T::logger()->debug( QString("Updated table %1 setting rating of "
                                    "object with id %2 to %3 successfully" )
                            .arg( tableName ).arg( stored->id() )
                            .arg( static_cast< int >( stored->rating() )));
    }
    else {
        T::logger()->debug( QString("Failed to update table %1 when setting "
                                    "rating of object with id %2 to %3" )
                            .arg( tableName ).arg( stored->id() )
                            .arg( static_cast< int >( stored->rating() )));
    }
    return result;
}


bool StdDataUpdater::updatePlayCount(
        Data::StoredMediaObject *stored )
{
    if( ! stored ) {
        T::logger()->error( "updatePlayCount: NULL value given" );
        return false;
    }
    QString queryString( "UPDATE %1 "
                   "SET playCount = ?"
                   "WHERE %2 = ?;" );
    QString tableName = getTableName( stored );
    queryString = queryString.arg( tableName ).arg(
                getUniqueColumnName( stored ));
    QSqlQuery query;
    query.prepare( queryString );
    T::logger()->debug( QString("Updating table %1 setting playCount of object "
            "with id %2 to %3" ).arg( tableName ).arg( stored->id() )
                        .arg( stored->playCount() ));
    query.addBindValue( stored->playCount() );
    query.addBindValue( stored->id() );
    bool result = query.exec();
    T::logger()->log( "updatePlayCount", query );
    if( result ) {
        T::logger()->debug( QString("Updated table %1 setting playCount of "
                                    "object with id %2 to %3 successfully" )
                            .arg( tableName ).arg( stored->id() )
                            .arg( stored->playCount() ));
    }
    else {
        T::logger()->debug( QString("Failed to update table %1 when setting "
                                    "playCount of object with id %2 to %3" )
                            .arg( tableName ).arg( stored->id() )
                            .arg( stored->playCount() ));
    }
    return result;
}


bool StdDataUpdater::updateTags( Data::StoredMediaObject *stored )
{
    if( ! stored ) {
        T::logger()->error( "updateTags: NULL value given" );
        return false;
    }
    QString queryString( "UPDATE %1 "
                   "SET tags = ?"
                   "WHERE %2 = ?;" );
    QString tagString = getTagString( stored->tags() );
    QString tableName = getTableName( stored );
    queryString = queryString.arg( tableName ).arg(
                getUniqueColumnName( stored ));
    QSqlQuery query;
    query.prepare( queryString );
    T::logger()->debug( QString("Updating table %1 setting tags of object "
            "with id %2 to %3" ).arg( tableName ).arg( stored->id() )
                        .arg( tagString ));
    query.addBindValue( tagString );
    query.addBindValue( stored->id() );
    bool result = query.exec();
    T::logger()->log( "updateTags", query );
    if( result ) {
        T::logger()->debug( QString("Updated table %1 setting tags of "
                                    "object with id %2 to %3 successfully" )
                            .arg( tableName ).arg( stored->id() )
                            .arg( tagString ));
    }
    else {
        T::logger()->debug( QString("Failed to update table %1 when setting "
                                    "tags of object with id %2 to %3" )
                            .arg( tableName ).arg( stored->id() )
                            .arg( tagString ));
    }
    return result;
}




bool StdDataUpdater::saveCurrentPlaylist(
        const QList<Data::MediaItem *> &list )
{
    QFile plFile( FileSystemUtil::getPlaylistPath( "#current.tpls" ));
    if( plFile.exists() ) {
        T::logger()->debug( QString( "Removing prev. playlist file %1" ).arg(
                                plFile.fileName() ));
        plFile.remove();
    }
    plFile.open( QFile::WriteOnly );
    T::logger()->debug( "Saving current playlist to file: current.tpls" );
    QTextStream playlistStream( &plFile );
    Data::AudioTrack *track = 0;
    foreach (Data::MediaItem *item, list) {
        if ( track == dynamic_cast< Data::AudioTrack *>( item )) {
            playlistStream << item->trackId()
                         << "|"
                         << item->url().toString()
                         << "\n";
        }
    }
    plFile.close();
    T::logger()->debug( "Saved current playlist to file: current.tpls" );
    return true;
}



bool StdDataUpdater::clearPlaylist( QString playlistName )
{
    QFile plFile( Tanyatu::FileSystemUtil::getPlaylistPath( playlistName ));
    return ( plFile.exists() && plFile.remove() );
}



bool StdDataUpdater::savePlaylist( QString playlistName,
                                   const QList< QString > &ids )
{
    QFile plFile( FileSystemUtil::getPlaylistPath(
                      playlistName.append( ".tpls" )));
    if( plFile.exists() ) {
        T::logger()->debug( QString( "Removing prev. playlist file " ).arg(
                                plFile.fileName() ));
        plFile.remove();
    }
    plFile.open( QFile::WriteOnly );
    T::logger()->debug( QString( "Saving playlist to file: %1" )
                        .arg( playlistName ));
    QTextStream playlistStream( &plFile );
    foreach ( QString id, ids ) {
        playlistStream << id << "\n";
    }
    plFile.close();
    T::logger()->debug( QString( "Saved playlist: %1" ).arg( playlistName ));
    return true;
}


bool StdDataUpdater::removePlaylist( QString playlistName )
{
    QSqlQuery plQuery;
    plQuery.prepare( "DELETE FROM SavedPlaylist "
                     "WHERE name = ?;" );
    plQuery.addBindValue( playlistName );

    T::logger()->debug( QString( "Removing playlist \"%1\"" )
                        .arg( playlistName ));
    QFile plFile( FileSystemUtil::getPlaylistPath(
                     playlistName.append( ".tpls" )));
    if( plFile.exists() ) {
        T::logger()->debug( QString( "Removing playlist file " ).arg(
                                plFile.fileName() ));
        plFile.remove();
    }
    bool result = plQuery.exec();
    T::logger()->log( "removePlaylist", plQuery );
    if( result ) {
        T::logger()->debug( QString( "Removed playlist \"%1\"" )
                            .arg( playlistName));
    }
    else {
        T::logger()->error( QString( "Error while deleting playlist \"%1\"")
                            .arg( playlistName ));
    }
    return result;
}


bool StdDataUpdater::add( Data::SavedPlaylist *playlist )
{
    QSqlQuery query;
    query.prepare( "INSERT INTO SavedPlaylist ("
                   "    name,"
                   "    description,"
                   "    creationTime,"
                   "    modifiedTime,"
                   "    rating,"
                   "    playCount,"
                   "    tags"
                   ")"
                   "VALUES( ?, ?, ?, ?,"
                   "        ?, ?, ? );"
                   );
    T::logger()->debug( QString( "Adding playlist \"%1\" to database" )
                       .arg( playlist->name() ));
    query.addBindValue( playlist->name() );
    query.addBindValue( playlist->description() );
    query.addBindValue( playlist->creationTime().toMSecsSinceEpoch() );
    query.addBindValue( playlist->modifiedTime().toMSecsSinceEpoch() );
    query.addBindValue( static_cast< int >( playlist->rating() ));
    query.addBindValue( playlist->playCount() );
    query.addBindValue( getTagString( playlist->tags() ));
    bool result = query.exec();
    T::logger()->log( "addPlaylistInfo", query );
    if( result ) {
        T::logger()->debug( QString( "Playlist \"%1\" is successfully added to "
                                    "database" ).arg( playlist->name() ));
    }
    else {

        T::logger()->error( QString("Error while adding playlist \"%1\" to "
                         "database" ).arg( playlist->name() ));
    }
    return result;
}




QString StdDataUpdater::getTableName( Data::StoredMediaObject *stored ) {
    if( stored->storageId() == Data::StoredAudio::STORAGE_ID ) {
        return "AudioLibrary";
    }
    else if( stored->storageId() == Data::StoredVideo::STORAGE_ID ) {
        return "VideoLibrary";
    }
    else if( stored->storageId() == Data::ArtistInfo::STORAGE_ID ) {
        return "ArtistInfo";
    }
    else if( stored->storageId() == Data::AlbumInfo::STORAGE_ID ) {
        return "AlbumInfo";
    }
    else if( stored->storageId() == Data::SavedPlaylist::STORAGE_ID ) {
        return "SavedPlaylist";
    }
    return "NO_SUITABLE_TABLE";
}


QString StdDataUpdater::getUniqueColumnName(
        Data::StoredMediaObject *stored ) {
    if( stored->storageId() == Data::StoredAudio::STORAGE_ID ) {
        return "trackId";
    }
    else if( stored->storageId() == Data::StoredVideo::STORAGE_ID ) {
        return "vidId";
    }
    else if( stored->storageId() == Data::ArtistInfo::STORAGE_ID ) {
        return "name";
    }
    else if( stored->storageId() == Data::AlbumInfo::STORAGE_ID ) {
        return "albumId";
    }
    else if( stored->storageId() == Data::SavedPlaylist::STORAGE_ID ) {
        return "name";
    }
    return "NO_SUITABLE_TABLE";
}


QString StdDataUpdater::getTagString( const QSet<QString> &tags )
{
    QString result;
    QSet< QString >::const_iterator it = tags.begin();
    int count = 0;
    for( ; it != tags.end(); ++ it ) {
        result.append( (*it) );
        if( count < tags.size() - 1 ) {
            result.append( "," );
        }
    }
    return result;
}



} } //end of namespaces
