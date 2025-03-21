////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Py3.hpp"
#include "Py3InfoWriter.hpp"

namespace yq::doodler {
    Py3Info::Py3Info(std::string_view zName, PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D3);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Py3::init_info()
    {
        auto w = writer<Py3>();
        w.description("Physical Object in 3D");
    }

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

