////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Py2.hpp>

namespace yq::doodler {
    template <typename T>
    class Py2Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py2Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py2Info& pInfo) : Writer(&pInfo)
        {
        }
    };
}
