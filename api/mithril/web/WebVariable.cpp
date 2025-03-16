////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "WebVariable.hpp"
#include "WebRepo.hxx"

namespace yq::mithril {
    WebVariable::WebVariable(std::string_view n, const std::source_location& sl) : MetaBase(n, sl)
    {
        WLOCK
        if(_r.openReg){
            _r.allVars.push_back(this);
            _r.variables[n] = this;
        }
    }
    
    WebVariable::~WebVariable()
    {
    }

//  ------------------------------------------------

    WebVariable::Writer::Writer(WebVariable*p) : MetaBase::Writer(p), m_var(p)
    {
    }

    WebVariable::Writer::Writer(Writer&& mv) : MetaBase::Writer(std::move(mv)), m_var(mv.m_var)
    {
        mv.m_var = nullptr;
    }
    
    WebVariable::Writer& WebVariable::Writer::operator=(Writer&& mv)
    {
        if(this != &mv){
            //if(m_var)
                //m_var -> seal();
            m_var  = mv.m_var;
            mv.m_var   = nullptr;
        }
        return *this;
    }
    
    WebVariable::Writer::~Writer()
    {
        //if(m_var)
            //m_var -> seal();
    }

    WebVariable::Writer&  WebVariable::Writer::description(std::string_view sv)
    {
        if(m_var)
            MetaBase::Writer::description(sv);
        return *this;
    }
}

