/*******************************************************************************
 * ChilliCache.h
 *
 * Copyright (c) 2013, Varuna L Amachi. All rights reserved.
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

#include <QPixmap>
#include <QHash>

#include "../TanyatuCommonGlobal.h"

namespace  Tanyatu { namespace Ui {

class TANYATU_GUI_EXPORT ChilliCache {
public:

    void insertPixmap( QString id, QPixmap *pixmap )
    {
        if( m_cache.contains( id ))     {
            delete m_cache.value( id );
        }
        m_cache.insert( id, pixmap );
    }

    void insertPixmap( QString id, QString resUrl )
    {
        QPixmap *pixmap = new QPixmap( resUrl );
        insertPixmap( id, pixmap );
    }

    const QPixmap* getPixmap( QString id )
    {
        return m_cache.value( id );
    }

    bool removePixmap( QString id )
    {
        bool result = false;
        if( QPixmap *pixmap = m_cache.value( id )) {
            m_cache.remove( id );
            delete pixmap;
            result = true;
        }
        return result;
    }

    static ChilliCache* get()
    {
        if( ! s_instance ) {
            s_instance = new ChilliCache();
        }
        return s_instance;
    }

    static void destroy()
    {
        delete s_instance;
    }

private:
    ChilliCache() {}

    ~ChilliCache()
    {
        qDeleteAll( m_cache.begin(), m_cache.end() );
    }

    static ChilliCache *s_instance;

    QHash< QString, QPixmap *> m_cache;
};

} }//end namespace GreenChilli

#define CHILLI_CACHE() Tanyatu::Ui::ChilliCache::get()
