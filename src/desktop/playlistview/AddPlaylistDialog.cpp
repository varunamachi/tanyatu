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

#include "AddPlaylistDialog.h"

namespace GreenChilli { namespace Components { namespace Playlists {


AddPlaylistDialog::AddPlaylistDialog( QWidget *parent )
    : QDialog( parent )
{
    QHBoxLayout *txtLayout = new QHBoxLayout();
    m_nameEdit = new QLineEdit( this );
    QRegExpValidator validator( QRegExp( "A-Za-z[A-Za-z0-9]{8}" ));
    m_nameEdit->setValidator( &validator );
    QLabel *txtLabel = new QLabel( this );
    txtLabel->setText( tr( "Playlist Name" ));
    txtLayout->addWidget( txtLabel );
    txtLayout->addWidget( m_nameEdit );

    m_messageLabel = new QLabel( this );
    m_messageLabel->setText( "" );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton( this );
    m_okButton->setText( "Create" );
    m_okButton->setEnabled( false );
    m_cancelButton = new QPushButton( this );
    m_cancelButton->setText( "Cancel" );
    buttonLayout->addStretch();
    buttonLayout->addWidget( m_okButton );
    buttonLayout->addWidget( m_cancelButton );

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout( txtLayout );
    mainLayout->addWidget( m_messageLabel );
    mainLayout->addLayout( buttonLayout );
    this->setLayout( mainLayout );

    connect( m_okButton, SIGNAL( clicked() ),
             this, SLOT( accept() ));
    connect( m_cancelButton, SIGNAL( clicked() ),
             this, SLOT( reject() ));
    connect( m_nameEdit, SIGNAL( textEdited( QString )),
             this, SLOT( onNameChanged() ));


}

void AddPlaylistDialog::onNameChanged()
{
    QString newName = m_nameEdit->text();
    if( PL_MANAGER()->hasPlaylist( newName )) {
        m_okButton->setEnabled( false );
        m_messageLabel->setText( QString( "<font color=red>" )
                + tr( "A playlist with given name already exists" )
                + "</font>" );
    }
    else {
        m_okButton->setEnabled( true );
        m_messageLabel->setText( "" );
    }
}

} } }
