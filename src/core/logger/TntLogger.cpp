#include "TntLogger.h"

#include <stdarg.h>

namespace Tanyatu { namespace Logger {

TntLogger *TntLogger::s_instance = 0;


bool TntLogger::init( ILogDispatcher *dispatcher, TntLogLevel level )
{
    bool result = false;
    if( s_instance == nullptr ) {
        s_instance = new TntLogger( dispatcher, level );
        result = true;
    }
    return result;
}


void TntLogger::destroy()
{
    delete s_instance;
}


void TntLogger::log( QDateTime &time,
                           TntLogLevel level,
                           int lineNumber,
                           QString &module,
                           QString &method,
                           QString &message )
{
    if( m_enabled
        && m_dispatcher != nullptr
        && level >= m_globalSevLevel ) {
        LogMessage *lgmsg = new LogMessage( level,
                                            time,
                                            CUR_THREAD_ID,
                                            module,
                                            method,
                                            lineNumber,
                                            message );
        m_dispatcher->write( lgmsg );
    }

}


void TntLogger::log( LogMessage *msg )
{
    if( m_enabled
        && m_dispatcher != nullptr
        && msg->logLevel() >= m_globalSevLevel ) {
        m_dispatcher->write( msg );
    }
}

} } // end of namespaces
