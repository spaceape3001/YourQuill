#include "Attribute.hpp"
#include "yq/util/Set.hpp"
#include "yq/util/Reverse.hpp"

namespace {
    bool    in(const String& a, const String& b)
    {
        if(a.size() != b.size())
            return false;
        for(size_t n=0;n<a.size();++n)
            if(tolower(a[n]) != tolower(b[n]))
                return false;
        return true;
    }
    
    bool    in(const String& a, const char* b)
    {
        if(!b)
            return false;
        size_t  n;
        for(n=0;n<a.size();++n){
            if(!b[n])
                return false;
            if(tolower(a[n]) != tolower(b[n]))
                return false;
        }
        return !b[n];
    }
    
    bool    in(const char* a, const String& b)
    {
        return in(b,a);
    }

    bool    in(const Vector<String>& vec, const String& b)
    {
        for(const String& a : vec)
            if(in(a,b))
                return true;
        return false;
    }

    bool    in(const std::initializer_list<String>& vec, const String& b)
    {
        for(const String& a : vec)
            if(in(a,b))
                return true;
        return false;
    }

    bool    in(const std::initializer_list<const char*>& vec, const String& b)
    {
        for(const char* a : vec)
            if(a && in(b,a))
                return true;
        return false;
    }

    StringSet   make_set(const Vector<const Attribute*>&attrs, const String& sep)
    {
        StringSet       ret;
        for(const Attribute* a : attrs){
            if(!a)
                continue;
            for(const String& s : a->data.split(sep)){
                String v   = s.trimmed();
                if(v.empty())
                    continue;
                ret << v;
            }
        }
        return ret;
    }
}

Vector<const Attribute*>    AttrTree::all(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    Vector<const Attribute*>    ret;
    if(key){
        for(const Attribute& a : attrs)
            if(in(key, a.key))
                ret << &a;
    }
    return ret;
}

Vector<const Attribute*>    AttrTree::all(const String& key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const Attribute*>    AttrTree::all(const std::initializer_list<String>& key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const Attribute*>    AttrTree::all(const std::initializer_list<const char*>&key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const Attribute*>    AttrTree::all(const Vector<String>&key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

bool                AttrTree::empty() const 
{ 
    return attrs.empty(); 
}

void                AttrTree::erase_all(const String& key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                AttrTree::erase_all(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return ;
        
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                AttrTree::erase_all(const std::initializer_list<String>& key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                AttrTree::erase_all(const std::initializer_list<const char*>&key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                AttrTree::erase_all(const Vector<String>& key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                AttrTree::erase_seconds(const String& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    attrs.erase_if([&](const Attribute& a) -> bool {
        if(in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}

void                AttrTree::erase_seconds(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return ;
        
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    attrs.erase_if([&](const Attribute& a) -> bool {
        if(in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}


void                AttrTree::erase_seconds(const std::initializer_list<const char*>& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    attrs.erase_if([&](const Attribute& a) -> bool {
        if(in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}

void                AttrTree::erase_seconds(const std::initializer_list<String>& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    attrs.erase_if([&](const Attribute& a) -> bool {
        if(in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}


void                AttrTree::erase_seconds(const Vector<String>& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    attrs.erase_if([&](const Attribute& a) -> bool {
        if(in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}

Attribute*          AttrTree::first(const String& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          AttrTree::first(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          AttrTree::first(const std::initializer_list<String>& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          AttrTree::first(const std::initializer_list<const char*>& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          AttrTree::first(const Vector<String>& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::first(const String& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::first(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::first(const std::initializer_list<String>& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::first(const std::initializer_list<const char*>& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::first(const Vector<String>& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

bool                AttrTree::has(const String&key) const
{
    return first(key) != nullptr;
}

bool                AttrTree::has(const char* key) const
{
    return first(key) != nullptr;
}

bool                AttrTree::has(const std::initializer_list<String>& key) const
{
    return first(key) != nullptr;
}

bool                AttrTree::has(const std::initializer_list<const char*>& key) const
{
    return first(key) != nullptr;
}

bool                AttrTree::has(const Vector<String>&key) const
{
    return first(key) != nullptr;
}

const Attribute*    AttrTree::last(const String& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::last(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}



const Attribute*    AttrTree::last(const std::initializer_list<String>& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::last(const std::initializer_list<const char*>& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    AttrTree::last(const Vector<String>& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          AttrTree::set(const String&key, const String& data, bool purge)
{
    if(purge)
        erase_seconds(key);
    
    Attribute*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    Attribute   n;
    n.key       = key;
    n.data     = data;
    attrs.push_back(n);
    return &attrs.back();
}

Attribute*          AttrTree::set(const char* key, const String& data, bool purge)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
        
    if(purge)
        erase_seconds(key);
    
    Attribute*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    Attribute   n;
    n.key       = key;
    n.data     = data;
    attrs.push_back(n);
    return &attrs.back();
}


Attribute*          AttrTree::set(const std::initializer_list<String>&key, const String& data, bool purge)
{
    if(key.size() == 0)
        return nullptr;
    if(purge)
        erase_seconds(key);
    
    Attribute*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    Attribute   n;
    n.key       = *(key.begin());
    n.data     = data;
    attrs.push_back(n);
    return &attrs.back();
}

Attribute*          AttrTree::set(const std::initializer_list<const char*>&key, const String& data, bool purge)
{
    if(key.size() == 0)
        return nullptr;
    assert(*key.begin() && "NULL pointers are unacceptable");
    if(!*key.begin())
        return nullptr;
    if(purge)
        erase_seconds(key);
    
    Attribute*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    Attribute   n;
    n.key       = *(key.begin());
    n.data     = data;
    attrs.push_back(n);
    return &attrs.back();
}


Attribute*          AttrTree::set(const Vector<String>&key, const String& data, bool purge)
{
    if(key.empty())
        return nullptr;
    if(purge)
        erase_seconds(key);
    
    Attribute*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    Attribute   n;
    n.key       = *(key.begin());
    n.data     = data;
    attrs.push_back(n);
    return &attrs.back();
}

Attribute*          AttrTree::set_set(const char*key, const StringSet&vals, const String& sep)
{
    assert(key && "KEY pointer cannot be null");
    return set(key, sep.join(vals), true);
}

Attribute*          AttrTree::set_set(const String&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

Attribute*          AttrTree::set_set(const std::initializer_list<const char*>&key, const StringSet& vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

Attribute*          AttrTree::set_set(const std::initializer_list<String>&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

Attribute*          AttrTree::set_set(const Vector<String>&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

    
String              AttrTree::value(const String& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              AttrTree::value(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              AttrTree::value(const std::initializer_list<String>& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              AttrTree::value(const std::initializer_list<const char*>& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              AttrTree::value(const Vector<String>& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

Vector<String>     AttrTree::values(const String& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

Vector<String>     AttrTree::values(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    Vector<String>  ret;
    if(key){
        for(const Attribute& a : attrs)
            if(in(key, a.key) && !a.data.empty())
                ret << a.data;
    }
    return ret;
}

Vector<String>     AttrTree::values(const std::initializer_list<String>& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

Vector<String>     AttrTree::values(const std::initializer_list<const char*>& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}


Vector<String>     AttrTree::values(const Vector<String>& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

StringSet          AttrTree::values_set(const char* key, const String& sep) const
{
    assert(key && "KEY pointer cannot be NULL");
    if(!key)    
        return StringSet();
    return make_set(all(key), sep);
}

StringSet          AttrTree::values_set(const String&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          AttrTree::values_set(const std::initializer_list<const char*>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          AttrTree::values_set(const std::initializer_list<String>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          AttrTree::values_set(const Vector<String>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
AttrTree&          AttrTree::operator+=(Attribute&& v)
{
    attrs << std::move(v);
    return *this;
}

AttrTree&          AttrTree::operator+=(const Attribute& v)
{
    attrs << v;
    return *this;
}

AttrTree&          AttrTree::operator+=(AttrTree&&rhs)
{
    // FULL TODO (later)
    attrs += std::move(rhs.attrs);
    return *this;
}

AttrTree&          AttrTree::operator+=(const AttrTree&rhs)
{
    // FULL TODO (later)
    attrs += rhs.attrs;
    return *this;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bool                Attribute::empty() const
{
    return cmd.empty() && id.empty() && key.empty() && data.empty() && attrs.empty(); 
}

