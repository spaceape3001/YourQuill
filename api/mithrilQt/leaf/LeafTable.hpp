////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class LeafModel;
    class LeafTable : public IdTableT<Leaf> {
        Q_OBJECT
    public:

        static std::span<const Column>  defColumns();

        LeafTable(all_t, QWidget*parent=nullptr);
        LeafTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        LeafTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~LeafTable();
        
        LeafModel*          model();
        const LeafModel*    model() const;
    };
}
