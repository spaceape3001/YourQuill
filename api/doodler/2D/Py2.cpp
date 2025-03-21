////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Py2.hpp"
#include "Py2InfoWriter.hpp"

namespace yq::doodler {
    Py2Info::Py2Info(std::string_view zName, PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D2);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Py2::init_info()
    {
        auto w = writer<Py2>();
        w.description("Physical Object in 2D");
    }

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

