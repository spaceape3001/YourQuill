////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/character/CharacterCDB.hpp>

namespace yq::mithril {
    namespace cdb {
        std::vector<Character>      all_characters(Sorted sorted)
        {
            return blind_remap(all_atoms(character_class(), sorted), Character());
        }
        
        Atom                        atom(Character ch)
        {
            return Atom{ ch.id };
        }

        Character                   character(Atom at)
        {
            return is(at, character_class()) ? Character(at.id) : Character();
        }

        Class   character_class()
        {
            static const Class  cls = db_class("Character");
            return cls;
        }

        bool                        exists(Character ch)
        {
            return is(Atom(ch.id), character_class());
        }
        
    }
}
