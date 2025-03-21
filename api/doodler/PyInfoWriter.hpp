////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Py.hpp>

namespace yq::doodler {

    template <typename T>
    class PyInfo::Writer : public DObjectInfo::Writer<T> {
    public:
        Writer(PyInfo* pInfo) : DObjectInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(PyInfo& pInfo) : Writer(&pInfo)
        {
        }
    };
}
