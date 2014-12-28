#pragma once

#include <QString>
#include <QDateTime>
#include <QtGlobal>

#include "../TanyatuCoreGlobal.h"


namespace Tanyatu { namespace Logger {

class TntLogger;
enum class TntLogLevel
{
    Method  = -1,
    Trace   = 0,
    Debug   = 1,
    Info    = 2,
    Warn    = 3,
    Error   = 4,
    Fatal   = 5,
    Special = 6
};


struct LogMessage
{
public:
    LogMessage( TntLogLevel level,
                QDateTime time,
                quint64 threadId,
                QString module,
                QString method,
                int lineNum,
                QString message = "" )
        : m_logLevel( level )
        , m_time( time )
        , m_threadId( threadId )
        , m_moduleName( module )
        , m_methodName( method )
        , m_lineNumber( lineNum )
        , m_logMessage( message )
    {

    }

    inline const TntLogLevel & logLevel() const { return m_logLevel; }

    inline const QDateTime & time() const { return m_time; }

    inline const quint64 & threadId() const { return m_threadId; }

    inline const QString & moduleName() const { return m_moduleName; }

    inline const QString & methodName() const { return m_methodName; }

    inline const int & lineNum() const { return m_lineNumber; }

    inline const QString & message() const { return m_logMessage; }

    inline QString & mutableMessage() { return m_logMessage; }

private:
    TntLogLevel m_logLevel;

    QDateTime m_time;

    quint64 m_threadId;

    QString m_moduleName;

    QString m_methodName;

    int m_lineNumber;

    QString m_logMessage;
};

} } //end of ns
