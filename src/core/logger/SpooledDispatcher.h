/*******************************************************************************
 * SpooledDispatcher.h
 *
 * Copyright (c) 2014, Varuna L Amachi. All rights reserved.
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
#include <QHash>
#include <memory>
#include <QReadWriteLock>

#include "TntLogger.h"
#include "../TanyatuCoreGlobal.h"

namespace Tanyatu { namespace Logger {



class TANYATU_CORE_EXPORT SpooledDispatcher : public QThread,
                          public ILogDispatcher
{
    Q_OBJECT
public:
    SpooledDispatcher( QObject *parent );

    bool addTarget( AbstractLogTarget *target );

    AbstractLogTarget * target( QString targetId );

    bool setTargetEnabledState( const QString &trgId, bool value );

    bool removeTarget( const QString &targetId );

    bool installFilter( ILogFilter *filter,
                        const QString &trgtId = "" );

    bool uninstallFilter( const QString &filterId,
                          const QString &trgtId = "" );

    void write( LogMessage *message );

    void flush();

    void stopDispatch();

    ~SpooledDispatcher();

protected:
    void run();

private:
    struct TargetInfo
    {
        TargetInfo( AbstractLogTarget *target, bool enable )
            : m_target( target )
            , m_enabled( enable ) {}

        ~TargetInfo()
        {
            delete m_target;
        }

        AbstractLogTarget *m_target;

        bool m_enabled;

        QList< ILogFilter *> m_targetFilters;
    };

    struct FilterInfo
    {
        FilterInfo( ILogFilter *filter )
            : m_filter( filter )
            , m_refs( 0 )
            , m_enabled( true ) {}

        ~FilterInfo()
        {
            delete m_filter;
        }

        ILogFilter *m_filter;

        int m_refs;

        bool m_enabled;
    };

    void writeToTargets( LogMessage *msg );

    bool m_stop;

    QQueue< LogMessage *> m_logQueue;

    QHash< QString, FilterInfo *> m_allFilters;

    QHash< QString, TargetInfo *> m_targets;

    QReadWriteLock m_lock;

    QReadWriteLock m_logIoLock;

    bool m_shouldStop;

};

} } // end of namespaces

