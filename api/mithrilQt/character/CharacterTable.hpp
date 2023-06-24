////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class CharacterTable : public IdTableT<Character> {
        Q_OBJECT
    public:
        static std::span<const ColumnSpec>  defColumns();

        CharacterTable(all_t, QWidget*parent=nullptr);
        CharacterTable(all_t, std::initializer_list<ColumnSpec> columns, QWidget*parent=nullptr);
        CharacterTable(all_t, std::span<const ColumnSpec> columns, QWidget*parent=nullptr);
        ~CharacterTable();
    };
}
