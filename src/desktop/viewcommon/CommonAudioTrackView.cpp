/*******************************************************************************
 * AudioTrackView.cpp
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
#include <cstdint>

#include <QMenu>
#include <QPoint>
#include <QAbstractItemModel>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QContextMenuEvent>
#include <QHBoxLayout>

#include <core/T.h>

#include "CommonAudioTrackView.h"

namespace GreenChilli { namespace Views {

CommonAudioTrackView::CommonAudioTrackView( QAbstractItemModel *model,
                                QAbstractItemDelegate *delegate,
                                QWidget *parent )
    : QWidget( parent )
    , m_contextMenu( 0 )
{
    m_view = new QTreeView( this );
    setItemModel( model );
    if( delegate != nullptr ) {
        m_view->setItemDelegate( delegate );
    }
    m_view->setContentsMargins( QMargins() );

    connect( m_view,
             SIGNAL( activated( QModelIndex )),
             this,
             SLOT( onViewActivated( QModelIndex )));
    connect( m_view->selectionModel(),
             SIGNAL( selectionChanged( QItemSelection, QItemSelection )),
             this,
             SIGNAL( uiStateChanged() ));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget( m_view );
    layout->setContentsMargins( QMargins() );
    layout->setSpacing( 0 );
    this->setLayout( layout );
}



std::size_t CommonAudioTrackView::selectedTracks(
        TNT_OUT QList< Tanyatu::Data::StoredAudio *> &tracksOut ) const
{
    std::size_t numSelected = 0;
    if( ! m_view->model() ) {
        return numSelected;
    }
    QModelIndexList indexList = m_view->selectionModel()->selectedRows( 0 );
    if( ! indexList.isEmpty() ) {
        foreach( const QModelIndex &index, indexList ) {
            Tanyatu::Data::StoredAudio *track =
                    AUDIO_LIB()->item( index.data().toString() );
            if( track ) {
                tracksOut.append( track );
                ++ numSelected;
            }
        }
    }
    return numSelected;
}


std::size_t CommonAudioTrackView::selectedItems(
        TNT_OUT QList<Tanyatu::Data::MediaItem *> &tracksOut ) const
{
    std::size_t numSelected = 0;
    if( ! m_view->model() ) {
        return numSelected;
    }
    QModelIndexList indexList = m_view->selectionModel()->selectedRows( 0 );
    if( ! indexList.isEmpty() ) {
        foreach( const QModelIndex &index, indexList ) {
            Tanyatu::Data::MediaItem *track = AUDIO_LIB()->item(
                                                      index.data().toString() );
            if( track ) {
                tracksOut.append( track );
                ++ numSelected;
            }
        }
    }
    return numSelected;
}


std::size_t CommonAudioTrackView::selectedIndexes(
        TNT_OUT QList< int > &indicesOut ) const
{
    std::size_t numSelected = 0;
    QModelIndexList selection = m_view->selectionModel()->selectedRows();
    foreach( QModelIndex index, selection ) {
        indicesOut.append( index.row() );
        ++ numSelected;
    }
    return numSelected ;
}


void CommonAudioTrackView::contextMenuEvent( QContextMenuEvent *event )
{
    if( m_contextMenu ) {
        m_contextMenu->exec( event->globalPos() );
    }
}


void CommonAudioTrackView::onViewActivated( const QModelIndex &index )
{
    if( m_view->model() && index.isValid() ) {
        QModelIndex zeroIndex = m_model->index( index.row(), 0 );
        emit activated( zeroIndex.data().toString() );
    }
}


void CommonAudioTrackView::setItemModel( QAbstractItemModel *model )
{
    if( model ) {
        m_model = model;
        m_view->setModel( model );
        m_view->setSortingEnabled( true );
        m_view->setWordWrap( false );
        m_view->setAutoScroll( true );
        m_view->header()->setSectionResizeMode( QHeaderView::Stretch );
        if( model->columnCount() >= 5 ) {
            m_view->setColumnWidth( 4, 16 );
        }
        m_view->header()->setStretchLastSection( false );
        m_view->hideColumn( 0 );
        m_view->setSelectionMode( QAbstractItemView::ExtendedSelection );
        m_view->setSelectionBehavior( QAbstractItemView::SelectRows );
        m_view->setFocusPolicy( Qt::WheelFocus );
        m_view->setEditTriggers( QAbstractItemView::CurrentChanged );
        m_view->setAlternatingRowColors(true);
        m_view->setFont( QFont( m_view->font().defaultFamily(), 8 ));
    }
}

} }
