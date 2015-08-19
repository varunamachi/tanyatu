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

#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QMargins>


class QSizeGrip;
class QHBoxLayout;

namespace GreenChilli
{

class PlaylistWidget;
class AudioPlayerWidget;


class ChilliMainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChilliMainWidget( QWidget *parent = 0 );

public slots:
    void onAboutToQuit();

    void setRoundedRect( bool useRoundedRect )
    {
        m_roundedRect = useRoundedRect;
    }

protected:
    void paintEvent( QPaintEvent *event );

private:
    QString createStyleSheet();

    QPushButton *m_maxRestore;

    QIcon *m_maximizeIcon;

    QIcon *m_restoreIcon;

    QWidget *m_leftWidget;

    PlaylistWidget *m_playlist;

    AudioPlayerWidget *m_audioPlayer;

    bool m_roundedRect;

    QSizeGrip *m_sizeGrip;

    friend class ChilliWindow;

};


class ChilliWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChilliWindow( QWidget *parent = 0 );

private slots:
    void onMaximizeRestore();

    void onMinimize();

protected:
    void mouseMoveEvent( QMouseEvent* event );

    void mousePressEvent( QMouseEvent* event );

    void mouseReleaseEvent( QMouseEvent* event );

    void showEvent( QShowEvent *evt );

    void resizeEvent( QResizeEvent *evt );

private:
    void maximize();

    void restore();

    void minimize();

    ChilliMainWidget *m_chilliWidget;


    bool m_moving;

    bool m_maximised;

    QPoint m_lastMousePosition;

    QByteArray m_geometry;

    QHBoxLayout *m_layout;

    QWidget *m_containerWidget;

};


}
