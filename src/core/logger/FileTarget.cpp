/*******************************************************************************
 * FileSync.cpp
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
#include <QFile>
#include <QDebug>

#include "FileTarget.h"
#include "LogUtil.h"

namespace Tanyatu { namespace Logger {


const QString FileTarget::TARGET_ID = QString( "FileLogger" );

FileTarget::FileTarget( const QString &fileSuffix )
    : AbstractLogTarget( TARGET_ID )
    , m_fileSuffix( fileSuffix )
    , m_prevDate( QDateTime::currentDateTime() )
{
    initFile();
}


void FileTarget::write( const QString message )
{
    if( m_prevDate.daysTo( QDateTime::currentDateTime() ) != 0 ) {
        m_stream.flush();
        initFile();
    }
    m_stream << message << endl;
}

void FileTarget::flush()
{
    m_stream.flush();
}


void FileTarget::initFile()
{
    QString fileName = m_prevDate.toString( "yyyy_MM_dd_" )
                       + m_fileSuffix
                       + ".log";
    m_stream.reset();
    m_logFile.clear();
    m_logFile = new QFile( fileName );
    if( m_logFile->open( QIODevice::Append | QIODevice::Unbuffered )) {
        m_stream.setDevice( m_logFile.data() );
    }
    else {
        qDebug() << "Could not open the file....";
    }
}

} } //end of namespaces
