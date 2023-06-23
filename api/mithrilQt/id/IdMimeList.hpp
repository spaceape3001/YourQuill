#pragma once


#include <QMimeData>
#include "core/type/Collections.hpp"


class IdMimeList : public QMimeData {
    Q_OBJECT
public:
    
    IdMimeList(const Vector<quint64>&);
    virtual ~IdMimeList();
    
    static QStringList  mimeTypes();
    
    virtual bool    hasFormat(const QString& mt) const;

    //  payload
    
    Vector<quint64>        idList;
};

