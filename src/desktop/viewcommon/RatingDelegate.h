/*******************************************************************************
 * RatingDeligate.h
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

#include <QStyledItemDelegate>

namespace GreenChilli { namespace Views {

class RatingDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RatingDelegate( int columnIndex,
                             bool editable = true,
                             QObject *parent = 0 );

protected:
    void paint( QPainter *painter,
          const QStyleOptionViewItem &option,
          const QModelIndex &index ) const;

    QWidget *createEditor( QWidget *parent,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const;

    void setEditorData( QWidget *editor,
                       const QModelIndex &index ) const;

    void setModelData( QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index ) const;

private:
    int m_columnIndex;

    bool m_isEditable;
    
};

} }
