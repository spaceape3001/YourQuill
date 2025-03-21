////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Py0.hpp"
#include "Py0InfoWriter.hpp"

YQ_OBJECT_IMPLEMENT(yq::doodler::Py0)

namespace yq::doodler {
    Py0Info::Py0Info(std::string_view zName, const PyInfo& base, const std::source_location& sl) : PyInfo(zName, base, sl)
    {
        set(Flag::D0);
    }

    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    void Py0::init_info()
    {
        auto w = writer<Py0>();
        w.description("Physical Object in 0D");
    }

    Py0::Py0(Project& prj) : Py(prj)
    {
    }
    
    Py0::Py0(Project& prj, const Py0&cp) : Py(prj, cp)
    {
    }

    Py0::~Py0()
    {
    }

    //! Remap IDs/pointers appropriately
    void        Py0::remap(const Remapper&rMap)
    {
        Py::remap(rMap);
    }
}

