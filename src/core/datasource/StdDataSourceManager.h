#pragma once

#include <QObject>

#include "../interfaces/IDataSourceManager.h"

namespace Tanyatu { namespace Impl {

class TANYATU_CORE_EXPORT StdDataSourceManager : public IDataSourceManager
{

public:
    explicit StdDataSourceManager();
    
    bool init( QHash< QString, QString > &params );

    bool isDataSourceValid();

    QString dataSourceId();

    QString description();

    QString lastError();

    bool executeQuery( const QString queryId, const QString queryString );

    virtual ~StdDataSourceManager() {

    }

protected:
    bool createSchema();

    virtual QHash< QString, QString > *getQueries();

private:
    QString m_error;

    bool m_valid;
};


} } //end of namespaces

