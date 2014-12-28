/*******************************************************************************
 * StdSettingsManager.h
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


#include <QtGlobal>
#include <QString>
#include <QHash>
#include <QCache>

#include "../TanyatuCoreGlobal.h"
#include "../coreutils/Convenience.h"
#include "../interfaces/ISettingsReaderWriter.h"

namespace Tanyatu {

template< typename T>
struct typeinfo
{
    QString name() { return "_UNKNOWN_"; }
};

template<>
struct typeinfo< bool >
{
    QString name() { return "bool"; }
    QString toString( bool val ) { return QString::number( val ); }
    bool fromString( QString val )
    {
        return static_cast< bool >( val.toInt() );
    }
};

template<>
struct typeinfo< qint32 >
{
    QString name() { return "int"; }
    QString toString( qint32 val ) { return QString::number( val ); }
    qint32 fromString( QString val )
    {
        return static_cast< qint32 >( val.toLong() );
    }
};

template<>
struct typeinfo< quint32 >
{
    QString name() { return "uint"; }
    QString toString( quint32 val ) { return QString::number( val ); }
    quint32 fromString( QString val )
    {
        return static_cast< quint32 >( val.toULong() );
    }
};

template<>
struct typeinfo< qlonglong >
{
    QString name() { return "largeint"; }
    QString toString( qlonglong val ) { return QString::number( val ); }
    qlonglong fromString( QString val ) { return val.toLongLong(); }
};

template<>
struct typeinfo< qulonglong >
{
    QString name() { return "largeuint"; }
    QString toString( qulonglong val ) { return QString::number( val ); }
    qulonglong fromString( QString val ) { return val.toULongLong(); }
};

template<>
struct typeinfo< double >
{
    QString name() { return "realnum"; }
    QString toString( double val ) { return QString::number( val ); }
    long double fromString( QString val ) { return val.toDouble(); }
};

template<>
struct typeinfo< QString >
{
    QString name() { return "string"; }
    QString toString( QString val ) { return val; }
    QString fromString( QString val ) { return val; }
};


class SettingsManager
{
public:
    SettingsManager( ISettingsReaderWriter *readerWriter )
        : m_readerWriter( readerWriter )
        , m_settingCache( 500 )
    {

    }

    ~SettingsManager()
    {
        if( m_readerWriter != null ) {
            delete m_readerWriter;
        }
        m_settingCache.clear();
    }

    template< typename T >
    T get( QString key )
    {
        if( m_readerWriter != nullptr ) {
            StoredEntry *entry = m_settingCache.object( key );
            if( entry == nullptr ) {
                entry = m_readerWriter->get( key );
                if( entry != nullptr ) m_settingCache.insert( key, entry );
            }
            if( entry != nullptr ) {
                typeinfo< T > info;
                if( entry->type == info.name() ) {
                    return info.fromString( entry.value );
                }
            }
        }
        return T();
    }

    template< typename T >
    bool set( QString key, T value )
    {
        typeinfo< T > info;
        StoredEntry entry( key, info.name(), info.toString( value ));
        bool result = false;
        if( m_readerWriter != nullptr ) {
            result = m_readerWriter->set( entry );
            m_settingCache.insert( key, entry );
        }
        return result;
    }

    bool remove( QString key )
    {
        bool result = false;
        if( m_readerWriter->remove( key )) {
            m_settingCache.remove( key );
            result = true;
        }
        return result;
    }

private:
    ISettingsReaderWriter *m_readerWriter;

    QCache< QString, StoredEntry > m_settingCache;

};



}
