////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Py0.hpp>

namespace yq::doodler {
    template <typename T>
    class Py0Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py0Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py0Info& pInfo) : Writer(&pInfo)
        {
        }
    };
}
