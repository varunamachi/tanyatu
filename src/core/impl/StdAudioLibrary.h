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
