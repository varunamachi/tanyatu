/*******************************************************************************
 * AddTrackDialog.cpp
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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <core/T.h>

#include "AddTrackDialog.h"
#include "../viewcommon/RatingDelegate.h"


namespace GreenChilli { namespace Components { namespace Playlists {

AddTrackDialog::AddTrackDialog( QWidget *parent )
    : QDialog( parent )
{
    m_proxyModel = new Views::TrackFilter( this );
//    m_proxyModel->setSourceModel( AUDIO_LIB() );
    m_view = new Views::CommonAudioTrackView(
                m_proxyModel,
                new Views::RatingDelegate( 4, false, this),
                this );
    m_view->setViewSelectionMode( QAbstractItemView::MultiSelection );
    m_searchBox = new Widgets::SearchBox( this );


    QPushButton *addButton = new QPushButton( tr( "Add" ), this );
    QPushButton *closeButton = new QPushButton( tr( "Close" ), this );
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget( addButton );
    buttonLayout->addWidget( closeButton );

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget( m_searchBox );
    layout->addWidget( m_view );
    layout->addLayout( buttonLayout );
    this->setLayout( layout );

    connect( m_searchBox, SIGNAL( textChanged( QString )),
             m_proxyModel, SLOT( setExpression( QString )));
    connect( addButton, SIGNAL( clicked() ),
             this, SLOT( accept()) );
    connect( closeButton, SIGNAL( clicked() ),
             this, SLOT( reject() )) ;
}


} } }

