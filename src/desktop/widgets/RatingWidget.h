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

