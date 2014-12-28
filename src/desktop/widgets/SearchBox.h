/*******************************************************************************
 * SearchBox.h
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

#include <QLineEdit>
#include <QIcon>
#include <QToolButton>

namespace GreenChilli { namespace Widgets {

class SearchBox : public QLineEdit
{
    Q_OBJECT

public:
    explicit SearchBox( QWidget *parent = 0 );
    
public slots:
    void setClearImage( const QIcon &icon );

protected:
    void resizeEvent( QResizeEvent *event );

private slots:
    void updateCloseButton( const QString &text );

private:
    QToolButton *m_clearButton;
};

} }
