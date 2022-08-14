////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Stage3.hpp"

namespace yq {
    struct Stage3::Repo {
        std::vector<const Stage3*> all;
    };
    
    Stage3::Repo& Stage3::repo()
    {
        static Repo s_ret;
        return s_ret;
    }
    
    const std::vector<const Stage3*>& Stage3::all()
    {
        return repo().all;
    }
    

    Stage3::Stage3(const FileSpec& spec, int order, const std::source_location&sl) 
    {
        m_spec          = spec;
        m_order         = order;
        m_source        = sl;
        repo().all.push_back(this);
    }
    
    Stage3::~Stage3()
    {
    }
}

