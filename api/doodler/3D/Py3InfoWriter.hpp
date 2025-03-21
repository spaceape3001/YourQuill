////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <doodler/DObjectInfoWriter.hpp>
#include <doodler/Py3.hpp>

namespace yq::doodler {
    template <typename T>
    class Py3Info::Writer : public PyInfo::Writer<T> {
    public:
        Writer(Py3Info* pInfo) : PyInfo::Writer<T>(pInfo)
        {
        }
        
        Writer(Py3Info& pInfo) : Writer(&pInfo)
        {
        }
    };

}
