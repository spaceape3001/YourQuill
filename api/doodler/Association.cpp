////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Association.hpp"
#include "AssociationInfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Association)

namespace yq::doodler {
    AssociationInfo::AssociationInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::ASSOCIATION);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Association::init_info()
    {
        auto w = writer<Association>();
        w.description("Association");
    }
    
    Association::Association(Project& prj) : DObject(prj)
    {
    }
    
    Association::Association(Project& prj, const Association& assn) : DObject(prj, assn)
    {
    }

    Association::~Association()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Association::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

