/*******************************************************************************
 * TManager.cpp
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
#include <QSqlQuery>
#include <QSqlError>

#include "T.h"
#include "coreutils/ComponentIntegrator.h"


using namespace Tanyatu;


T *T::s_instance = 0;
bool T::s_initialized = false;


void T::init( IEngine *audioEngine,
              IAudioLibrary *audioLibrary,
              ITrackInformationManager *trackInfoManager,
              IPlayQueue *playQueue,
              IPlaylistManager *playlistManger,
              IHistoryManager *historyManager,
              IDataSourceManager *dataManager,
              IDataRetriever *dataRetriever,
              IDataUpdater *dataUpdater,
              JobManager *jobManager,
              Logger::LogLevel logLevel,
              LogWriterFunc writerFunc )
{
    if( ! s_initialized ) {
        s_instance = new T;
        s_instance->m_audioEngine       = audioEngine;
        s_instance->m_audioLibrary      = audioLibrary;
        s_instance->m_trackInfoManager  = trackInfoManager;
        s_instance->m_playQueue         = playQueue;
        s_instance->m_playlistManger    = playlistManger;
        s_instance->m_historyManager    = historyManager;
        s_instance->m_dataManager       = dataManager;
        s_instance->m_dataRetriever     = dataRetriever;
        s_instance->m_dataUpdater       = dataUpdater;
        s_instance->m_jobManager        = jobManager;

        s_instance->m_logger = new Logger( writerFunc, logLevel );

        ComponentIntegrator *integrator = new ComponentIntegrator();
        integrator->integrate();
    }
}





T *T::get()
{
    return s_instance;
}


T::~T()
{
    delete m_audioEngine;
    delete m_audioLibrary;
    delete m_trackInfoManager;
    delete m_playQueue;
    delete m_playlistManger;
    delete m_dataManager;
    delete m_dataRetriever;
    delete m_dataUpdater;
}


////////////////////// Logger methods /////////////////////////////
T::Logger *T::logger()
{
    return s_instance->m_logger;
}


void T::Logger::log( Logger::LogLevel level, QString message ) {
    QString msg;
    msg.append(QDateTime::currentDateTime().toString());
    switch(level) {
    case Log_Debug:
        msg.append(" [DEBUG] ");
        break;
    case Log_Info:
        msg.append(" [ INFO] ");
        break;
    case Log_Warning:
        msg.append(" [ WARN] ");
        break;
    case Log_Error:
        msg.append(" [ERROR] ");
        break;
    case Log_Fatal:
        msg.append(" [FATAL] ");
        break;
    }
    if( m_logLevel == Log_Error ) {
        msg.append(message);
        m_writerFunc( message );
    }
}


void T::Logger::log( QString queryId, QSqlQuery &query ) {
    QSqlError queryError = query.lastError();
    if( queryError.isValid() ) {
        QString errorText( "Query: %1 failed." );
        this->error( errorText.arg(queryId) );
        if( m_logLevel == Log_Debug ) {
            QString errorText(
              "DETAILS: \n\t\tDB_ERROR: %1\n\t\tDRIVER_ERROR: %2"
              "\n--------------------Q-U-E-R-Y-----------------------------\n"
              "%3"
              "\n-----------------E-N-D---Q-U-E-R-Y------------------------\n");
            debug( errorText.arg(queryError.databaseText() )
                      .arg( queryError.driverText() )
                      .arg( query.executedQuery()) );
        }
    }
    else {
        QString msg("Query %1 succeeded.");
        debug( msg.arg( queryId ));
    }
}


