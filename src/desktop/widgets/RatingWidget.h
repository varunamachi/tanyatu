/*******************************************************************************
 * RatingWidget.h
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
#include <QIcon>
#include <QPixmap>


namespace GreenChilli { namespace Widgets {

class RatingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RatingWidget( int numValues,
                           const QPixmap *unsetIcon,
                           const QPixmap *setIcon,
                           QWidget *parent = 0,
                           int initValue = 0 );

    virtual ~RatingWidget();

    QSize sizeHint() const;

    int rating() const;

    int floatingRating() const;

    static void paint( int value, QPainter* painter, int x, int y );

signals:
    void ratingSet( int newValue );

public slots:
    void setRating( int value );

protected:
    void paintEvent( QPaintEvent *event );

    void mouseMoveEvent( QMouseEvent * event );

    void mouseReleaseEvent( QMouseEvent *event );

    void leaveEvent( QEvent *event );

private:
    const QPixmap *m_unsetIcon;

    const QPixmap *m_setIcon;

    int m_prevValue;

    int m_flotingValue;

    int m_curValue;

    int m_numValues;

};

} }

