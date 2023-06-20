////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class RootModel;
    class RootTable : public IdTableT<Root> {
        Q_OBJECT
    public:
        static std::span<const Column>  defColumns();

        RootTable(all_t, QWidget*parent=nullptr);
        RootTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        RootTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~RootTable();
        
        RootModel*          model();
        const RootModel*    model() const;
    };
}
