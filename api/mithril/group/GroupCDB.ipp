////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/group/GroupCDB.hpp>

namespace yq::mithril::cdb {
    std::vector<Group>      all_groups(Sorted sorted)
    {
        return blind_remap(all_atoms(group_class(), sorted), Group());
    }
    
    Atom  atom(Group ch)
    {
        return Atom{ ch.id };
    }

    bool  exists(Group ch)
    {
        return exists_group(ch.id);
    }

    bool  exists_group(uint64_t i)
    {
        return is(Atom(i), group_class());
    }

    Group  group(Atom at)
    {
        return is(at, group_class()) ? Group(at.id) : Group();
    }

    Class  group_class()
    {
        static const Class  cls = db_class("Group");
        return cls;
    }

    Image               icon(Group g)
    {
        return icon(atom(g));
    }

    std::string         key(Group g)
    {
        return key(atom(g));
    }
    
    std::string         name(Group g)
    {
        return name(atom(g));
    }
    
}
