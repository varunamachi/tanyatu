/*******************************************************************************
 * StdPlaylistManager.cpp
 *
 * Copyright (c) 2012, Varuna L Amachi. All rights reserved.
 *
 * This program/library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program/library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 ******************************************************************************/

#include "StdPlaylistManager.h"
#include "../coreutils/Utils.h"
#include "../T.h"

#include <QList>
#include <QSize>
#include <QFont>



namespace Tanyatu { namespace Impl {

QString StdPlaylistManager::COMPONENT_NAME =
        "Tanyatu::Core::StdPlaylistManager";


QString StdPlaylistManager::COMPONENT_MODULE =
        "Tanyatu::Core";


StdPlaylistManager::StdPlaylistManager( QObject *parent )
    : IPlaylistManager( parent )
{

}


bool StdPlaylistManager::init()
{
    bool result = DATA_RETRIEVER()->getPlaylists( m_playlists );
    QHash< QString, Data::SavedPlaylist *>::iterator it = m_playlists.begin();
    emit aboutToChangePlaylistManager();
    for( ; it != m_playlists.end(); it ++) {
        loadPlaylist( it.key() );
    }
    emit playlistManagerChanged();
    return result;
}


StdPlaylistManager::~StdPlaylistManager()
{
    clear();
}


void StdPlaylistManager::appendToPlaylist( QString playlistName,
                                      QList< QString > &trackIds )
{
    QList< QString > *itemIds = m_playlistToTrackIds.value( playlistName );
    if( itemIds ) {
        itemIds->append( trackIds );
        emit playlistContentModified( m_playlists.value( playlistName ));
    }
}


void StdPlaylistManager::appendToPlaylist( QString playlistName,
                                           QString trackId )
{
    QList< QString > *itemIds = m_playlistToTrackIds.value( playlistName );
    if( itemIds ) {
        itemIds->append( trackId );
        emit playlistContentModified( m_playlists.value( playlistName ));
    }
}


bool StdPlaylistManager::deletePlaylist( QString playlistName )
{
    if( m_playlists.contains( playlistName )) {
        Data::SavedPlaylist *playlist = m_playlists.value( playlistName );
        emit aboutToChangePlaylistManager();
        emit aboutToRemovePlaylist( playlist );
        m_playlists.remove( playlistName );
        m_playlistToTrackIds.remove( playlistName );
        delete playlist;
        emit playlistRemoved( playlistName );
        emit playlistManagerChanged();
        T::get()->dataUpdater()->removePlaylist( playlistName );
        return true;
    }
    return false;
}


bool StdPlaylistManager::createPlaylist( QString playlistName )
{
    if( ! m_playlists.contains( playlistName )) {
        Data::SavedPlaylist *playlist = new Data::SavedPlaylist(
                    playlistName, QDateTime::currentDateTime() );
        emit aboutToChangePlaylistManager();
        m_playlists.insert( playlistName, playlist );
        m_playlistToTrackIds.insert( playlistName, new QList< QString >() );
        emit playlistCreated( playlist );
        emit playlistManagerChanged();
        return true;
    }
    return false;
}


bool StdPlaylistManager::setTrackIdList( QString playlistName,
                                         QList< QString > &list )
{
    bool result = false;
    if( m_playlists.contains( playlistName )) {
        QList< QString > *trackList = m_playlistToTrackIds.value( playlistName);
        emit aboutToChangePlaylistManager();
        trackList->clear();
        trackList->append( list );
        emit playlistModified( m_playlists.value( playlistName ));
        emit playlistManagerChanged();
        result = true;
    }
    return result;
}


bool StdPlaylistManager::clearPlaylist( QString playlistName )
{
    if( m_playlists.contains( playlistName )) {
        emit aboutToChangePlaylistManager();
        DATA_UPDATER()->clearPlaylist( playlistName );
        m_playlistToTrackIds.value( playlistName )->clear();
        emit playlistContentModified( m_playlists.value( playlistName ));
        emit playlistManagerChanged();
        return true;
    }
    return false;
}


bool StdPlaylistManager::deleteAllPlaylists()
{
    emit aboutToChangePlaylistManager();
    emit aboutToClear();
    for( auto plIt = m_playlists.begin(); plIt != m_playlists.end(); ++ plIt ) {
        Data::SavedPlaylist *playlist = plIt.value();
        emit aboutToRemovePlaylist( playlist );
//        m_playlists.remove( playlist->name() );
        delete m_playlistToTrackIds.value( playlist->name() );
        m_playlistToTrackIds.remove( playlist->name() );
        DATA_UPDATER()->removePlaylist( playlist->name() );
        delete playlist;
        emit playlistRemoved( playlist->name() );
    }
    m_playlists.clear();
    emit cleared();
    emit playlistManagerChanged();
    return true;

}


bool StdPlaylistManager::removeTracksFromPlaylist( QString playlistId,
                                                   QList< int > &trackIndices )
{
    int numRemoved = 0;
    if( trackIndices.size() > 1 ) {
        qSort( trackIndices );
    }
    if( m_playlists.contains(( playlistId ))) {
        QList< QString > *trackList = m_playlistToTrackIds.value( playlistId );
        if( trackList ) {
            emit aboutToChangePlaylistManager();
            for( int i = trackIndices.size() - 1; i >= 0; -- i ){
                int index = trackIndices.at( i );
                trackList->removeAt( index );
                ++ numRemoved;
            }
            emit playlistManagerChanged();
            emit playlistContentModified( m_playlists.value( playlistId ));
        }
    }
    return trackIndices.empty() ? false : ( numRemoved == trackIndices.size() );
}


void StdPlaylistManager::ratePlaylist( QString playlistName,
                   Data::MediaRating rating )
{
    if( m_playlists.contains( playlistName )) {
        emit aboutToChangePlaylistManager();
        Data::SavedPlaylist *playlist = m_playlists.value( playlistName );
        DATA_UPDATER()->updateRating( playlist );
        playlist->setRating( rating );
        emit playlistModified( playlist );
        emit playlistManagerChanged();
    }
}


void StdPlaylistManager::tagItem( QString playlistName, QString tag )
{
    if( m_playlists.contains( playlistName )) {
        Data::SavedPlaylist *playlist = m_playlists.value( playlistName );
        playlist->addTag( tag );
        T::get()->dataUpdater()->updateTags( playlist );
        emit playlistModified( playlist );
        emit playlistManagerChanged();
    }
}


void StdPlaylistManager::removeItemTag( QString playlistName, QString tag )
{
    if( m_playlists.contains( playlistName )) {
        Data::SavedPlaylist *playlist = m_playlists.value( playlistName );
        playlist->removeTag( tag );
        emit aboutToChangePlaylistManager();
        DATA_UPDATER()->updateTags( playlist );
        emit playlistModified( playlist );
        emit playlistManagerChanged();
    }
}


bool StdPlaylistManager::hasPlaylist(QString playlistName)
{
    return m_playlists.contains( playlistName );
}


Data::SavedPlaylist *StdPlaylistManager::playlist( QString plName ) const
{
    return m_playlists.value( plName, 0 );
}


void StdPlaylistManager::clear()
{
    emit aboutToChangePlaylistManager();
    emit aboutToClear();
    QHash< QString, QList< QString > *>::iterator it =
            m_playlistToTrackIds.begin();
    for(; it != m_playlistToTrackIds.end(); it ++ ) {
        QList< QString > *list = it.value();
        list->clear();
        delete list;
    }
    m_playlistToTrackIds.clear();
    for( auto it = m_playlists.begin(); it != m_playlists.end(); ++ it ) {
        delete it.value();
    }
    m_playlists.clear();
    emit cleared();
    emit playlistManagerChanged();
}


void StdPlaylistManager::onPlaylistSelected( QString playlistName )
{

    if( m_playlists.contains( playlistName )) {
        Data::SavedPlaylist *playlist = m_playlists.value( playlistName );
        emit aboutToChangePlaylistManager();
        playlist->setPlayCount( playlist->playCount() + 1 );
        DATA_UPDATER()->updatePlayCount( playlist );
        emit playlistManagerChanged();
    }

}


void StdPlaylistManager::onItemRemovalFromStorage( QString trackId )
{
    QHash< QString, QList< QString > *>::iterator mapIt =
            m_playlistToTrackIds.begin();
    emit aboutToChangePlaylistManager();
    for( ; mapIt != m_playlistToTrackIds.end(); ++ mapIt ) {
        QList< QString > *trackList = mapIt.value();
        if( trackList->removeAll( trackId ) != 0 ) {
            emit playlistContentModified( m_playlists.value( mapIt.key() ));
        }
    }
    emit playlistManagerChanged();
}


const QList< QString > StdPlaylistManager::trackIdsIn(
        QString playlistName )
{
    if( m_playlists.contains( playlistName )) {
        return *m_playlistToTrackIds.value( playlistName );
    }
    return QList< QString >();
}


quint32 StdPlaylistManager::numTracksIn( QString playlistName ) const
{
    if( m_playlistToTrackIds.contains( playlistName )) {
        return m_playlistToTrackIds.value( playlistName )->size();
    }
    return 0;
}


quint32 StdPlaylistManager::allPlaylists(
        QList< Data::SavedPlaylist *>& playlists )
{
    quint32 count = 0;
    for( auto it = m_playlists.begin(); it != m_playlists.end(); ++ it ) {
        playlists.append( it.value() );
    }
    return count;
}


QString StdPlaylistManager::displayName() const
{
    return tr("Playlist Manager");
}


QString StdPlaylistManager::module() const
{
    return COMPONENT_MODULE;
}


QString StdPlaylistManager::uniqueName() const
{
    return COMPONENT_NAME;
}

void StdPlaylistManager::loadPlaylist( const QString playlistName )
{
    if( ! m_playlistToTrackIds.contains( playlistName ) ) {
        QList< QString > *list = new QList< QString >();
        if( DATA_RETRIEVER()->getPlaylistContent( playlistName, *list )) {
            m_playlistToTrackIds.insert( playlistName, list );
        }
    }
}


} } //End of namespaces
