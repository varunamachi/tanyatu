#pragma once

#include <QAbstractItemModel>
#include <core/data/StoredAudio.h>

#include "../TanyatuCommonGlobal.h"

namespace Tanyatu { namespace Ui {

class TANYATU_GUI_EXPORT MediaItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MediaItemModel( int numColumns, QObject *parent = 0 );

    Data::MediaItem * itemAt( int index );

    ~MediaItemModel();

    void setItemList( const QList< Data::MediaItem *> *itemList );

    const QList< Data::MediaItem *> * getItemList();

    void setNumColumns( int numColums );

    virtual QVariant dataForTrack( Data::MediaItem *item,
                                   int column ) const = 0;

    virtual QString headerForColumn( int column ) const = 0;

    int rowCount( const QModelIndex &parent ) const;

    int columnCount( const QModelIndex &parent ) const;

    QVariant data( const QModelIndex &index, int role ) const;

    QVariant headerData( int section,
                         Qt::Orientation orientation,
                         int role ) const;

    QModelIndex index( int row, int column, const QModelIndex &parent ) const;

    QModelIndex parent( const QModelIndex &child ) const;

    bool hasChildren( const QModelIndex &parent ) const;


public slots:
    void clear();

private:
    int m_columnCount;

    const QList< Data::MediaItem *> *m_items;
};

} }
