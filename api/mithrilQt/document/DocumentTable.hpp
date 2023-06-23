////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class DocumentModel;
    class DocumentTable : public IdTableT<Document> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        DocumentTable(all_t, QWidget*parent=nullptr);
        DocumentTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        DocumentTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~DocumentTable();
        
        DocumentModel*          model();
        const DocumentModel*    model() const;
    };
}
