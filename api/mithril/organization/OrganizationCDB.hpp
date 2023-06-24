////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Organization.hpp"

namespace yq::mithril::cdb {
    std::vector<Organization>   all_organizations(Sorted sorted=Sorted());
    Atom                        atom(Organization);
    Entity                      entity(Organization);
    bool                        exists(Organization);
    bool                        exists_organization(uint64_t);
    Group                       group(Organization);
    Image                       icon(Organization);
    std::string                 key(Organization);
    std::string                 name(Organization);
    Organization                organization(Atom);
    Organization                organization(Entity);
    Organization                organization(Group);
    Class                       organization_class();
}

