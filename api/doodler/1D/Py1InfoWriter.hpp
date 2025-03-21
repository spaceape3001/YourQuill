////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Py1.hpp>

namespace yq::doodler {
    template <typename T>
    class Py1Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py1Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py1Info& pInfo) : Writer(&pInfo)
        {
        }
    };
}
