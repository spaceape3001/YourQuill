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

    Group  group(Atom at)
    {
        return is(at, group_class()) ? Group(at.id) : Group();
    }

    Class  group_class()
    {
        static const Class  cls = db_class("Group");
        return cls;
    }

    bool  exists(Group ch)
    {
        return is(Atom(ch.id), group_class());
    }
    
}
