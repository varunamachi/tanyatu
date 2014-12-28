/*******************************************************************************
 * AudioTrackView.h
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

#include <QWidget>
#include <QTreeView>
#include <QStringList>

#include <core/data/StoredAudio.h>
#include <core/coreutils/Convenience.h>

class QMenu;
class QPoint;
class QAbstractItemModel;

namespace GreenChilli { namespace Views {

class CommonAudioTrackView : public QWidget
{
    Q_OBJECT
public:
    explicit CommonAudioTrackView( QAbstractItemModel *model = 0,
                             QAbstractItemDelegate *delegate = 0,
                             QWidget *parent = 0 );

    std::size_t selectedTracks(
            TNT_OUT QList< Tanyatu::Data::StoredAudio *> &tracksOut ) const;

    std::size_t selectedItems(
            TNT_OUT QList< Tanyatu::Data::MediaItem *> &itemsOut ) const;

    std::size_t selectedIndexes( TNT_OUT QList< int > &indicesOut ) const;

    void setItemModel( QAbstractItemModel *model );

    void setViewDelegate( QAbstractItemDelegate *delegate )
    {
        m_view->setItemDelegate( delegate );
    }

    void selectAll()
    {
        m_view->selectAll();
    }

    void updateView()
    {
        m_view->repaint();
    }

    const QTreeView * view() const
    {
        return m_view;
    }
    
signals:
    void activated( QString trackId );

    void uiStateChanged();

public slots:
    void setContextMenu( QMenu *menu )
    {
        m_contextMenu = menu;
    }

    void setViewSelectionMode( QAbstractItemView::SelectionMode mode )
    {
        m_view->setSelectionMode( mode );
    }

protected:
    void contextMenuEvent( QContextMenuEvent *event );

private slots:
    void onViewActivated( const QModelIndex &index );

private:
    QTreeView *m_view;

    QMenu *m_contextMenu;

    QAbstractItemModel *m_model;
};

} } // namespace GreenChilli
