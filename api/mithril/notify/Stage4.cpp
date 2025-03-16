////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Stage4.hpp"

namespace yq::mithril {

    struct Stage4::Repo {
        std::vector<const Stage4*> all;
    };
    
    Stage4::Repo& Stage4::repo()
    {
        static Repo s_ret;
        return s_ret;
    }
    
    const std::vector<const Stage4*>& Stage4::all()
    {
        return repo().all;
    }
    

    Stage4::Stage4(int order, const std::source_location& sl) : m_order(order), m_source(sl)
    {
        repo().all.push_back(this);
    }
    
    Stage4::~Stage4()
    {
    }

}
