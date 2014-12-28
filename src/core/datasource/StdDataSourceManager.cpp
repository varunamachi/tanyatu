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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QPair>

#include "StdDataSourceManager.h"
#include "../T.h"

namespace Tanyatu { namespace Impl {


StdDataSourceManager::StdDataSourceManager() :
    IDataSourceManager( ),
    m_error( "" ),
    m_valid( false )
{
}


bool StdDataSourceManager::init( QHash<QString, QString> &params )
{
    if( ! m_valid ) {
        QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
        db.setHostName( params.value( "hostname","localHost" ));
        db.setDatabaseName( params.value( "dbname", "tanyatu.sqlite" ));
        if ( db.open() ) {
            QSqlQuery query( "SELECT name FROM sqlite_master "
                             "WHERE type='table' AND name='AudioLibrary'" );
            if( query.exec() && query.next() ) {
                T::logger()->info( "Database initiation succesful" );
                m_valid = true;
            }
            else {
                //Database does not exists
                T::logger()->info( "Creating database" );
                if( createSchema() ) {
                    T::logger()->info( "Database creation successfull");
                    m_valid = true;
                }
                else {
                    T::logger()->error( "Database creation failed!" );
                    m_valid = false;
                }
            }
            return m_valid;
        }
        T::logger()->error( db.lastError().text() );
        return false;
    }
    else {
        T::logger()->warning( "Attempt to reinitialize data source. Ignored..");
        return false;
    }
}


bool StdDataSourceManager::isDataSourceValid()
{
    return m_valid;
}


QString StdDataSourceManager::dataSourceId()
{
    return "SQLITE";
}


QString StdDataSourceManager::description()
{
    return QObject::tr( "This is the SqlLite data source backend for Tanyatu" );
}


QString StdDataSourceManager::lastError()
{
    return m_error;
}


bool StdDataSourceManager::createSchema()
{
    QHash< QString, QString > *queries = getQueries();
    QHash< QString, QString >::iterator it;
    for( it = queries->begin(); it != queries->end(); ++ it) {
        QString queryId = it.key();
        QString query   = it.value();
        if( ! executeQuery( queryId, query )) {
            delete queries;
            return false;
        }
    }
    delete queries;
    return true;
}


bool StdDataSourceManager::executeQuery(
        const QString queryId,
        const QString queryString )
{
        QSqlQuery query;
        bool result = query.exec(queryString);
        T::logger()->log( queryId, query );
        if( ! result ) {
            QSqlError queryError = query.lastError();
            if( queryError.isValid() ) {
                m_error = QString( "%1 - %2").arg(queryError.databaseText() )
                        .arg( queryError.driverText() );
            }
        }
        return result;

}



QHash< QString, QString > *StdDataSourceManager::getQueries()
{
    QHash< QString, QString > *queries = new QHash< QString, QString >();
    queries->insert( "createAudioLibrarySchema",
            "CREATE TABLE AudioLibrary ("
            "    trackId        TEXT	PRIMARY KEY,"
            "    url            TEXT,"
            "    title          TEXT,"
            "    artist         TEXT,"
            "    album          TEXT,"
            "    cdate          INTEGER,"
            "    genre          TEXT,"
            "    description    TEXT,"
            "    trackNumber    TEXT,"
            "    duration       INTEGER,"
            "    bitRate        INTEGER,"
            "    sampleRate 	INTEGER,"
            "    channels       INTEGER,"
            "    rating         INTEGER,"
            "    playCount      INTEGER,"
            "    tags           TEXT"
            ");" );

    queries->insert( "createVideoLibrarySchema",
            "CREATE TABLE VideoLibrary ("
            "    vidId          TEXT	PRIMARY KEY,"
            "    url            TEXT,"
            "    name           TEXT,"
            "    category       TEXT,"
            "    year           INTEGER,"
            "    duration       INTEGER,"
            "    descreption    TEXT,"
            "    creator        TEXT,"
            "    artistsRoles   TEXT,"
            "    rating         INTEGER,"
            "    playCount      INTEGER,"
            "    tags           TEXT"
            ");" );

    queries->insert( "createArtistInfoSchema",
            "CREATE TABLE ArtistInfo ("
            "    name           TEXT	PRIMARY KEY,"
            "    comment        TEXT,"
            "    rating         INTEGER,"
            "    playCount      INTEGER,"
            "    tags           TEXT"
            ");" );

    queries->insert( "createAlbumInfoSchema",
            "CREATE TABLE AlbumInfo ("
            "    albumId        TEXT    PRIMARY KEY,"
            "    artistName     TEXT,"
            "    name           TEXT,"
            "    comment        TEXT,"
            "    rating         INTEGER,"
            "    playCount      INTEGER,"
            "    tags           TEXT"
            ");" );

    queries->insert( "createSavedPlaylistSchema",
            "CREATE TABLE SavedPlaylist ("
            "    name           TEXT	PRIMARY KEY,"
            "    description	TEXT,"
            "    creationTime   INTEGER,"
            "    modifiedTime   INTEGER,"
            "    rating         INTEGER,"
            "    playCount      INTEGER,"
            "    tags           TEXT"
            ");" );

    queries->insert( "createPlayHistorySchema",
            "CREATE TABLE PlayHistory ("
            "    lastPlayedTime INTEGER,"
            "    trackId        TEXT	   REFERENCES AudioLibrary( trackId )"
            ");" );


    queries->insert( "createRecentAdditionsSchema",
            "CREATE TABLE RecentAdditions ("
            "    addedTime      INTEGER,"
            "    trackId        TEXT	   REFERENCES AudioLibrary( trackId )"
            ");" );

    queries->insert( "createPlaylistContentSchema",
            "CREATE TABLE PlaylistContent ("
            "    trackIdex          INTEGER, "
            "    playlistName   TEXT	   REFERENCES SavedPlaylist( name ),"
            "    trackId        TEXT	   REFERENCES AudioLibrary( trackId )"
            ");" );

    queries->insert( "createPropertyStoreSchema",
            "CREATE TABLE PropertyStore ("
            "    key            TEXT    PRIMARY KEY, "
            "    type           TEXT	,"
            "    value          TEXT	"
            ");" );

    return queries;
}


} } //end of namespaces
