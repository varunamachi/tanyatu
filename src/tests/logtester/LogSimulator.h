#pragma once
#include <QThread>
#include <QDateTime>
#include <QDebug>
#include <ctime>
#include <core/logger/TntLogger.h>



class LoggerThread : public QThread
{
    Q_OBJECT
public:
    LoggerThread( QString name, QObject *parent )
        : QThread( parent )
        , m_name( name )
        , m_thatsEnough( false )
        , m_index( 0 )
    {
        connect( this,
                 &QThread::started,
                 [ this ]() { m_thatsEnough = false; } );
    }

    void thatsEnough()
    {
        m_thatsEnough = true;
    }

    QString name() const { return m_name; }

protected:
    void run()
    {
        while( ! m_thatsEnough ) {
            using namespace Tanyatu::Logger;
            switch ( m_index ) {
            case 0:
                TNT_TRACE( m_name ) << m_name << " " << qrand();
                break;
            case 1:
                TNT_DEBUG( m_name ) << m_name << " " << qrand();
                break;
            case 2:
                TNT_INFO( m_name ) << m_name << " " << qrand();
                break;
            case 3:
                TNT_WARN( m_name ) << m_name << " " << qrand();
                break;
            case 4:
                TNT_ERROR( m_name ) << m_name << " " << qrand();
                break;
            default:
                TNT_FATAL( m_name ) << m_name << " " << qrand();
                break;
            }
            this->sleep( 1 );
            m_index = ( m_index + 1 ) % 6;
        }
    }

private:
    QString m_name;

    bool m_thatsEnough;

    int m_index;
};


