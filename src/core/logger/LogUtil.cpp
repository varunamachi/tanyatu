/*******************************************************************************
 * LogUtil.cpp
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

#include "LogUtil.h"
#include "TntLogger.h"

namespace Tanyatu { namespace Logger {



QString LogUtil::getSeverityString( TntLogLevel level )
{
    switch( level ) {
    case TntLogLevel::Trace : return "[ TRACE ]";
    case TntLogLevel::Debug : return "[ DEBUG ]";
    case TntLogLevel::Info  : return "[ INFO  ]";
    case TntLogLevel::Warn  : return "[ WARNG ]";
    case TntLogLevel::Error : return "[ ERROR ]";
    case TntLogLevel::Fatal : return "[ FATAL ]";
    default                 : return "[ ***** ]";
    }
    return "";
}


QString LogUtil::format( const LogMessage *msg )
{
    QString strMsg = QString( msg->time().toString( "yyyy-MM-dd hh:mm:ss" ))
            + " "
            + getSeverityString( msg->logLevel() )
            + "  "
            + msg->message()
  /*          + " ["
            + msg->moduleName() + " | "
            + QString::number( msg->lineNum()  ) + " | "
            + QString::number( msg->threadId() ) + " | "
            + msg->methodName() + " ] "*/;
    return strMsg;
}

} } //end of namespces
