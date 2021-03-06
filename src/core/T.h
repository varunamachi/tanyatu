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

#include <QDebug>

#include "interfaces/ITrackInformationManager.h"
#include "interfaces/IAudioLibrary.h"
#include "interfaces/IPlayQueue.h"
#include "interfaces/IEngine.h"
#include "interfaces/IPlaylistManager.h"
#include "interfaces/IHistoryManager.h"
#include "interfaces/IDataSourceManager.h"
#include "interfaces/IDataRetriever.h"
#include "interfaces/IDataUpdater.h"
#include "coreutils/JobManager.h"
#include "data/StoredAudio.h"

class QSqlQuery;


namespace Tanyatu {

class TANYATU_CORE_EXPORT T
{
public:

    typedef void ( *LogWriterFunc )( QString message );

    class TANYATU_CORE_EXPORT Logger
    {
    public:
        enum LogLevel {
            Log_Debug,
            Log_Info,
            Log_Warning,
            Log_Error,
            Log_Fatal
        };

        void log( LogLevel level, QString message );

        void log( QString queryId, QSqlQuery &query );

        void setLevel( LogLevel level )
        {
            m_logLevel = level;
        }


        void debug( QString message )
        {
            log( Log_Debug, message );
        }

        void info( QString message )
        {
            log( Log_Info, message );
        }

        void warning( QString message )
        {
            log( Log_Warning, message );
        }

        void error( QString message )
        {
            log( Log_Error, message );
        }

        void fatal( QString message )
        {
            log( Log_Fatal, message );
        }


    protected:

        T::LogWriterFunc m_writerFunc;

        LogLevel m_logLevel;

        static void consoleWrite( QString message )
        {
            qDebug() << message;
        }

        Logger( LogWriterFunc func, LogLevel logLevel )
        {
            m_writerFunc = func ? func : & Logger::consoleWrite;
            m_logLevel = logLevel;
        }

        friend class T;
    };


    IEngine *audioEngine() const
    {
        return m_audioEngine;
    }

    IAudioLibrary *audioLib() const
    {
        return m_audioLibrary;
    }

    ITrackInformationManager *trackInfoManager() const
    {
        return m_trackInfoManager;
    }

    IPlayQueue *playQueue() const
    {
        return m_playQueue;
    }

    IPlaylistManager *playlistManager() const
    {
        return m_playlistManger;
    }

    IHistoryManager *historyManager() const
    {
        return m_historyManager;
    }

    IDataSourceManager *dataManager() const
    {
        return m_dataManager;
    }

    IDataRetriever *dataRetriever() const
    {
        return  m_dataRetriever;
    }

    IDataUpdater *dataUpdater() const
    {
        return m_dataUpdater;
    }

    JobManager * jobManager() const
    {
        return m_jobManager;
    }



    virtual ~T();

    static void init( IEngine *audioEngine,
                     IAudioLibrary *audioLibrary,
                     ITrackInformationManager *trackInfoManager,
                     IPlayQueue *playlist,
                     IPlaylistManager *playlistManger,
                     IHistoryManager *historyManager,
                     IDataSourceManager *dataManager,
                     IDataRetriever *dataRetriever,
                     IDataUpdater *dataUpdater,
                     JobManager *jobManager,
                     Logger::LogLevel logLevel = Logger::Log_Error,
                     LogWriterFunc logWriterFunction = 0 );

    static T *get();

    static Logger *logger();

private:

    T()
    {

    }

    IEngine *m_audioEngine;

    IAudioLibrary *m_audioLibrary;

    ITrackInformationManager *m_trackInfoManager;

    IPlayQueue *m_playQueue;

    IPlaylistManager *m_playlistManger;

    IHistoryManager *m_historyManager;

    IDataSourceManager *m_dataManager;

    IDataRetriever *m_dataRetriever;

    IDataUpdater *m_dataUpdater;

    JobManager *m_jobManager;

    Logger *m_logger;

    static bool s_initialized;

    static T *s_instance;
};

} //end of namespace


#define AUDIO_ENGINE()      Tanyatu::T::get()->audioEngine()
#define AUDIO_LIB()         Tanyatu::T::get()->audioLib()
#define TRACKINF_MANAGER()  Tanyatu::T::get()->trackInfoManager()
#define PLAYQUEUE()         Tanyatu::T::get()->playQueue()
#define PL_MANAGER()        Tanyatu::T::get()->playlistManager()
#define HISTORY_MANAGER()   Tanyatu::T::get()->historyManager()
#define DATA_UPDATER()      Tanyatu::T::get()->dataUpdater()
#define DATA_RETRIEVER()    Tanyatu::T::get()->dataRetriever()
#define DATA_MANAGER()      Tanyatu::T::get()->dataManager()
#define JOB_MANAGER()       Tanyatu::T::get()->jobManager()

