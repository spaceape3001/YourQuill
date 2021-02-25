////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Meta.hpp"

class MetaProperty : public Meta {
public:

    struct Writer;

    const MetaObject&   parent() const { return m_parent; }
    const MetaValue&    value() const { return m_value; }
    
    Variant             get(const Object&) const;
    bool                set(Object&, const Variant&) const;

    virtual bool        is_settable() const { return false; }
    virtual const char* generic() const { return "MetaProperty"; }
    
protected:
    MetaProperty(const char*, MetaObject&, const MetaValue&);
    ~MetaProperty();

    virtual Variant     get_impl(const Object&) const = 0;
    virtual bool        set_impl(Object&, const void*) const { return false; }

    MetaObject&         m_parent;
    const MetaValue&    m_value;
};
