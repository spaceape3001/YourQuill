////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/update/U.hpp>
#include <unordered_map>

namespace yq::mithril::update {

    template <IdType T>
    struct U<T>::Repo {
        std::vector<Handler>        handlers;
        
        Repo()
        {
            handlers.push_back({});
        }
    };

    template <IdType T>
    typename U<T>::Repo& U<T>::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    template <IdType T>
    size_t       U<T>::add_handler(Handler&&h)
    {
        auto& _r    = repo();
        size_t  s   = _r.handlers.size();
        _r.handlers.push_back(std::move(h));
        return s;
    }

    template <IdType T>
    void         U<T>::erase_all_handlers(Id i)
    {
        auto& _r = repo();
        for(auto& h : _r.handlers){
            if(h.origin == i)
                h.execute   = {};
        }
    }

    template <IdType T>
    void         U<T>::erase_handler(size_t z)
    {
        auto& _r    = repo();
        if(z < _r.handlers.size())
            _r.handlers[z].execute  = {};
    }

    template <IdType T>
        template <typename UU>
    UU&   U<T>::lookup(T x)
    {
        static_assert(std::is_base_of_v<U,UU>);
        static std::unordered_map<id_t,UU*>    s_lookup;
        
        auto [i,f]   = s_lookup.try_emplace(x.id, nullptr);
        if(f)
            i->second       = new UU(x);
        return *(i->second);
    }

////////////////////////////////////////////////////////////////////////////////

    template <IdType T>
    void    U<T>::notify(Change chg)
    {
        auto& _r    = repo();
        for(auto& h : _r.handlers){
            if(!h.execute)
                continue;
            if(!h.change(chg))
                continue;
            if(h.trigger && !h.trigger(x, chg))
                continue;
            h.execute(x, chg);
        }
    }

}
