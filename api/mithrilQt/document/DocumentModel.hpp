////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/document/Document.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class DocumentModel : public IdModelT<Document> {
        Q_OBJECT
    public:
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        DocumentModel(Type t, QObject* parent=nullptr) : DocumentModel(t, ALL, parent) {}
        DocumentModel(Type t, all_t, QObject* parent=nullptr);
        ~DocumentModel();
        
        using IdModelT<Document>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);
        
        using IdModelT<Document>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());

    private:
        DocumentModel(Type t, Document, IdProvider&&, QObject*parent);
    };
}
