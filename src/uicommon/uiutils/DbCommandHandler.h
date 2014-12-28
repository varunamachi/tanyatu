/*******************************************************************************
 * DbCommandHandler.h
 *
 * Copyright (c) 2013, Varuna L Amachi. All rights reserved.
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
#pragma once

#include <QThread>
#include <QQueue>
#include <QCoreApplication>
#include <core/TanyatuCoreGlobal.h>

namespace Tanyatu {
namespace Worker {

class TANYATU_CORE_EXPORT DbCommandHandler : public QThread
{
    Q_OBJECT
public:
    enum CommandCode
    {
        Cmd_AddStoredObjects,
        Cmd_AddTracksToLib,
        Cmd_ClearAudioLib,
        Cmd_RemoveTracksFromLib,
        Cmd_UpdateTrackInfo,
        Cmd_AddArtist,
        Cmd_AddAlbum,
        Cmd_RemoveArtist,
        Cmd_RemoveAlbum,
        Cmd_UpdateArtist,
        Cmd_UpdateAlbum,
        Cmd_UpdateStoredObject,
        Cmd_SavePlaylist
    };

    static DbCommandHandler* get();

    bool hasPendingTasks()
    {
//        return ( ! m_opQueue.isEmpty() ) || this->isRunning();
        return this->isRunning() || hasQueuedCommands();
    }

signals:
    void cmdExecutionStarted(
            int type, void *data );

    void cmdExecutionFinished(
            int type,
            void *data,
            bool result );

public slots:
    void addCmd( CommandCode op, void *data )
    {
        createAndAddCommand( op, data );
        if( ! isRunning() ) {
            start();
        }
    }

protected:
    void run();

private:
    struct Command {
        Command( CommandCode code,
                 void *data )
            : m_code( code ),
              m_data( data )
        {

        }

        CommandCode m_code;

        void *m_data;
    };

    DbCommandHandler( QObject *parent = 0 )
        : QThread( parent ) { /** NOTHING_HERE */ }

    void createAndAddCommand( CommandCode op, void *data );

    Command takeNextCommand();

    bool hasQueuedCommands();


    QQueue< Command > m_addOpQueue;

    QQueue< Command > m_updateOpQueue;

    QQueue< Command > m_removeOpQueue;

    static DbCommandHandler *s_instance;
};

} // namespace Worker
} // namespace Tanyatu


#define DBCMD_CODE Tanyatu::Worker::DbCommandHandler
#define DBCMD_HANDLER() Tanyatu::Worker::DbCommandHandler::get()
