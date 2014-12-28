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
