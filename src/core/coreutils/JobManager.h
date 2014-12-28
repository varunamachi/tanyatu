/*******************************************************************************
 * JobManager.h
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

#include <functional>
#include <QThread>
#include <QQueue>
#include <QSet>
#include <QReadWriteLock>

#include "Convenience.h"

namespace Tanyatu {

typedef std::function< void() > RespFunc;
typedef std::function< RespFunc() > JobFunc;


TNT_INTERFACE IJob
{
    virtual const QString& name() const = 0;

    virtual const QString& category() const = 0;

    virtual bool shouldPostResponse() const = 0;

    virtual RespFunc execute() = 0;

    virtual ~IJob() {}
};


class JobManager : public QThread
{
    Q_OBJECT

public:
    explicit JobManager( QObject *parent = nullptr );

    bool hasPendingJobs();

signals:
    void executionFinished( QString name, QString category );

    void executionStarted( QString name, QString category );

    void allJobsFinished();

    void jobDescarded( const QString name, const QString category );

public slots:
    void addJob( IJob *job );

    void addJob( QString name,
                 JobFunc job,
                 QString category = "general",
                 bool postResponseToEventQueue = true );

    bool cancelJob( QString name );

    bool cancelJobCategory( QString category );

    void cancelAll();

protected:
    void run();

    bool event( QEvent *event );

private:
    QReadWriteLock m_lock;

    QQueue< IJob * > m_jobs;

    QSet< QString > m_cancelledJobs;

    QSet< QString > m_cancelledCategories;

    bool m_stopProcessing;
};

} //end of namespace

