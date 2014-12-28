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
