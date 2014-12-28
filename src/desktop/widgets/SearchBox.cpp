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
