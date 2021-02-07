#pragma once

#include "Meta.hpp"

class MetaValue;
class String;
class Variant;


class Global : public Meta {
public:


    static Vector<const Global*>    all();
    static Vector<String>           keys();
    static const Global*            lookup(unsigned int);
    static const Global*            lookup(const String&);
    static Variant                  value(const String&);

    virtual Variant                 value() const = 0;
    const MetaValue&                type() const { return m_type; }
    const String&                   key() const { return name(); }
    
    virtual bool                    can_set() const = 0;
    virtual bool                    set(const Variant&) const = 0;

    virtual const char*             generic() const override { return "Global"; }
    
    template <typename T> class Typed;
    template <typename T> class Writer;
    
protected:
    Global(const char*, const MetaValue&);
    //template <typename T> friend class Writer;
    virtual ~Global();
    
    const MetaValue&     m_type;
};


