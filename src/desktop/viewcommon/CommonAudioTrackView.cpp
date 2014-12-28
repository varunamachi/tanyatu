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
