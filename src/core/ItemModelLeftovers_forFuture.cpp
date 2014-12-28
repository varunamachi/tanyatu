// __START__( IAudioLibrary )



int StandardAudioLibrary::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return m_itemList->size();
}


int StandardAudioLibrary::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 5;
}


QVariant StandardAudioLibrary::data( const QModelIndex &index, int role ) const
{
    if( ! index.isValid() || index.row() >= m_tracks.size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Model::StoredAudio *source = m_itemList->at( index.row() );
        switch( index.column() ) {
        case 0: return source->id();
        case 1: return source->title();
        case 2: return source->artist();
        case 3: return source->album();
        case 4: return int( source->rating() );
        }
    }
    else if( role == Qt::ToolTipRole ) {
        Model::StoredAudio *source = m_itemList->at( index.row() );
        QString tooltip = QString() +
                tr( "Title  : <b>" ) + source->title()  + "</b><br>" +
                tr( "Artist : <b>" ) + source->artist() + "</b><br>" +
                tr( "Album  : <b>" ) + source->album()  + "</b>" ;
        return tooltip;
    }
    //??
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    return QVariant();
}


QVariant StandardAudioLibrary::headerData( int section,
                                           Qt::Orientation orientation,
                                           int role ) const
{
    Q_UNUSED( orientation )
    if ( role == Qt::TextAlignmentRole ) {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        switch( section ) {
        case 0: return tr( "Id" );
        case 1: return tr( "Title" );
        case 2: return tr( "Artist" );
        case 3: return tr( "Album" );
        case 4: return tr( "Rating" );
        }
    }
    return QVariant();
}


QModelIndex StandardAudioLibrary::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex StandardAudioLibrary::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool StandardAudioLibrary::hasChildren( const QModelIndex &parent ) const
{
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


Qt::ItemFlags StandardAudioLibrary::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 4 ) {
        return flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool StandardAudioLibrary::setData( const QModelIndex &index,
                                    const QVariant &value,
                                    int role )
{
    Q_UNUSED( role );
    if( index.column() == 4 ) {
        Model::StoredAudio *item = m_itemList->at( index.row() );
        setRating( item->id(), Utils::getRatingFromInt( value.toInt() ));
        return true;
    }
    return false;
}
// __END__( IAudioLibrary )

//__START__( IHistoryManager )
int StdHistoryManager::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return m_history.size();
}

int StdHistoryManager::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent );
    return 5;
}


QVariant StdHistoryManager::data(const QModelIndex &index, int role) const
{
    if( ! index.isValid() || index.row() >= m_history.size() ) {
        return QVariant();
    }
    if( role == Qt::DisplayRole ) {
        QPair< QString, QDateTime > *history = m_history.at( index.row() );
        Model::StoredAudio *track = T::get()->audioLib()->item(
                    history->first );
        switch( index.column() ) {
        case 0: return track->id();
        case 1: return track->title();
        case 2: return track->artist();
        case 3: return track->album();
        case 4: return history->second.toString();
        }
    }
    else if( role == Qt::TextAlignmentRole ) {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    else if( role == Qt::FontRole ) {
        return QFont( "Droid Sans", 9 );
    }
    return QVariant();
}

QVariant StdHistoryManager::headerData(
        int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED( orientation );
    if( role == Qt::DisplayRole ) {

        switch( section ) {
        case 0: return tr("id");
        case 1: return tr("Title");
        case 2: return tr("Artist");
        case 3: return tr("Album");
        case 4: return tr("Time");
        }
    }
    return QVariant();
}


QModelIndex StdHistoryManager::index(
        int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    return createIndex( row, column );
}


QModelIndex StdHistoryManager::parent(const QModelIndex &child) const
{
    Q_UNUSED( child );
    return QModelIndex();
}


bool StdHistoryManager::hasChildren(const QModelIndex &parent) const
{
    if( ! parent.isValid() ) {
        return true;
    }
    return false;
}



//__END__( IHistoryManager )

//__START__( IPlaylistManager )

int StdPlaylistManager::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return m_itemList->size();
}


int StdPlaylistManager::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED( parent );
    return 4;
}


QVariant StdPlaylistManager::data(
        const QModelIndex &index, int role ) const
{
    if( ! index.isValid()
        || m_itemList->isEmpty()
        || index.row() >= m_itemList->size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        return int ( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        Model::SavedPlaylist *plist = m_itemList->at( index.row() );
        switch( index.column() ) {
        case 0: return plist->name();
        case 1: return numTracksIn( plist->name() );
        case 2: return plist->playCount();
        case 3: return plist->rating();
        }
    }
    //?? Settings???
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 18 );
    }
    else if( role == Qt::FontRole ) {
        return QFont( "Droid Sans", 9 );
    }
    return QVariant();
}


QVariant StdPlaylistManager::headerData( int section,
                                           Qt::Orientation orientation,
                                           int role ) const
{
    Q_UNUSED( orientation )
    if ( role == Qt::TextAlignmentRole ) {
        return int( Qt::AlignLeft | Qt::AlignVCenter );
    }
    else if ( role == Qt::DisplayRole ) {
        switch( section ) {
        case 0: return tr( "Name" );
        case 1: return tr( "Tracks" );
        case 2: return tr( "Played" );
        case 3: return tr( "Rating" );
        }
    }
    return QVariant();
}


QModelIndex StdPlaylistManager::index( int row, int column,
                                         const QModelIndex &parent ) const
{
    Q_UNUSED( parent )
    return createIndex(row, column);
}


QModelIndex StdPlaylistManager::parent( const QModelIndex &child ) const
{
    Q_UNUSED( child )
    return QModelIndex();
}


bool StdPlaylistManager::hasChildren( const QModelIndex &parent ) const
{
    if(! parent.isValid()) {
        return true;
    }
    return false;
}


Qt::ItemFlags StdPlaylistManager::flags( const QModelIndex &index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );
    if( index.column() == 3 ) {
        return flags |= Qt::ItemIsEditable;
    }
    return flags;
}


bool StdPlaylistManager::setData( const QModelIndex &index,
                                    const QVariant &value,
                                    int role )
{
    Q_UNUSED( role );
    if( index.column() == 3 ) {
        Model::SavedPlaylist *plist = m_itemList->at( index.row() );
        ratePlaylist( plist->name() , Utils::getRatingFromInt( value.toInt() ));
        return true;
    }
    return false;
}

void StdPlaylistManager::loadPlaylist( const QString playlistName )
{
    if( ! m_playlistToTrackIds.contains( playlistName ) ) {
        QList< QString > *list = new QList< QString >();
        if( T::get()->dataRetriever()->getPlaylistContent( playlistName,
                                                           *list )) {
            m_playlistToTrackIds.insert( playlistName, list );
        }
    }
}

//__END__( IPlaylistManager )

//__START__( IPlayQueue )
int StdPlayQueue::rowCount( const QModelIndex& ) const {
    return m_items.size();
}


int StdPlayQueue::columnCount( const QModelIndex& ) const {
    return  2;
}


QVariant StdPlayQueue::data( const QModelIndex &index, int role ) const {
    if ( ! index.isValid() || index.row() >= m_items.size() ) {
        return QVariant();
    }
    if ( role == Qt::TextAlignmentRole ) {
        if( index.column() == 0 ) {
            return int (Qt::AlignLeft | Qt::AlignVCenter);
        }
        else {
            return int (Qt::AlignRight | Qt::AlignVCenter);
        }
    }
    //These are for the settings to decide... but for now this what we have
    else if( role == Qt::SizeHintRole ) {
        return QSize( 0, 16 );
    }
    else if( role == Qt::FontRole ) {
        return QFont("Droid Sans", 8);
    }
    else {
        Data::MediaItem *item = m_items[ index.row() ];
        item->type();
        if( item && item->type() == Data::Media_LocalAudio
                || item->type() == Data::Media_StoredAudio ) {
            Data::AudioTrack *audioTrack = dynamic_cast<Data::AudioTrack *>
                    ( item );
            if ( role == Qt::DisplayRole ) {
                switch( index.column() ) {
                case 0: return audioTrack->title() +
                            ( audioTrack->artist().isEmpty()
                            ? "" :
                              ( " - " + audioTrack->artist() ));
                case 1: return Utils::getStringTime( audioTrack->duration() );
                }
            }
            else if( role == Qt::ToolTipRole ) {
                Data::AudioTrack *audioTrack =
                        dynamic_cast<Data::AudioTrack *>(
                            m_items.at( index.row() ) );
                QString tooltip = QString() +
                        "Title  : <b>"+ audioTrack->title() +"</b><br>"+
                        "Artist : <b>"+ audioTrack->artist() +"</b><br>"+
                        "Album  : <b>"+ audioTrack->album()  +"</b>";
                return tooltip;
            }
        }
        else if( item && item->type() == Data::Media_LocalVideo
                 || item->type() == Data::Media_StoredVideo ) {
            Data::VideoTrack *videoTrack = dynamic_cast< Data::VideoTrack *>(
                        item );
            if ( role == Qt::DisplayRole ) {
                switch( index.column() ) {
                case 0: return videoTrack->name();
                case 1: return Utils::getStringTime( videoTrack->duration() );
                }
            }
            else if( role == Qt::ToolTipRole ) {
                Data::AudioTrack *track = dynamic_cast< Data::AudioTrack *>(
                            m_items.at( index.row() ) );
                QString tooltip = QString() +
                        "Title: <b>"+ videoTrack->name()       +"</b><br>"+
                        "Path : <b>"+ track->url().toString()  +"</b>";
                return tooltip;
            }
        }
    }
    return QVariant();
}


QVariant StdPlayQueue::headerData( int section,
                                   Qt::Orientation orientation,
                                   int role ) const {
    Q_UNUSED( role );
   if( orientation == Qt::Horizontal ) {
        switch( section ) {
        case 0: return tr( "Track" );
        case 1: return tr( "Len" );
        }
    }
    else {
        return "";
    }
    return QVariant();
}


QModelIndex StdPlayQueue::parent( const QModelIndex & ) const {
    return QModelIndex();
}


bool StdPlayQueue::hasChildren( const QModelIndex &parent ) const {
    if( ! parent.isValid() ) {
        return true;
    }
    return false;
}



QModelIndex StdPlayQueue::index( int row, int column,
                                    const QModelIndex & ) const {
    return createIndex(row, column);
}
//__END__( IPlayQueue )
