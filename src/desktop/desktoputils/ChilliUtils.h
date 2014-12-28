#pragma once

#include <QtGlobal>
#include <QAbstractItemView>


namespace GreenChilli { namespace Common {

class ChilliUtils
{
public:
    static void selectRow( QAbstractItemView *view, int row );

private:
    ChilliUtils(){ }

    Q_DISABLE_COPY( ChilliUtils )
};


} }

