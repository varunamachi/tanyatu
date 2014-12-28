/******************************************************************************* * main.cppC
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

#include <QApplication>
#include <QObject>

#include <core/T.h>
#include <core/impl/MMAudioEngine.h>
#include <core/impl/StdAudioLibrary.h>
#include <core/impl/StdTrackInformationManager.h>
#include <core/impl/StdHistoryManager.h>
#include <core/impl/StdPlayQueue.h>
#include <core/impl/StdPlaylistManager.h>
#include <core/datasource/StdDataRetriever.h>
#include <core/datasource/StdDataSourceManager.h>
#include <core/datasource/StdDataUpdater.h>
#include <core/coreutils/FileSystemUtil.h>
#include <uicommon/uiutils/ChilliCache.h>
#include <uicommon/itemmodels/CommonTrackModel.h>


#include "app/ChilliWindow.h"
#include "app/ComponentManager.h"
#include "libview/AudioLibView.h"
#include "trackview/AudioTrackView.h"
#include "playlistview/PlaylistView.h"
#include "viewcommon/CommonAudioTrackView.h"
#include "viewcommon/RatingDelegate.h"


void initCoreComponents( QObject *parent )
{
    Tanyatu::FileSystemUtil::initTanyatuFS();

    Tanyatu::IEngine *engine =
            new Tanyatu::Impl::MMAudioEngine( parent );
    Tanyatu::Impl::StdAudioLibrary *audLib =
            new Tanyatu::Impl::StdAudioLibrary( parent );
    Tanyatu::ITrackInformationManager *audMan =
            new Tanyatu::Impl::StdTrackInformationManager( parent );
    Tanyatu::IPlayQueue *playqueue =
            new Tanyatu::Impl::StdPlayQueue( parent );
    Tanyatu::IPlaylistManager *plMan =
            new Tanyatu::Impl::StdPlaylistManager( parent );
    Tanyatu::IHistoryManager *hisMan =
            new Tanyatu::Impl::StdHistoryManager( parent );
    Tanyatu::IDataSourceManager *dsMan =
            new Tanyatu::Impl::StdDataSourceManager();
    Tanyatu::IDataRetriever *dataRetriever =
            new Tanyatu::Impl::StdDataRetriever();
    Tanyatu::IDataUpdater *dataUpdater =
            new Tanyatu::Impl::StdDataUpdater();

    Tanyatu::T::init( engine,
                      audLib,
                      audMan,
                      playqueue,
                      plMan,
                      hisMan,
                      dsMan,
                      dataRetriever,
                      dataUpdater,
                      new Tanyatu::JobManager( parent ),
                      Tanyatu::T::Logger::Log_Debug );

    QHash< QString, QString > params;
    QString dbPath = Tanyatu::FileSystemUtil::getPathFromTanyatu(
                                                            "tanyatu.sqlite" );
    params.insert( QString( "dbname" ),  dbPath );

    dsMan->init( params );
    audLib->init();
    audMan->init();
    plMan->init();
    hisMan->init();
}

void initUiComponents( QMainWindow *win )
{
    ///[[ Working...]]
    Tanyatu::Ui::CommonTrackModel *trackModel =
            new Tanyatu::Ui::CommonTrackModel( 5, win );
    GreenChilli::ComponentManager::get()->addComponent(
                new GreenChilli::Components::AudioTrackView( trackModel,
                                                             4,
                                                             win ));
    GreenChilli::ComponentManager::get()->addComponent(
                new GreenChilli::Components::AudioLibView( win ));
    GreenChilli::ComponentManager::get()->addComponent(
                new GreenChilli::Components::PlaylistView( win ));
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::instance()->setApplicationName( "GreenChilli" );
    a.setApplicationName( QObject::tr( "GreenChilli" ));

    CHILLI_CACHE()->insertPixmap( "star_empty", ":/images/nothot" );
    CHILLI_CACHE()->insertPixmap(  "star_filled", ":/images/hot" );
    initCoreComponents( 0 );
    GreenChilli::ChilliWindow win;
    win.setWindowTitle( "GreenChilli" );
    win.setWindowIcon( QIcon( ":/images/greenchilli" ));
    initUiComponents( &win );
    win.show();
    return a.exec();
}

