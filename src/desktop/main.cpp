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
#ifndef Q_OS_WIN
    #include <core/impl/PhononAudioEngine.h>
#endif

#include "app/ChilliWindow.h"
#include "app/ComponentManager.h"
#include "libview/AudioLibView.h"
#include "trackview/AudioTrackView.h"
#include "playlistview/PlaylistView.h"
#include "viewcommon/CommonAudioTrackView.h"
#include "viewcommon/RatingDelegate.h"

#include "player/PlayerControlWidget.h"


void initCoreComponents( QObject *parent )
{
    Tanyatu::FileSystemUtil::initTanyatuFS();

    Tanyatu::IEngine *engine =
            new Tanyatu::Impl::MMAudioEngine( parent );
#if 0
    Tanyatu::IEngine *engine =
            new Tanyatu::Impl::PhononAudioEngine( parent );
#endif
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
    auto audio = new QList< Tanyatu::Data::StoredAudio *>();
    AUDIO_LIB()->allItems( *audio );
    Tanyatu::Ui::CommonTrackModel *trackModel =
            new Tanyatu::Ui::CommonTrackModel( 5, win );
    trackModel->clear( true );
    trackModel->setTrackList( audio );
    auto trackView = new GreenChilli::Components::AudioTrackView( trackModel,
                                                                  4,
                                                                  win );
    GreenChilli::ComponentManager::get()->addComponent( trackView );
    GreenChilli::ComponentManager::get()->addComponent(
                new GreenChilli::Components::AudioLibView( win ));
    GreenChilli::ComponentManager::get()->addComponent(
                new GreenChilli::Components::PlaylistView( win ));

    QObject::connect( AUDIO_LIB(),
                      &Tanyatu::IAudioLibrary::libraryChanged,
                      [ = ]()
    {
        auto audio = new QList< Tanyatu::Data::StoredAudio *>();
        AUDIO_LIB()->allItems( *audio );
        trackModel->clear( true );
        trackModel->setTrackList( audio );
    });

    QObject::connect( AUDIO_LIB(),
                      &Tanyatu::IAudioLibrary::aboutToClear,
                      [ = ]()
    {
        auto audio = new QList< Tanyatu::Data::StoredAudio *>();
        trackModel->clear( true );
        trackModel->setTrackList( audio );
    });

}


int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        QCoreApplication::instance()->setApplicationName( "GreenChilli" );
        a.setApplicationName( QObject::tr( "GreenChilli" ));

        CHILLI_CACHE()->insertPixmap( "star_empty", ":/images/norating" );
        CHILLI_CACHE()->insertPixmap(  "star_filled", ":/images/hot" );
        initCoreComponents( 0 );
        GreenChilli::ChilliWindow win;
        win.setWindowTitle( "GreenChilli" );
        win.setWindowIcon( QIcon( ":/images/greenchilli" ));
        initUiComponents( &win );
        win.show();
        //    GreenChilli::PlayerControlWidget win;
        //    win.show();
        return a.exec();
    } catch( ... ) {
        qDebug() << "Caught an exception";
    }
    return -1;
}

