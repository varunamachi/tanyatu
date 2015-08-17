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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include <core/T.h>
#include <uicommon/itemmodels/CommonTrackModel.h>

#include "AddTrackDialog.h"
#include "../viewcommon/RatingDelegate.h"


namespace GreenChilli { namespace Components { namespace Playlists {

AddTrackDialog::AddTrackDialog( QWidget *parent )
    : QDialog( parent )
{
    m_proxyModel = new Views::TrackFilter( this );
    m_trackModel = new Tanyatu::Ui::CommonTrackModel( 5, this );
    m_proxyModel->setSourceModel( m_trackModel );
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


void AddTrackDialog::refreshTracks()
{
    QList< Tanyatu::Data::StoredAudio *> *tracks
            = new QList< Tanyatu::Data::StoredAudio *>();
    AUDIO_LIB()->allItems( *tracks );
    m_trackModel->setTrackList( tracks );
}


} } }

