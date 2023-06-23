////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class AtomModel;
    class AtomTable : public IdTableT<Atom> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        AtomTable(all_t, QWidget*parent=nullptr);
        AtomTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        AtomTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~AtomTable();
        
        AtomModel*          model();
        const AtomModel*    model() const;
    };
}
