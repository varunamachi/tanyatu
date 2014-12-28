/*******************************************************************************
 * ComponentManager.h
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

#include <QWidget>
#include <QHash>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>

#include "../widgets/OrientationButton.h"
#include "ChilliComponent.h"

namespace GreenChilli {

class IndexButton : public QPushButton
{
    Q_OBJECT
public:
    IndexButton( int index, QString text, QWidget *parent = 0 ) :
        QPushButton( parent ),
        m_index( index )
    {
        setCheckable( true );
        setStyleSheet(
                    " QPushButton {"
                    "     border-radius: 5px;"
                    "     background-color: regba( 32, 32, 32, 200 );"
                    "     min-width: 80px;"
                    "     min-height: 20px;"
                    "     font-size: 10px;"
                    " }"
                    " QPushButton:checked {"
                    "     background-color: #FFA858;"
                    "     color: #202020;"
                    " }"
         );
        setText( text );
    }

protected:
    void mousePressEvent( QMouseEvent *evt )
    {
        if( ! isChecked() ) {
            QPushButton::mousePressEvent( evt );
            emit activated( m_index );
        }
        evt->ignore();
    }

signals:
    void activated( int index );

private:
    int m_index;
};

class ComponentManager : public QWidget
{
    Q_OBJECT
public:
    static void destroy();

    static ComponentManager* get();

    QList< ChilliComponent *> allComponents() const;

    const ChilliComponent* component( QString componentId ) const;

    const ChilliComponent* currentComponent() const;
    
signals:
    void componentSelected( const GreenChilli::ChilliComponent *component );

    void componentAdded( const GreenChilli::ChilliComponent *component );

    void exitRequested();

    void minimizeReqested();


public slots:
    void addComponent( GreenChilli::ChilliComponent *component );

    void selectComponent( QString componentId );

private slots:
    void onIndexSelected( int row );

private:
    explicit ComponentManager( QWidget *parent = 0 );

    virtual ~ComponentManager();

    void setupUi();

    static ComponentManager *s_instance;

    QHash< QString, ChilliComponent *> m_components;

    QList< IndexButton *> m_buttons;

    QHash< QString, int > m_nameToIndex;

    QStackedWidget *m_mainWidget;

    QHBoxLayout *m_buttonLayout;
};
} // namespace GreenChilli

