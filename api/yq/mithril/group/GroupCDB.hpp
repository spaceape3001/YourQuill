////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/group/Group.hpp>

namespace yq::mithril {        
    namespace cdb {
        std::vector<Group>  all_groups(Sorted sorted=Sorted());
        Atom                atom(Group);
        std::string         brief(Group);
        bool                exists(Group);
        bool                exists_group(uint64_t);
        Group               group(Atom);
        Group               group(Entity);
        Group               group(std::string_view);
        Group               group(uint64_t);
        Class               group_class();
        Image               icon(Group);
        std::string         key(Group);
        std::string         name(Group);
    }
}
