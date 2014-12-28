#include <QDebug>
#include <QCoreApplication>
#include <QTimer>

#include <core/logger/TntLogger.h>
#include <core/logger/SpooledDispatcher.h>
#include <core/logger/FileTarget.h>
#include <core/logger/ConsoleTarget.h>
#include <core/coreutils/Convenience.h>

#include "LogSimulator.h"

class SimpleFilter : public Tanyatu::Logger::ILogFilter
{
public:
    SimpleFilter( QString id, Tanyatu::Logger::TntLogLevel level )
        : m_id( id )
        , m_level( level )
    {

    }

    QString filterId() const
    {
        return m_id;
    }

    bool filterOut( Tanyatu::Logger::LogMessage *message )
    {
        return message->logLevel() < m_level;
    }

private:
    QString m_id;

    Tanyatu::Logger::TntLogLevel m_level;
};


class SomeFormatter : public Tanyatu::Logger::ILogFormatter
{

public:
    QString format( const Tanyatu::Logger::LogMessage *msg )
    {
        using namespace Tanyatu::Logger;
        QString strMsg = QString( msg->time().toString( "yyyy-MM-dd hh:mm:ss" ))
                + " "
                + LogUtil::getSeverityString( msg->logLevel() )
                + "  "
                + msg->message()
                + " ["
                + msg->moduleName() + " | "
                + QString::number( msg->lineNum()  ) + " | "
                + QString::number( msg->threadId() ) + " | "
                + msg->methodName() + " ] ";
        return strMsg;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTimer timer;

    auto dispatcher = new Tanyatu::Logger::SpooledDispatcher( &timer);
    dispatcher->addTarget(
                new Tanyatu::Logger::FileTarget( "test_log" ));
    auto ctarg = new Tanyatu::Logger::ConsoleTarget();
    ctarg->setFormatterOwned( new SomeFormatter() );
    dispatcher->addTarget( ctarg );
    Tanyatu::Logger::TntLogger::init(
                dispatcher, Tanyatu::Logger::TntLogLevel::Trace );
    dispatcher->installFilter(
                new SimpleFilter( "SimpleFilter_Debug",
                                  Tanyatu::Logger::TntLogLevel::Debug ),
                Tanyatu::Logger::FileTarget::TARGET_ID );

    dispatcher->installFilter(
                new SimpleFilter( "SimpleFilter_Info",
                                  Tanyatu::Logger::TntLogLevel::Info ),
                Tanyatu::Logger::ConsoleTarget::TARGET_ID );

    QList< LoggerThread *> threads;
    for( int i = 0; i < 3; ++ i ) {
        threads.append( new LoggerThread(
                            QString( "Thread[ %1 ]" ).arg( i ),
                            QCoreApplication::instance() ));
    }

    foreach( LoggerThread *th, threads ) {
        th->start();
    }

    QObject::connect(
                &timer,
                &QTimer::timeout,
                [ = ]()
                {
                    TNT_INFO( "Main" ) << "*** Removing filter ***";
                    dispatcher->uninstallFilter(
                                "SimpleFilter_Info",
                                Tanyatu::Logger::ConsoleTarget::TARGET_ID );
                });



    QObject::connect( &a,
                      &QCoreApplication::aboutToQuit,
                      [ = ]()
                      {
                          dispatcher->flush();
                          foreach( LoggerThread *th, threads ) {
                              th->thatsEnough();
                              th->wait( 10 );
                              qDebug() <<  "stopped  " << th->name() << endl;
                          }
                      });

    timer.setSingleShot( true );
    timer.start( 5000 );
    return a.exec();
}
