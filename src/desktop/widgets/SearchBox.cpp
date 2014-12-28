/*******************************************************************************
 * SearchBox.cpp
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

#include <QStyle>
#include <QtGlobal>

#include "SearchBox.h"


namespace GreenChilli { namespace Widgets {

/* XPM */
static const char *clearIcon[] = {
    /* columns rows colors chars-per-pixel */
    "32 32 9 1 ",
    "  c #FF7112",
    ". c #FF7113",
    "X c #FF7213",
    "o c #FF7014",
    "O c #FF7114",
    "+ c #FF7214",
    "@ c #FF761C",
    "# c #FF771C",
    "$ c None",
    /* pixels */
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$+$$$$$$$$$$$$X$$$$$$$$$",
    "$$$$$$$$@#X$$$$$$$$$$X@@$$$$$$$$",
    "$$$$$$$@###X$$$$$$$$X@##@$$$$$$$",
    "$$$$$$@#####+$$$$$$+####@@$$$$$$",
    "$$$$$X@@#####+$$$$+#######+$$$$$",
    "$$$$$$X@######+$$+#######X$$$$$$",
    "$$$$$$$X######@++@######X$$$$$$$",
    "$$$$$$$$+@######@######+$$$$$$$$",
    "$$$$$$$$$+@@####@@###@+$$$$$$$$$",
    "$$$$$$$$$$+@########@+$$$$$$$$$$",
    "$$$$$$$$$$$+########+$$$$$$$$$$$",
    "$$$$$$$$$$$+########+$$$$$$$$$$$",
    "$$$$$$$$$$+#########@+$$$$$$$$$$",
    "$$$$$$$$$+####@#######+$$$$$$$$$",
    "$$$$$$$$+@####@@#####@@+$$$$$$$$",
    "$$$$$$$X#######++@######X$$$$$$$",
    "$$$$$$X@####@@+$$+######@X$$$$$$",
    "$$$$$+######@+$$$$+#####@#X$$$$$",
    "$$$$$$@#####+$$$$$$+#####@$$$$$$",
    "$$$$$$$@###X$$$$$$$$X#@@@$$$$$$$",
    "$$$$$$$$##X$$$$$$$$$$X#@$$$$$$$$",
    "$$$$$$$$$X$$$$$$$$$$$$+$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$",
    "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"
};


SearchBox::SearchBox( QWidget *parent) :
    QLineEdit( parent )
{
    m_clearButton = new QToolButton( this );
    m_clearButton->setIcon( QPixmap( clearIcon ));
    m_clearButton->setCursor( Qt::ArrowCursor );
    m_clearButton->setStyleSheet(
                "QToolButton { "
                    "background-color: rgba( 0xFF, 0xFF, 0xFF, 0); "
                    "border: none; padding: 0px; "
                "}" );
    m_clearButton->hide();

    connect( m_clearButton,
             SIGNAL( clicked() ),
             this,
             SLOT( clear() ));
    connect( this,
             SIGNAL( textChanged( const QString& )),
             this,
             SLOT( updateCloseButton( const QString& )));

    int frameWidth = style()->pixelMetric( QStyle::PM_DefaultFrameWidth );
    setStyleSheet( QString( "QSearchBox { "
                                "padding-right: %1px; "
                            "} " )
                   .arg( m_clearButton->sizeHint().width() + frameWidth + 1 ));
    QSize msz = minimumSizeHint();
    setMinimumSize(
                qMax( msz.width(),
                      m_clearButton->sizeHint().height() + frameWidth * 2 + 2 ),
                qMax( msz.height(),
                      m_clearButton->sizeHint().height() + frameWidth * 2 + 2));
}


void SearchBox::setClearImage( const QIcon &icon )
{
    m_clearButton->setIcon( icon );
}


void SearchBox::resizeEvent(QResizeEvent *)
{
    QSize sz = m_clearButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    m_clearButton->move( ( rect().right() - frameWidth - sz.width() ),
                         ( rect().bottom() + 1 - sz.height() ) / 2);
}


void SearchBox::updateCloseButton(const QString& text)
{
    m_clearButton->setVisible(!text.isEmpty());
}

} }
