////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/id/Id.hpp>
#include <unordered_map>
#include <string>

namespace yq::mithril::update {

    /*! \brief Cached Update
    
        This is the root for a cached update object that needs to loiter in memory.
    
        \note THIS IS MEANT FOR SINGLE THREADED USE!!!!  Keep it to a single thread.
    */
    template <IdType T>
    class U {
    public:
    
        const T                 x;
        const id_t              id;
        const std::string       key;
    
    protected:
        U(T _x, std::string&&k) : x(_x), id(_x), key(std::move(k)) {}
        
        template <typename UU>
        static UU&   lookup(T x)
        {
            static_assert(std::is_base_of_v<U,UU>);
            static std::unordered_map<id_t,UU*>    s_lookup;
            
            auto [i,f]   = s_lookup.try_emplace(x.id, nullptr);
            if(f)
                i->second       = new UU(x);
            return *(i->second);
        }
        
    };
}
