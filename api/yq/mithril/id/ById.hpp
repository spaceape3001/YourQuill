////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/TypeMeta.hpp>
#include <yq/mithril/id/Id.hpp>

namespace yq::mithril {

    /*! \brief Lookup comparison
    
        Used for maps, the lookup for an ID type of key.
    */
    struct ById {
        static MetaBase::id_t   id_for(const TypeMeta* t)
        {
            return t ? t->id() : 0;
        }
        
        bool    operator()(const TypeMeta* a, const TypeMeta* b) const
        {
            return id_for(a) < id_for(b);
        }
        
        template <IdType T>
        constexpr bool    operator()(T a, T b) const
        {
            return a.id < b.id;
        }
    };
    
}


