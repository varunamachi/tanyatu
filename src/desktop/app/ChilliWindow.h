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

class QSizeGrip;

namespace GreenChilli
{

class PlaylistWidget;
class AudioPlayerWidget;

class ChilliWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChilliWindow(QWidget *parent = 0);

public slots:
    void onAboutToQuit();

protected:
    void mouseMoveEvent(QMouseEvent* event);

    void mousePressEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void showEvent( QShowEvent *evt );

    void resizeEvent( QResizeEvent *evt );

private slots:
    void onMaximizeRestore();

    void onMinimize();

private:
    void maximize();

    void restore();

    void minimize();

    QString createStyleSheet();

    QPushButton *m_maxRestore;

    QByteArray m_geometry;

    QPoint m_lastMousePosition;

    QIcon *m_maximizeIcon;

    QIcon *m_restoreIcon;

    QWidget *m_leftWidget;

    PlaylistWidget *m_playlist;

    AudioPlayerWidget *m_audioPlayer;

    QSizeGrip *m_sizeGrip;

    bool m_moving;

    bool m_maximised;
};
}
