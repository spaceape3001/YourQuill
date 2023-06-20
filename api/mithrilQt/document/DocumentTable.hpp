////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class DocumentModel;
    class DocumentTable : public IdTableT<Document> {
        Q_OBJECT
    public:
        DocumentTable(all_t, QWidget*parent=nullptr);
        DocumentTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        DocumentTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~DocumentTable();
        
        DocumentModel*          model();
        const DocumentModel*    model() const;
    };
}
