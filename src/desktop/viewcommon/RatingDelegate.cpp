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
