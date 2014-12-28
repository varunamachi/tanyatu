/*******************************************************************************
 * StdAudioLibrary.h
 *
 * Copyright (c) 2012, Varuna L Amachi. All rights reserved.
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

#include "../interfaces/IAudioLibrary.h"
#include "../coreutils/Convenience.h"


namespace Tanyatu { namespace Impl {


class TANYATU_CORE_EXPORT StdAudioLibrary : public IAudioLibrary
{
    Q_OBJECT

public:
    void add( Tanyatu::Data::StoredAudio *item );

    void add( QList< Tanyatu::Data::StoredAudio *>& items);

    void remove( QString id );

    void remove( QList< QString > ids );

    void rescan();

    void setRating( QString id, Tanyatu::Data::MediaRating rating );

    void updateItem( QString id, Tanyatu::Data::AudioTrack &updateInfo );

    void onItemPlaybackStarted( Tanyatu::Data::MediaItem *item );

    void onItemPlaybackFinished( Tanyatu::Data::MediaItem *item );

    void tagItem( QString id, QString tag );

    void removeItemTag( QString id, QString tag );

    void clearLibrary();

    explicit StdAudioLibrary(QObject *parent = 0);

    virtual ~StdAudioLibrary();

    quint32 itemCount() const;

    bool hasItem( const QString itemId );

    Data::StoredAudio *item( QString id ) const;

    quint32 items( QList< QString > &ids, QHash< QString,
                                        Data::StoredAudio *>& itemsOut ) const;

    quint32 items( QList< QString >& ids,
                           QList< Data::MediaItem *> &itemsOut ) const;

    quint32 items( QList< QString >& ids,
                           QList< Data::StoredAudio *> &itemsOut ) const;

//    Data::StoredAudio *item( int index ) const;

    quint32 allItems( QList< Tanyatu::Data::StoredAudio *> &out ) const;

    void setValid(bool value);

    QString uniqueName() const;

    QString module()  const;

    QString displayName() const;

    bool init();

    static QString COMPONENT_NAME;

    static QString COMPONENT_MODULE;

protected:

    QHash< QString, Data::StoredAudio *> m_tracks;

    bool m_valid;
};

} } //End of namespaces
