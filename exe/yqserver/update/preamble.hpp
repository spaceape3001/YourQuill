////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>
#include <basic/Flags.hpp>
#include <math/Counter.hpp>
#include <mithril/id/Id.hpp>

namespace yq::mithril {
    struct Class;
    struct Field;
}

namespace yq::mithril::update {
    using StringZCountMap       = std::map<std::string,CountU16,IgCase>;
    using ClassZCountMap        = std::map<Class,CountU16>;
    using FieldZCountMap        = std::map<Field,CountU16>;
    
    using StringHCountMap       = std::map<std::string,HCountU16,IgCase>;
    using ClassHCountMap        = std::map<Class,HCountU16>;
    using FieldHCountMap        = std::map<Field,HCountU16>;

    struct UClass;
    struct UField;
    
    enum class F {
        LOAD,
        INFO,
        ICON,
        TAGS     
    };
    
    using FF        = Flags<F>;
    
    
    template <IdType T>
    class U {
    public:
    
        const T                 x;
        const id_t              id;
        const std::string       key;
    
    protected:
        U(T _x, std::string&&k) : x(_x), id(_x), key(std::move(k)) {}
        
        template <typename UU, size_t N>
        static UU&   lookup(T x)
        {
            static_assert(std::is_base_of_v<U,UU>);
            static std::vector<UU*>      s_lookup(N, nullptr);
            if(s_lookup.size() <= x.id){
                size_t  cnt = (x.id + N + N);
                cnt -= cnt % N;
                s_lookup.resize(cnt, nullptr);
            }

            auto& u = s_lookup[x.id];
            if(!u)
                u   = new UU(x);
            return *u;
        }
        
    };
    
}
