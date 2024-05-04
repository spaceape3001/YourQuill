////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Project.hpp"

namespace yq::doodle {
    ProjectInfo::ProjectInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::PROJECT);
    }

    Project* ProjectInfo::createP(DObject* parent) const
    {
        return static_cast<Project*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Project::Project(DObject* parent) : DObject(parent)
    {
    }
    
    Project::Project(const Project&cp) : DObject(cp)
    {
    }
    
    Project::~Project()
    {
    }



    //! Remap IDs/pointers appropriately
    void        Project::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
    
    static void reg_project()
    {
        auto w = writer<Project>();
    }
}

YQ_OBJECT_IMPLEMENT(yq::doodle::Project)
