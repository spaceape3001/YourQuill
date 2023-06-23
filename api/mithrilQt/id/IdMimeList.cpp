#include "IdMimeList.hpp"
#include <QStringList>

IdMimeList::IdMimeList(const Vector<quint64>& ids)
    : idList(ids)
{
}

IdMimeList::~IdMimeList()
{
}

    
static const char*   szMimeType  = "application/x-custom/atoms";

QStringList  IdMimeList::mimeTypes() 
{
    return QStringList() << szMimeType;
}

bool    IdMimeList::hasFormat(const QString& mt) const
{
    if(mt == szMimeType)
        return true;
    return QMimeData::hasFormat(mt);
}

