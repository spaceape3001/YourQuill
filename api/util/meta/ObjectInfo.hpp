////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CompoundInfo.hpp"

namespace yq {
    namespace meta {
    
        /*! \brief Represents info for an object thats intrusively bound
        */
        class ObjectInfo : public CompoundInfo {
        public:
        
            const ObjectInfo*   base() const { return m_base; }
        
            template <typename C> class Writer;
        
        protected:
            friend class PropertyInfo;
            friend class MethodInfo;
            
            ObjectInfo(const char* zName, const char* zFile, ObjectInfo* b=nullptr);
            
            virtual void                    sweep_impl() override;

            ObjectInfo*     m_base; // defined/declared base
            
            struct D {
                LUC<ObjectInfo>     bases;
                LUC<ObjectInfo>     derived;
                LUC<PropertyInfo>   properties;
                LUC<MethodInfo>     methods;
            };
            
            D               m_local, m_all;
        };


        /*! \brief Converts meta to object, if it's valid
        
            \return ObjectInfo pointer, if valid, NULL otherwise
        */
        const ObjectInfo* to_object(const Meta* m);
    }
}
