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
    
        CharacterModel(Type t, QObject* parent=nullptr) : CharacterModel(t, ALL, parent) {}
        CharacterModel(Type t, all_t, QObject* parent=nullptr);
        ~CharacterModel();
        
    private:
        CharacterModel(Type t, Character, IdProvider&&, QObject*parent);
    };
}
