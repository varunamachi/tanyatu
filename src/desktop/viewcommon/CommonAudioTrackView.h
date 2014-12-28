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
