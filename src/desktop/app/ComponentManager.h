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

