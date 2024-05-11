////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Py3.hpp"

namespace yq::doodler {
    Py3Info::Py3Info(std::string_view zName, const PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D3);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Py3::Py3(Project& prj) : Py(prj)
    {
    }
    
    Py3::Py3(Project& prj, const Py3&cp) : Py(prj, cp)
    {
    }

    Py3::~Py3()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Py3::remap(const Remapper&rMap)
    {
        Py::remap(rMap);
    }
}

