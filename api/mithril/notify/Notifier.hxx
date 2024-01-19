////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/notify/Notifier.hpp>
#include <vector>

namespace yq::mithril {
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
    size_t   Notifier<Args...>::add(Handler&&h)
    {
        auto& _r    = repo();
        size_t  s   = _r.handlers.size();
        _r.handlers.push_back(std::move(h));
        return s;
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
    void     Notifier<Args...>::notify(Change chg, Args...args)
    {
        auto& _r    = repo();
        for(auto& h : _r.handlers){
            if(!h.execute)
                continue;
            if(!h.change(chg))
                continue;
            if(h.trigger && !h.trigger(chg, args...))
                continue;
            h.execute(chg, args...);
        }
    }
    
}
