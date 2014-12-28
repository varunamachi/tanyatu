/*******************************************************************************
 * AddTrackDialog.h
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

#include <QDialog>

#include "../widgets//SearchBox.h"
#include "../viewcommon/CommonAudioTrackView.h"
#include "../viewcommon/TrackFilter.h"

namespace GreenChilli { namespace Components { namespace Playlists {

class AddTrackDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddTrackDialog( QWidget *parent = nullptr );

    std::size_t selectedTracks(
            QList< Tanyatu::Data::StoredAudio *> &tracksOut ) const
    {
        return m_view->selectedTracks( tracksOut );
    }

    void clear()
    {
        m_view->view()->selectionModel()->clear();
    }

private:
    Views::CommonAudioTrackView *m_view;

    Widgets::SearchBox *m_searchBox;

    Views::TrackFilter *m_proxyModel;

};

} } }

