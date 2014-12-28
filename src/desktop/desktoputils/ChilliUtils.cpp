#include "ChilliUtils.h"

namespace GreenChilli { namespace Common {


void ChilliUtils::selectRow( QAbstractItemView *view, int row )
{
    QItemSelectionModel *selection  = view->selectionModel();
    QModelIndex colSelectionBegin = view->model()->index( row, 0 );
    QModelIndex colSelectionEnd =
            view->model()->index( row,
                                  view->model()->columnCount() - 1 );
    QItemSelection itemSel( colSelectionBegin, colSelectionEnd );
    selection->select ( itemSel, QItemSelectionModel::Clear );
    selection->select( itemSel, QItemSelectionModel::Select );
    view->scrollTo( view->model()->index( row, 0 ));
}

} }

