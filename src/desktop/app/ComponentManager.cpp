/*******************************************************************************
 * ComponentManager.cpp
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

#include <QHBoxLayout>
#include <QSplitter>
#include <QIcon>

#include "ComponentManager.h"

using namespace GreenChilli;

ComponentManager *ComponentManager::s_instance = 0;


void ComponentManager::destroy()
{
    if( s_instance ) {
        delete s_instance;
    }
}


ComponentManager* ComponentManager::get()
{
    if( ! s_instance ) {
        s_instance = new ComponentManager();
    }
    return s_instance;
}



ComponentManager::ComponentManager( QWidget *parent ) :
    QWidget( parent )
{
    setupUi();
}



ComponentManager::~ComponentManager()
{
    m_nameToIndex.clear();
    qDeleteAll( m_components.begin(), m_components.end() );
}


void ComponentManager::setupUi()
{
    m_mainWidget = new QStackedWidget( this );
    m_buttonLayout = new QHBoxLayout();
    QHBoxLayout *winlayout = new QHBoxLayout();

    QHBoxLayout *topLayout = new QHBoxLayout();
    m_buttonLayout->addStretch();

    QPushButton *closeButton = new QPushButton( QIcon( ":/images/clear_act" ),
                                                "",
                                                this );
    closeButton->setFlat( true );
    closeButton->setFixedSize( QSize( 20, 20 ));
    closeButton->setToolTip( tr( "Exit" ));
    closeButton->setContentsMargins( QMargins() );

    QPushButton *minimizeButton = new QPushButton(
                QIcon( ":/images/remove_act" ),
                "",
                this );
    minimizeButton->setFlat( true );
    minimizeButton->setFixedSize( QSize( 24, 24 ));
    minimizeButton->setToolTip( tr( "Exit" ));
    minimizeButton->setContentsMargins( QMargins() );

    winlayout->addWidget( minimizeButton );
    winlayout->addWidget( closeButton );
    winlayout->setContentsMargins( QMargins() );
    winlayout->setSpacing( 0 );

    topLayout->addLayout( m_buttonLayout );
    topLayout->addStretch();
    topLayout->addLayout( winlayout );


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout( topLayout );
    mainLayout->addWidget( m_mainWidget );
    mainLayout->setContentsMargins( QMargins() );
    mainLayout->setSpacing( 0 );

    m_mainWidget->setContentsMargins( 0, 0, 0, 0 );
    m_buttonLayout->setContentsMargins( 4, 2, 0, 0 );
    m_buttonLayout->setSpacing( 5 );
    this->setLayout( mainLayout );

    connect( closeButton, SIGNAL( released() ),
             this, SIGNAL( exitRequested() ));
    connect( minimizeButton, SIGNAL( released() ),
             this, SIGNAL( minimizeReqested() ));
}


QList< ChilliComponent *> ComponentManager::allComponents() const
{
    return m_components.values();
}


const ChilliComponent* ComponentManager::component( QString componentId ) const
{
    return m_components.value( componentId );
}


const ChilliComponent *ComponentManager::currentComponent() const
{
    QWidget *widget = m_mainWidget->currentWidget();
    if( widget ) {
        return static_cast< ChilliComponent *>( widget );
    }
    return 0;
}


void ComponentManager::addComponent( GreenChilli::ChilliComponent *component )
{
    if( component && ! m_components.contains( component->componentId() )) {
        m_components.insert( component->componentId(), component );
        IndexButton *button = new IndexButton( m_components.size() - 1,
                                               component->componentName(),
                                               this );
        m_buttons.append( button );
        m_nameToIndex.insert( component->componentId(),
                              m_components.size() - 1 );
        m_buttonLayout->insertWidget( m_buttonLayout->count() - 1, button );
        connect( button, SIGNAL( activated( int )),
                 this, SLOT( onIndexSelected( int )));
        m_mainWidget->addWidget( component );
        emit componentAdded( component );
        if( m_components.size() == 1 ) {
            button->setChecked( true );
             m_mainWidget->setCurrentIndex( 0 );
        }
    }
}


void ComponentManager::selectComponent( QString componentId )
{
    if( m_nameToIndex.contains( componentId )) {
        int index = m_nameToIndex.value( componentId );
        m_buttons.at( m_mainWidget->currentIndex() )->setChecked( false );
        m_buttons.at( index )->setChecked( true );
        m_mainWidget->setCurrentIndex( index );
        emit componentSelected( m_components.value( componentId ));
    }
}


void ComponentManager::onIndexSelected( int row )
{
    m_buttons.at( m_mainWidget->currentIndex() )->setChecked( false );
    m_mainWidget->setCurrentIndex( row );
}
