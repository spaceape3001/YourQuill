////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Py.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Py)

namespace yq::doodler {
    PyInfo::PyInfo(std::string_view zName, DObjectInfo& base, const std::source_location& sl) : DObjectInfo(zName, base, sl)
    {
        set(Flag::PHYSICAL);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Py::init_info()
    {
        w.description("Physical Object");
    }

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

