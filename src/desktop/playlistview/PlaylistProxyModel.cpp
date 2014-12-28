#include <core/data/SavedPlaylist.h>
#include <core/T.h>

#include "PlaylistProxyModel.h"

namespace GreenChilli { namespace Components { namespace Playlists {


void PlaylistProxyModel::setExpression( QString expression )
{
    m_exp = expression;
    invalidateFilter();
}


bool PlaylistProxyModel::lessThan( const QModelIndex &left,
                                   const QModelIndex &right ) const
{
    QVariant lData = left.data();
    QVariant rData = right.data();
    bool ok;
    int leftInt = lData.toInt( &ok );
    int rightInt = rData.toInt( &ok );
    return ok ? leftInt < rightInt
              : QString::localeAwareCompare( lData.toString(),
                                             rData.toString()) < 0;
}



bool PlaylistProxyModel::filterAcceptsRow(
        int source_row,
        const QModelIndex &source_parent ) const
{
    Q_UNUSED( source_parent )
    QString playlistName = sourceModel()->data(
                sourceModel()->index( source_row, 0 )).toString();
    Tanyatu::Data::SavedPlaylist *pl = PL_MANAGER()->playlist( playlistName );
    return pl ? pl->name().contains(m_exp ) : false;
}


} } }
