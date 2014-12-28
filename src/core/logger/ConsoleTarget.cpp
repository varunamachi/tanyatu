/*******************************************************************************
 * ConsoleSync.cpp
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
#include <QDebug>

#include "ConsoleTarget.h"
#include "TntLogger.h"
#include "LogUtil.h"

#define format( x ) ( formatter() != nullptr ? formatter()->format( x )  \
                                             : LogUtil::format( x ))

namespace Tanyatu { namespace Logger {

const QString ConsoleTarget::TARGET_ID = "ConsoleLogger";

ConsoleTarget::ConsoleTarget()
    : AbstractLogTarget( TARGET_ID )
{
}


void ConsoleTarget::write( const LogMessage *message )
{
    if( message ) {
        if( message->logLevel() <= TntLogLevel::Info ) {
            qDebug() << format( message );
        }
        else if( message->logLevel() == TntLogLevel::Warn ) {
            qWarning() << format( message );
        }
        else {
            qCritical() << format( message );
        }
    }
}


void ConsoleTarget::write( const QString message )
{
    Q_UNUSED( message )
}

} }//end of namespace
