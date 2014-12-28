/*******************************************************************************
 * AudioBrowserSortFilter.h
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

#include <QSortFilterProxyModel>
#include <QRegExp>

namespace GreenChilli { namespace Components {

class AudioBrowserSortFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit AudioBrowserSortFilter( QObject *parent = 0 );

    ~AudioBrowserSortFilter();
    
public slots:
    void setExpression( QString expression );

protected:
    bool lessThan( const QModelIndex &left,
                   const QModelIndex &right ) const;

    QString m_exp;
};


class ArtistSortFilter : public AudioBrowserSortFilter
{
protected:
    bool filterAcceptsRow( int source_row,
                           const QModelIndex &source_parent ) const;

};


class AlbumSortFilter : public AudioBrowserSortFilter
{
protected:
    bool filterAcceptsRow( int source_row,
                           const QModelIndex &source_parent) const;
};

class BrowserTrackSortFilter : public AudioBrowserSortFilter
{
protected:
    bool filterAcceptsRow( int source_row,
                           const QModelIndex &source_parent) const;
};


} }

