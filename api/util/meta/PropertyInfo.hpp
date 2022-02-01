////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Meta.hpp"

namespace yq {

    class PropGetter;
    class PropSetter;

    /*! \brief Attribute of a object/value
    
        A property is an attribute of an object/value.  
        A "state" proeprty is considered to be a field of a structure.  
        If all states are restored, the objects/value should behave identical.  
        Non-state proproperties should generally be read-only, but not always the case.  
        A vector's "set-heading" is an equally valid way to set a vector -- though that could also
        be in a method (or static method);
        
    */
    class PropertyInfo : public Meta {
        friend class PropGetter;
        friend class PropSetter;
    public:
    
        template <typename T> class Writer;
        template <typename T> class VarW;
        template <typename C, typename T> class PropW;
    
        const TypeInfo&     type() const { return m_type; }
        
        Variant             get(const void*) const;
        bool                set(void*, const Variant&) const;
        
        const PropGetter*   getter() const { return m_getter; }
        const PropSetter*   setter() const { return m_setter; }
        
        bool                is_state() const;
        bool                is_static() const;
        
    protected:
        PropertyInfo(const char* zName, const TypeInfo&, Meta*, options_t opts=0);
        
        virtual Variant     get_impl(const void*) const = 0;
        virtual bool        set_impl(void*, const void*) const { return false; }
        
    private:
        const PropSetter*   m_setter    = nullptr;
        const PropGetter*   m_getter    = nullptr;
        const TypeInfo&     m_type;
        DataBlock           m_default;
    };
    
}
