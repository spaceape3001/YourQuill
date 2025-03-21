////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Py4.hpp>

namespace yq::doodler {
    template <typename T>
    class Py4Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py4Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py4Info& pInfo) : Writer(&pInfo)
        {
        }
    };
}
