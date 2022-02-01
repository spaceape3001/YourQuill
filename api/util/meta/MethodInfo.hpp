////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Meta.hpp"
#include "ArgInfo.hpp"

namespace yq {
    class MethodInfo : public Meta {
    public:
    
        class Writer;
    
        const LUC<ArgInfo>&     args() const { return m_args; }
    
    protected:
        LUC<ArgInfo>            m_args;
        MethodInfo(const char* zName, Meta*, options_t opts=0);
    };
}
