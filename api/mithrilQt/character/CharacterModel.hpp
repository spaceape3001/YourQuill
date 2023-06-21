////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithrilQt/preamble.hpp>
#include <mithrilQt/id/IdModel.hpp>
#include <mithril/character/Character.hpp>
#include <mithrilQt/preamble.hpp>

namespace yq::mithril {
    class CharacterModel : public IdModelT<Character> {
        Q_OBJECT
    public:
    
        static std::optional<IdColumn>     resolve(Column, ColOpts opts=ColOpts());

        CharacterModel(Type t, QObject* parent=nullptr) : CharacterModel(t, ALL, parent) {}
        CharacterModel(Type t, all_t, QObject* parent=nullptr);
        ~CharacterModel();
        
        using IdModelT<Character>::addColumn;
        void    addColumn(Column, ColOpts opts=ColOpts());
        void    addColumns(std::span<const Column> columns);
        
        using IdModelT<Character>::setColumn;
        void    setColumn(Column, ColOpts opts=ColOpts());

    private:
        CharacterModel(Type t, Character, IdProvider&&, QObject*parent);
    };
}
