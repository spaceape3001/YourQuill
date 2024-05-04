////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Py.hpp"

namespace yq::doodle {
    PyInfo::PyInfo(std::string_view zName, const DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::PHYSICAL);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Py::Py(Project& prj) : DObject(prj)
    {
    }
    
    Py::Py(Project& prj, const Py&cp) : DObject(prj, cp)
    {
    }

    Py::~Py()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Py::remap(const Remapper&rMap)
    {
        DObject::remap(rMap);
    }
}

