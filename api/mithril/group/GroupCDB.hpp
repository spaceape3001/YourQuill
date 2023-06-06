////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/group/Group.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Group>  all_groups(Sorted sorted=Sorted());
        Atom                atom(Group);
        Group               group(Atom);
        Class               group_class();
        bool                exists(Group);
        bool                exists_group(uint64_t);
    }
}
