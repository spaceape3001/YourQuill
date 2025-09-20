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
    std::string                 brief(Organization);
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
    Organization                organization(std::string_view);
    Organization                organization(uint64_t);
    Class                       organization_class();
}

