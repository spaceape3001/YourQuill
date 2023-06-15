////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/IgCase.hpp>
#include "IdColumn.hpp"

namespace yq::mithril {
    struct IdColumn::Repo {
        std::map<std::string_view,PublishFN,IgCase>     published;
        mutable tbb::spin_rw_mutex                      mutex;
    };
    
    #define LOCK                                                \
        auto & _r = repo();                                     \
        tbb::spin_rw_mutex::scoped_lock _lock(_r.mutex, false);
    #define WLOCK                                               \
        auto & _r = repo();                                     \
        tbb::spin_rw_mutex::scoped_lock _lock(_r.mutex, true);
    
    IdColumn::Repo&    IdColumn::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    void         IdColumn::publish_(std::string_view k, PublishFN&& fn)
    {
        WLOCK
        repo().published[k]    = std::move(fn);
    }
    
    IdColumn*    IdColumn::create(std::string_view k)
    {
        LOCK
        auto i = _r.published.find(k);
        if(i == _r.published.end())
            return nullptr;
        return (i->second)();
    }

    IdColumn::IdColumn() = default;
    IdColumn::~IdColumn() = default;
    
}
