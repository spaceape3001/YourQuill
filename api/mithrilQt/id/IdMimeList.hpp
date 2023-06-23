////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <QMimeData>
#include <mithril/id/Id.hpp>
#include <QStringList>

namespace yq::mithril {
    class IdMimeList : public QMimeData {
        Q_OBJECT
    public:
        
        IdMimeList(const std::vector<Id>&);
        IdMimeList(std::vector<Id>&&);
        virtual ~IdMimeList();
        
        static QStringList  mimeTypes();
        
        virtual bool    hasFormat(const QString& mt) const override;

        //  payload
        
        std::vector<Id>        idList;
    };
}
