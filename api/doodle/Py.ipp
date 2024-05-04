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

    Py* PyInfo::createP(DObject* parent) const
    {
        return static_cast<Py*>(createD(parent));
    }
    
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////


    Py::Py(DObject* parent) : DObject(parent)
    {
    }
    
    Py::Py(const Py&cp) : DObject(cp)
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

YQ_OBJECT_IMPLEMENT(yq::doodle::Py)
