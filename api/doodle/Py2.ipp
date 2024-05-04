////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Py2.hpp"

namespace yq::doodle {
    Py2Info::Py2Info(std::string_view zName, const PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D2);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Py2::Py2(Project& prj) : Py(prj)
    {
    }
    
    Py2::Py2(Project& prj, const Py2&cp) : Py(prj, cp)
    {
    }

    Py2::~Py2()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Py2::remap(const Remapper&rMap)
    {
        Py::remap(rMap);
    }
}

