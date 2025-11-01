////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "OrganizationCDB.hpp"
#include <yq/mithril/atom/AtomCDB.hpp>
#include <yq/mithril/class/ClassCDB.hpp>
#include <yq/mithril/entity/EntityCDB.hpp>
#include <yq/mithril/group/GroupCDB.hpp>
#include <yq/mithril/image/Image.hpp>

namespace yq::mithril::cdb {
    std::vector<Organization>     all_organizations(Sorted sorted)
    {
        return blind_remap(all_atoms(organization_class(), sorted), Organization());
    }
    
    Atom                    atom(Organization org)
    {
        return Atom(org.id);
    }
    
    std::string                 brief(Organization org)
    {
        return brief(atom(org));
    }
    
    Entity                  entity(Organization org)
    {
        return Entity(org.id );
    }

    bool                    exists(Organization org)
    {
        return exists_organization(org.id);
    }
    
    bool                    exists_organization(uint64_t i)
    {
        return is(Atom(i), organization_class());
    }

    Group                   group(Organization e)
    {
        return Group(e.id);
    }
    
    Image                   icon(Organization e)
    {
        return icon(atom(e));
    }
    
    std::string             key(Organization e)
    {
        return key(atom(e));
    }
    
    std::string             name(Organization e)
    {
        return name(atom(e));
    }

    Organization             organization(Atom a)
    {
        return exists_organization(a.id) ? Organization(a.id) : Organization();
    }

    Organization            organization(Entity e)
    {
        return exists_organization(e.id) ? Organization(e.id) : Organization();
    }
    
    Organization            organization(Group g)
    {
        return exists_organization(g.id) ? Organization(g.id) : Organization();
    }

    Organization            organization(std::string_view k)
    {
        return organization(atom(k)); // TODO -- better lookup
    }
    
    Organization            organization(uint64_t id)
    {
        return exists_organization(id) ? Organization(id) : Organization();
    }

    Class                   organization_class()
    {
        static const Class  cls = db_class("Organization");
        return cls;
    }
    
}
