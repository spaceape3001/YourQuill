////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/id/IdModel.hpp>
#include <mithril/character/Character.hpp>
#include <mithrilQt/column.hpp>

namespace yq::mithril {
    class CharacterModel : public IdModelT<Character> {
        Q_OBJECT
    public:
    
        static std::span<const Column>  defColumns();
    
        CharacterModel(Type t) : CharacterModel(t, ALL) {}
        CharacterModel(Type t, all_t, std::span<const Column> cols=defColumns(), QObject* parent=nullptr);
        ~CharacterModel();
        
        using IdModelT<Character>::addColumn;
        void    addColumn(Column);
        
    private:
        CharacterModel(Type t, Character, IdProvider&&, std::span<const Column> cols, QObject*parent);
    };
}
