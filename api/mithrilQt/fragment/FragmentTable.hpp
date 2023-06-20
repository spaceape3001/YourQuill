////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/column.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class FragmentModel;
    class FragmentTable : public IdTableT<Fragment> {
        Q_OBJECT
    public:

        static std::span<const Column>  defColumns();

        FragmentTable(all_t, QWidget*parent=nullptr);
        FragmentTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        FragmentTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~FragmentTable();
        
        FragmentModel*          model();
        const FragmentModel*    model() const;
    };
}
