////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Meta.hpp"

namespace yq {
    namespace meta {
    
        class Getter;
        class Setter;
    
        /*! \brief Attribute of a object/value
        
            A property is an attribute of an object/value.  
            A "state" proeprty is considered to be a field of a structure.  
            If all states are restored, the objects/value should behave identical.  
            Non-state proproperties should generally be read-only, but not always the case.  
            A vector's "set-heading" is an equally valid way to set a vector -- though that could also
            be in a method (or static method);
            
        */
        class PropertyInfo : public Meta {
            friend class Getter;
            friend class Setter;
        public:
        
            template <typename T> class Writer;
            template <typename T> class VarW;
            template <typename C, typename T> class PropW;
        
            const TypeInfo&     type() const { return m_type; }
            
            Variant             get(const void*) const;
            bool                set(void*, const Variant&) const;
            
            const Getter*       getter() const { return m_getter; }
            const Setter*       setter() const { return m_setter; }
            
            bool                is_state() const;
            bool                is_static() const;
            
        protected:
            PropertyInfo(const char* zName, const TypeInfo&, Meta*, options_t opts=0);
            
            virtual Variant     get_impl(const void*) const = 0;
            virtual bool        set_impl(void*, const void*) const { return false; }
            
        private:
            const Setter*       m_setter    = nullptr;
            const Getter*       m_getter    = nullptr;
            const TypeInfo&     m_type;
            impl::DataBlock     m_default;
        };
        
    }
}
