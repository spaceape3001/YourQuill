////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/document/Document.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class DocumentModel : public IdModelT<Document> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        DocumentModel(Type t) : DocumentModel(t, ALL) {}
        DocumentModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~DocumentModel();
        
        using IdModelT<Document>::addColumn;
        void    addColumn(Column);
        
    private:
        DocumentModel(Type t, Document, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
