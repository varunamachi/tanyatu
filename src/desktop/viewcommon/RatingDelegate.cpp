/*******************************************************************************
 * RatingDeligate.cpp
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

#include <QPainter>
#include <QApplication>
#include <uicommon/uiutils/ChilliCache.h>

#include "../widgets/RatingWidget.h"
#include "RatingDelegate.h"

namespace GreenChilli { namespace Views {

RatingDelegate::RatingDelegate( int columnIndex,
                                bool editable,
                                QObject *parent )
    : QStyledItemDelegate(parent)
    , m_columnIndex( columnIndex )
    , m_isEditable( editable )
{
}


void RatingDelegate::paint( QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    if( index.column() != m_columnIndex ) {
        painter->setBackgroundMode(Qt::TransparentMode);
        QStyledItemDelegate::paint(painter, option, index);
    }
    else {
        const QPixmap *empty = CHILLI_CACHE()->getPixmap( "star_empty" );
        const QPixmap *filled = CHILLI_CACHE()->getPixmap( "star_filled" );
        painter->save();
        int rating = index.data().toInt();
        if( option.state & QStyle::State_Selected ) {
            painter->fillRect( option.rect, QColor( 0xFF, 0xA8, 0x58, 200 ));
        }
        else if( option.state & QStyle::State_MouseOver ) {
            painter->fillRect( option.rect, QColor( 0xFF, 0xA8, 0x58, 130 ));
        }
        for( int i = 0; i < 5; ++i ) {
            if( i < rating ) {
                painter->drawPixmap( option.rect.x()
                                            + i * filled->width(),
                                     option.rect.y(),
                                     *filled );
            }
            else {
                painter->drawPixmap( option.rect.x()
                                            + i  * empty->width(),
                                     option.rect.y() ,
                                     *empty );
            }
        }
        painter->restore();

    }
}



QWidget* RatingDelegate::createEditor( QWidget *parent,
                                       const QStyleOptionViewItem &option,
                                       const QModelIndex &index ) const
{
    if( ! m_isEditable ) return nullptr;
    QWidget *star = nullptr;
    if( index.column() == m_columnIndex ) {
        star = new Widgets::RatingWidget(
                    5,
                    CHILLI_CACHE()->getPixmap( "star_empty" ),
                    CHILLI_CACHE()->getPixmap( "star_filled" ),
                    parent,
                    0 );
    }
    else {
        star = QStyledItemDelegate::createEditor( parent, option, index );
    }
    return star;
}



void RatingDelegate::setEditorData(QWidget *editor,
                                   const QModelIndex &index) const
{
    if( m_isEditable && index.column() == m_columnIndex ) {
        int rating = index.data().toInt();
        Widgets::RatingWidget *widget =
                qobject_cast< Widgets::RatingWidget *>( editor );
        widget->setRating( rating );
    }
    else {
        QStyledItemDelegate::setEditorData( editor, index );
    }
}


void RatingDelegate::setModelData( QWidget *editor,
                                   QAbstractItemModel *model,
                                   const QModelIndex &index ) const
{
    if( m_isEditable && index.column() == m_columnIndex ) {
        Widgets::RatingWidget *widget =
                qobject_cast< Widgets::RatingWidget *>( editor );
        model->setData( index, widget->floatingRating() );
    }
    else {
        QStyledItemDelegate::setModelData( editor, model, index );
    }
}

} }
