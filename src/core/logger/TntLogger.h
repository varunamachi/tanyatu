#pragma once

#include <QString>
#include <QDateTime>
#include <QTextStream>
#include <QThread>
#include <QtGlobal>

#include "LogUtil.h"
#include "LogStructures.h"
#include "../coreutils/Convenience.h"
#include "../TanyatuCoreGlobal.h"


#define CUR_THREAD_ID quint64( 0 )

namespace Tanyatu { namespace Logger {

class TntLogger;

TNT_INTERFACE ILogFormatter
{
    virtual QString format( const LogMessage *message ) = 0;

    virtual ~ILogFormatter() { }
};



class AbstractLogTarget
{
public:
    AbstractLogTarget( QString uniqueId )
        : m_uniqueId( uniqueId )
        , m_formatter( nullptr )
    {

    }

    QString uniqueId()
    {
        return m_uniqueId;
    }

    void setFormatterOwned( ILogFormatter *formatter )
    {
        if( m_formatter != nullptr ) {
            delete m_formatter;
        }
        m_formatter = formatter;
    }

    virtual void write( const LogMessage *message )
    {
        this->write(( m_formatter != nullptr ? m_formatter->format( message )
                                             : LogUtil::format( message )));
    }

    virtual ~AbstractLogTarget()
    {
        if( m_formatter != nullptr ) {
            delete m_formatter;
        }
    }

    virtual void flush() = 0;

protected:
    virtual void write( const QString logString ) = 0;

    ILogFormatter * formatter()
    {
        return m_formatter;
    }

private:
    QString m_uniqueId;

    ILogFormatter *m_formatter;

};



TNT_INTERFACE ILogFilter
{
    virtual QString filterId() const = 0;

    virtual bool filterOut( LogMessage *message ) = 0;

    virtual ~ILogFilter(){ }
};



TNT_INTERFACE ILogDispatcher
{
    virtual bool addTarget( AbstractLogTarget *target ) = 0;

    virtual AbstractLogTarget * target( QString targetId ) = 0;

    virtual bool setTargetEnabledState( const QString &trgId, bool value ) = 0;

    virtual bool removeTarget( const QString &trgId ) = 0;

    virtual bool installFilter( ILogFilter *filter,
                                const QString &trgId = "" ) = 0;

    virtual bool uninstallFilter( const QString &filterId,
                                  const QString &trgId = "" ) = 0;

    virtual void write( LogMessage *message ) = 0;

    virtual void flush() = 0;

    virtual void stopDispatch() = 0;

    virtual ~ILogDispatcher(){ }
};


class TANYATU_CORE_EXPORT TntLogger
{
public:

    void setDispatcher( ILogDispatcher *dispatcher )
    {
        if( m_dispatcher ) {
            delete m_dispatcher;
        }
        m_dispatcher = dispatcher;
    }

    ILogDispatcher * dispatcher() const
    {
        return m_dispatcher;
    }

    void setFilterLevel( TntLogLevel level )
    {
        m_globalSevLevel = level;
    }

    bool isMethodLoggingEnabled() const
    {
        return m_logMethods;
    }

    void setMethodLogState( bool value )
    {
        m_logMethods = value;
    }

    TntLogLevel filterLevel() const
    {
        return m_globalSevLevel;
    }

    static TntLogger * get()
    {
        return s_instance;
    }

    bool isEnabled() const
    {
        return m_enabled;
    }

    void setEnabled( bool val )
    {
        m_enabled = val;
    }

    void log( QDateTime &time,
                    TntLogLevel level,
                    int lineNumber,
                    QString &module,
                    QString &method,
                    QString &message );

    void log( LogMessage *msg );

    static bool init( ILogDispatcher *dispatcher,
               TntLogLevel level = TntLogLevel::Info );

    static void destroy();

private:
    TntLogger( ILogDispatcher *dispatcher, TntLogLevel level )
        : m_dispatcher( dispatcher )
        , m_globalSevLevel( level )
        , m_logMethods( false )
        , m_enabled( true )
    {

    }


    ~TntLogger()
    {
        delete m_dispatcher;
    }

    static TntLogger *s_instance;

    ILogDispatcher *m_dispatcher;

    TntLogLevel m_globalSevLevel;

    bool m_logMethods;

    bool m_enabled;

};


class LogLineHolder
{
public:
    LogLineHolder( LogMessage *msg )
        : m_msg( msg )
        , m_stream( &m_msg->mutableMessage() )
    {

    }

    QTextStream & stream()
    {
        return m_stream;
    }

    ~LogLineHolder()
    {
        m_stream.flush();
        TntLogger::get()->log( m_msg );
    }

private:
    LogMessage *m_msg;

    QTextStream m_stream;
};


} } // namespace Tanyatu

#define TNT_LOGGER() Tanyatu::Logger::TntLogger::get()

#ifndef __FUNCTION_NAME__
    #ifdef _MSC_VER //VC++
        #define __FUNCTION_NAME__   __FUNCSIG__
    #else          //Other
        #define __FUNCTION_NAME__   __PRETTY_FUNCTION__
    #endif
#endif


#ifndef TNT_DISABLE_LOGGING
    #define TNTL_COMMON( level, mod )                                       \
        Tanyatu::Logger::LogLineHolder(                                   \
           new Tanyatu::Logger::LogMessage(                                 \
                                            level,                          \
                                            QDateTime::currentDateTime(),   \
                                            CUR_THREAD_ID,                  \
                                            mod,                            \
                                            __FUNCTION_NAME__,              \
                                            __LINE__ )).stream()



    #define TNT_TRACE( module ) \
        TNTL_COMMON( Tanyatu::Logger::TntLogLevel::Trace, module )

    #define TNT_DEBUG( module ) \
        TNTL_COMMON( Tanyatu::Logger::TntLogLevel::Debug, module )

    #define TNT_INFO( module ) \
        TNTL_COMMON( Tanyatu::Logger::TntLogLevel::Info, module )

    #define TNT_WARN( module ) \
        TNTL_COMMON( Tanyatu::Logger::TntLogLevel::Warn, module )

    #define TNT_ERROR( module ) \
        TNTL_COMMON( Tanyatu::Logger::TntLogLevel::Error, module )

    #define TNT_FATAL( module ) \
        TNTL_COMMON( Tanyatu::Logger::TntLogLevel::Fatal, module )
#else
    #define TNTL_COMMON( level, mod, message )
    #define TNT_TRACE( module, message )
    #define TNT_DEBUG( module, message )
    #define TNT_INFO( module, message )
    #define TNT_WARNING( module, message )
    #define TNT_ERROR( module, message )
    #define TNT_FATAL( module, message )
#endif

