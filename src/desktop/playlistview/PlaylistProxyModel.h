#pragma once

#include <QSortFilterProxyModel>

namespace GreenChilli { namespace Components { namespace Playlists {

class PlaylistProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit PlaylistProxyModel(QObject *parent = 0)
        : QSortFilterProxyModel( parent )
        , m_exp( "" ){}

    ~PlaylistProxyModel(){}
    
public slots:
    void setExpression( QString expression );

protected:
    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const;

    bool filterAcceptsRow( int source_row,
                           const QModelIndex &source_parent) const;

    QString m_exp;
};

} } }

