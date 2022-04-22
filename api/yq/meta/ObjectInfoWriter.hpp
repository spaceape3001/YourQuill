////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ObjectInfo.hpp"
#include "CompoundInfoDynamic.hpp"

namespace yq {
    template <typename C>
    class ObjectInfo::Writer : public CompoundInfo::Dynamic<C> {
    public:
    
        /*! \brief Sets the base object (if not already set)
        */
        template <typename B>
        Writer&     base()
        {
            static_assert( std::is_base_of_v<B, C>, "T must derive from B!" );
            ObjectInfo*     obj = static_cast<ObjectInfo*>(Meta::Writer::m_meta);
            assert(!obj->m_base);
            obj->m_base = &meta<B>();
            return *this; 
        }
        
        Writer(ObjectInfo* obj) : CompoundInfo::Dynamic<C>(obj) 
        {
            assert(obj);
        }
        
        Writer(ObjectInfo& obj) : CompoundInfo::Dynamic<C>(&obj) 
        {
        }
    };
}
