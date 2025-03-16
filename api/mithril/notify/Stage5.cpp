////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Stage5.hpp"
#include <mutex>

namespace yq::mithril {

    struct Stage5::Repo {
        std::vector<const Stage5*>  all;
        std::vector<OneTime>        once;
        std::mutex                  once_guard;
    };
    
    Stage5::Repo& Stage5::repo()
    {
        static Repo s_ret;
        return s_ret;
    }
    
    const std::vector<const Stage5*>& Stage5::all()
    {
        return repo().all;
    }
    

    void     Stage5::push_once(OneTime&& ot)
    {
        auto& _r = repo();
        std::lock_guard<std::mutex> guard(_r.once_guard);
        _r.once.push_back(std::move(ot));
    }
    
    void     Stage5::swap_one_times(std::vector<OneTime>& ots)
    {
        auto& _r = repo();
        std::lock_guard<std::mutex> guard(_r.once_guard);
        std::swap(ots, _r.once);
    }

    Stage5::Stage5(int order, const std::source_location& sl) : m_order(order), m_source(sl)
    {
        repo().all.push_back(this);
    }
    
    Stage5::~Stage5()
    {
    }

}
