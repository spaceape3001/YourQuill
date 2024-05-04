////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Py1.hpp"

namespace yq::doodle {
    Py1Info::Py1Info(std::string_view zName, const PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D1);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Py1::Py1(Project& prj) : Py(prj)
    {
    }
    
    Py1::Py1(Project& prj, const Py1&cp) : Py(prj, cp)
    {
    }

    Py1::~Py1()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Py1::remap(const Remapper&rMap)
    {
        Py::remap(rMap);
    }
}

