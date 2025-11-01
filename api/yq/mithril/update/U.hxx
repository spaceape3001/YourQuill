////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/update/U.hpp>
#include <unordered_map>

namespace yq::mithril::update {
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
}
