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

    QHBoxLayout *topLayout = new QHBoxLayout();
    m_buttonLayout->addStretch();

    topLayout->addLayout( m_buttonLayout );
    topLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout( topLayout );
    mainLayout->addWidget( m_mainWidget );
    mainLayout->setContentsMargins( QMargins() );
    mainLayout->setSpacing( 0 );

    m_mainWidget->setContentsMargins( 0, 0, 0, 0 );
    m_buttonLayout->setContentsMargins( 4, 2, 0, 0 );
    m_buttonLayout->setSpacing( 5 );
    this->setLayout( mainLayout );
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
