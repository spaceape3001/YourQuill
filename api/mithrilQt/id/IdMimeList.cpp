////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "IdMimeList.hpp"

namespace yq::mithril {
    IdMimeList::IdMimeList(const std::vector<Id>&cp) : idList(cp)
    {
    }
    
    IdMimeList::IdMimeList(std::vector<Id>&&mv) : idList(std::move(mv))
    {
    }

    IdMimeList::~IdMimeList()
    {
    }

    static const char*   szMimeType  = "application/x-custom/yquill/ids";

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
}

#include "moc_IdMimeList.cpp"
