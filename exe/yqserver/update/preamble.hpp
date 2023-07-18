////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/preamble.hpp>
#include <basic/Flags.hpp>
#include <math/Counter.hpp>
#include <meta/TypeInfo.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/id/Id.hpp>
#include <unordered_map>

namespace yq::mithril {
    struct Class;
    struct Field;
}

namespace yq::mithril::update {

    using HopCount              = HCountI8;
    using hop_t                 = int8_t;

    using ClassHopMap           = std::map<Class,HopCount>;
    using FieldHopMap           = std::map<Field,HopCount>;

    struct UClass;
    struct UField;
    
    enum class F {
        LOCK,
        LOAD,
        INFO,
        ICON,
        TAGS     
    };
    
    using FF        = Flags<F>;
    
    template <typename T>
    struct AL {
        T   local;
        T   all;
    };

    template <IdType T>
    struct DD {
        std::map<T,HopCount>    hop;
        std::set<T>             all;        //  All (for fields)
        std::set<T>             direct;
    };
    
    struct ById {
        static Meta::id_t   id_for(const TypeInfo* t)
        {
            return t ? t->id() : 0;
        }
        
        bool    operator()(const TypeInfo* a, const TypeInfo* b) const
        {
            return id_for(a) < id_for(b);
        }
        
        template <IdType T>
        constexpr bool    operator()(T a, T b) const
        {
            return a.id < b.id;
        }
    };
    
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
