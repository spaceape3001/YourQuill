////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdTable.hpp>

namespace yq::mithril {
    class CharacterModel;
    
    class CharacterTable : public IdTableT<Character> {
        Q_OBJECT
    public:
        static std::span<const Column>  defColumns();

        CharacterTable(all_t, QWidget*parent=nullptr);
        CharacterTable(all_t, std::initializer_list<Column> columns, QWidget*parent=nullptr);
        CharacterTable(all_t, std::span<const Column> columns, QWidget*parent=nullptr);
        ~CharacterTable();
        
        CharacterModel*          model();
        const CharacterModel*    model() const;
    };
}
