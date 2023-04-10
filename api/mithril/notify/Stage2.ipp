////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Stage2.hpp"

namespace yq {
    namespace mithril {

        struct Stage2::Repo {
            std::vector<const Stage2*> all;
        };
        
        Stage2::Repo& Stage2::repo()
        {
            static Repo s_ret;
            return s_ret;
        }
        
        const std::vector<const Stage2*>& Stage2::all()
        {
            return repo().all;
        }
        

        Stage2::Stage2(int order, const std::source_location& sl) : m_order(order), m_source(sl)
        {
            repo().all.push_back(this);
        }
        
        Stage2::~Stage2()
        {
        }
    }
}
