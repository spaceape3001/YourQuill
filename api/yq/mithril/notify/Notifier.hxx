////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/notify/Notifier.hpp>
#include <vector>

namespace yq::mithril {
    template <typename ... Args>
    struct Notifier<Args...>::Handler {
        FNExecute           execute = {};
        Id                  origin  = Id();
    };
    
    template <typename ... Args>
    struct Notifier<Args...>::Repo {
        std::vector<Handler>        handlers;
        
        Repo()
        {
            handlers.push_back({});
        }
    };

    template <typename ... Args>
    Notifier<Args...>::Repo&  Notifier<Args...>::repo()
    {
        static Repo s_repo;
        return s_repo;
    }
    
    template <typename ... Args>
    size_t   Notifier<Args...>::add(Id i, FNExecute&& fn)
    {
        auto& _r    = repo();
        size_t  s   = _r.handlers.size();
        _r.handlers.push_back({ .execute = std::move(fn), .origin=i });
        return s;
    }
    
    template <typename ... Args>
    size_t   Notifier<Args...>::add(FNExecute&& fn)
    {
        return add(Id(), std::move(fn));
    }

    template <typename ... Args>
    void     Notifier<Args...>::erase(size_t z)
    {
        auto& _r    = repo();
        if(z < _r.handlers.size())
            _r.handlers[z].execute  = {};
    }

    template <typename ... Args>
    void     Notifier<Args...>::erase_all(Id i)
    {
        auto& _r = repo();
        for(auto& h : _r.handlers){
            if(h.origin == i)
                h.execute   = {};
        }
    }

    template <typename ... Args>
    void     Notifier<Args...>::notify(Args...args)
    {
        auto& _r    = repo();
        for(auto& h : _r.handlers){
            if(!h.execute)
                continue;
            h.execute(args...);
        }
    }
    
}
