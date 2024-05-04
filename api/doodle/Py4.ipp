////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Py4.hpp"

namespace yq::doodle {
    Py4Info::Py4Info(std::string_view zName, const PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D4);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Py4::Py4(Project& prj) : Py(prj)
    {
    }
    
    Py4::Py4(Project& prj, const Py4&cp) : Py(prj, cp)
    {
    }

    Py4::~Py4()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Py4::remap(const Remapper&rMap)
    {
        Py::remap(rMap);
    }
}

