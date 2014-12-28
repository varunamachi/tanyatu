/*******************************************************************************
 * ChilliWindow.cpp
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
