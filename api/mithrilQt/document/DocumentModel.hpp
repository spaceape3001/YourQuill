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
    
        DocumentModel(Type t, QObject* parent=nullptr) : DocumentModel(t, ALL, parent) {}
        DocumentModel(Type t, all_t, QObject* parent=nullptr);
        ~DocumentModel();
        
        using IdModelT<Document>::addColumn;
        void    addColumn(Column);
        void    addColumns(std::span<const Column> columns);
        
    private:
        DocumentModel(Type t, Document, IdProvider&&, QObject*parent);
    };
}
