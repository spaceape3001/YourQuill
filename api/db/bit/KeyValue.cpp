////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "KeyValue.hpp"

#include <util/Logging.hpp>
#include <util/Reverse.hpp>
#include <util/Set.hpp>
#include <util/Stream.hpp>
#include <util/Utilities.hpp>

#include <cctype>
#include <iostream>

namespace {

    StringSet   make_set(const Vector<const KeyValue*>&subs, const String& sep)
    {
        StringSet       ret;
        for(const KeyValue* a : subs){
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

Vector<const KeyValue*>    KVTree::all(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    Vector<const KeyValue*>    ret;
    if(key){
        for(const KeyValue& a : subs)
            if(is_in(key, a.key))
                ret << &a;
    }
    return ret;
}

Vector<const KeyValue*>    KVTree::all(const String& key) const
{
    Vector<const KeyValue*>    ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const KeyValue*>    KVTree::all(const std::initializer_list<String>& key) const
{
    Vector<const KeyValue*>    ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const KeyValue*>    KVTree::all(const std::initializer_list<const char*>&key) const
{
    Vector<const KeyValue*>    ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const KeyValue*>    KVTree::all(const Vector<String>&key) const
{
    Vector<const KeyValue*>    ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            ret << &a;
    return ret;
}

bool                KVTree::empty() const 
{ 
    return subs.empty(); 
}

void                KVTree::erase_all(const String& key)
{
    subs.erase_if([&](const KeyValue& a) -> bool {
        return is_in(key, a.key);
    });
}

void                KVTree::erase_all(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return ;
        
    subs.erase_if([&](const KeyValue& a) -> bool {
        return is_in(key, a.key);
    });
}

void                KVTree::erase_all(const std::initializer_list<String>& key)
{
    subs.erase_if([&](const KeyValue& a) -> bool {
        return is_in(key, a.key);
    });
}

void                KVTree::erase_all(const std::initializer_list<const char*>&key)
{
    subs.erase_if([&](const KeyValue& a) -> bool {
        return is_in(key, a.key);
    });
}

void                KVTree::erase_all(const Vector<String>& key)
{
    subs.erase_if([&](const KeyValue& a) -> bool {
        return is_in(key, a.key);
    });
}

void                KVTree::erase_seconds(const String& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    subs.erase_if([&](const KeyValue& a) -> bool {
        if(is_in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}

void                KVTree::erase_seconds(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return ;
        
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    subs.erase_if([&](const KeyValue& a) -> bool {
        if(is_in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}


void                KVTree::erase_seconds(const std::initializer_list<const char*>& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    subs.erase_if([&](const KeyValue& a) -> bool {
        if(is_in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}

void                KVTree::erase_seconds(const std::initializer_list<String>& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    subs.erase_if([&](const KeyValue& a) -> bool {
        if(is_in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}


void                KVTree::erase_seconds(const Vector<String>& key)
{
    //  WARNING!  This will break if std::remove_if is ever NOT forward-sequential
    bool    f   = false;
    subs.erase_if([&](const KeyValue& a) -> bool {
        if(is_in(key, a.key)){
            if(f)
                return true;
            f = true;
        }
        return false;
    });
}

KeyValue*          KVTree::first(const String& key)
{
    for(KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

KeyValue*          KVTree::first(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

KeyValue*          KVTree::first(const std::initializer_list<String>& key)
{
    for(KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

KeyValue*          KVTree::first(const std::initializer_list<const char*>& key)
{
    for(KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

KeyValue*          KVTree::first(const Vector<String>& key)
{
    for(KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

KeyValue*          KVTree::first(const std::initializer_list<const char*>&key, const String&val, bool fCreate)
{
    for(KeyValue& a : subs){
        if(is_in(key, a.key) && is_in(val, a.data))
            return &a;
    }
    if(fCreate && key.size()){
        KeyValue n;
        n.key   = *(key.begin());
        n.data  = val;
        subs << n;
        return &subs.back();
    }
    
    return nullptr;
}


const KeyValue*    KVTree::first(const String& key) const
{
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}



const KeyValue*    KVTree::first(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::first(const std::initializer_list<String>& key) const
{
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::first(const std::initializer_list<const char*>& key) const
{
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::first(const Vector<String>& key) const
{
    for(const KeyValue& a : subs)
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*  KVTree::first(const std::initializer_list<const char*>&key, const String&val) const
{
    for(const KeyValue& a : subs)
        if(is_in(key, a.key) && is_in(val, a.data))
            return &a;
    return nullptr;
}



const KeyValue*    KVTree::first_noncmd(const String& key) const
{
    for(const KeyValue& a : subs)
        if(a.cmd.empty() && is_in(key, a.key))
            return &a;
    return nullptr;
}



const KeyValue*    KVTree::first_noncmd(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const KeyValue& a : subs)
        if(a.cmd.empty() && is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::first_noncmd(const std::initializer_list<String>& key) const
{
    for(const KeyValue& a : subs)
        if(a.cmd.empty() && is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::first_noncmd(const std::initializer_list<const char*>& key) const
{
    for(const KeyValue& a : subs)
        if(a.cmd.empty() && is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::first_noncmd(const Vector<String>& key) const
{
    for(const KeyValue& a : subs)
        if(a.cmd.empty() && is_in(key, a.key))
            return &a;
    return nullptr;
}

    #if 0
    namespace {
        Vector<AttrData>::iterator      find(Vector<AttrData>& dst, const AttrData& ref)
        {
            for(auto i = dst.begin(); i != dst.end(); ++i){
                if(!is_similar(i->key, ref.key))
                    continue;
                if(ref.id != i->id)
                    continue;
                if(!is_similar(ref.root, i->root))
                    continue;
            }
            return dst.end();
        }

        void    fuse(Vector<AttrData>& dst, const Vector<AttrData>& src, const Root* rt)
        {
            for(const AttrData& a : src){
                switch(a.action){
                case Action::Add:
                    dst << a;
                    dst.back().root = rt -> key();
                    break;
                case Action::Modify: {
                    auto i = find(dst, a);
                    if(i != dst.end()){
                        //  TODO
                        fuse(i->sub, a.sub, rt);
                    }
                    break;
                }
                case Action::Remove: {
                    auto i = find(dst, a);
                    if(i != dst.end())
                        dst.erase(i);
                    break;
                }
                case Action::Existing: {
                    auto i = find(dst, a);
                    if(i != dst.end())
                        fuse(i->sub, a.sub, rt);
                    break;
                }
                }
            }
            
        }
    }
    #endif


void                KVTree::fusion(const KVTree&rhs) 
{
    //  TODO properly (LATER)
    subs    += rhs.subs;
}


bool                KVTree::has(const String&key) const
{
    return first(key) != nullptr;
}

bool                KVTree::has(const char* key) const
{
    return first(key) != nullptr;
}

bool                KVTree::has(const std::initializer_list<String>& key) const
{
    return first(key) != nullptr;
}

bool                KVTree::has(const std::initializer_list<const char*>& key) const
{
    return first(key) != nullptr;
}

bool                KVTree::has(const Vector<String>&key) const
{
    return first(key) != nullptr;
}

const KeyValue*    KVTree::last(const String& key) const
{
    for(const KeyValue& a : reverse(subs))
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::last(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const KeyValue& a : reverse(subs))
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}



const KeyValue*    KVTree::last(const std::initializer_list<String>& key) const
{
    for(const KeyValue& a : reverse(subs))
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::last(const std::initializer_list<const char*>& key) const
{
    for(const KeyValue& a : reverse(subs))
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}

const KeyValue*    KVTree::last(const Vector<String>& key) const
{
    for(const KeyValue& a : reverse(subs))
        if(is_in(key, a.key))
            return &a;
    return nullptr;
}


size_t              KVTree::max_key_size(bool recursive) const
{
    size_t  mx  = 0;
    for(auto& a : subs){
        mx      = std::max(mx, a.key.size());
        if(recursive)
            mx  = std::max(mx, a.max_key_size(true));
    }
    return mx;
}


KeyValue*          KVTree::set(const String&key, const String& data, bool purge)
{
    if(purge)
        erase_seconds(key);
    
    KeyValue*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    KeyValue   n;
    n.key       = key;
    n.data     = data;
    subs.push_back(n);
    return &subs.back();
}

KeyValue*          KVTree::set(const char* key, const String& data, bool purge)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
        
    if(purge)
        erase_seconds(key);
    
    KeyValue*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    KeyValue   n;
    n.key       = key;
    n.data     = data;
    subs.push_back(n);
    return &subs.back();
}


KeyValue*          KVTree::set(const std::initializer_list<String>&key, const String& data, bool purge)
{
    if(key.size() == 0)
        return nullptr;
    if(purge)
        erase_seconds(key);
    
    KeyValue*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    KeyValue   n;
    n.key       = *(key.begin());
    n.data     = data;
    subs.push_back(n);
    return &subs.back();
}

KeyValue*          KVTree::set(const std::initializer_list<const char*>&key, const String& data, bool purge)
{
    if(key.size() == 0)
        return nullptr;
    assert(*key.begin() && "NULL pointers are unacceptable");
    if(!*key.begin())
        return nullptr;
    if(purge)
        erase_seconds(key);
    
    KeyValue*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    KeyValue   n;
    n.key       = *(key.begin());
    n.data     = data;
    subs.push_back(n);
    return &subs.back();
}


KeyValue*          KVTree::set(const Vector<String>&key, const String& data, bool purge)
{
    if(key.empty())
        return nullptr;
    if(purge)
        erase_seconds(key);
    
    KeyValue*  a   = first(key);
    if(a){
        a->data    = data;
        return a;
    }
    
    KeyValue   n;
    n.key       = *(key.begin());
    n.data     = data;
    subs.push_back(n);
    return &subs.back();
}

KeyValue*          KVTree::set_set(const char*key, const StringSet&vals, const String& sep)
{
    assert(key && "KEY pointer cannot be null");
    return set(key, sep.join(vals), true);
}

KeyValue*          KVTree::set_set(const String&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

KeyValue*          KVTree::set_set(const std::initializer_list<const char*>&key, const StringSet& vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

KeyValue*          KVTree::set_set(const std::initializer_list<String>&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

KeyValue*          KVTree::set_set(const Vector<String>&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

KeyValue*          KVTree::set_set(const std::initializer_list<const char*>&key, const Set<uint16_t>&vals, const String& sep)
{
    StringSet       them;
    for(uint16_t v : vals)
        them << String::number(v);
    return set_set(key,them,sep);
}
    
String              KVTree::value(const String& key, bool excludeCmds) const
{
    const KeyValue*a   = excludeCmds ? first_noncmd(key) : first(key);
    return a ? a->data : String();
}

String              KVTree::value(const char* key, bool excludeCmds) const
{
    assert(key && "KEY pointer cannot be null");
    const KeyValue*a   = excludeCmds ? first_noncmd(key) : first(key);
    return a ? a->data : String();
}

String              KVTree::value(const std::initializer_list<String>& key, bool excludeCmds) const
{
    const KeyValue*a   = excludeCmds ? first_noncmd(key) : first(key);
    return a ? a->data : String();
}

String              KVTree::value(const std::initializer_list<const char*>& key, bool excludeCmds) const
{
    const KeyValue*a   = excludeCmds ? first_noncmd(key) : first(key);
    return a ? a->data : String();
}

String              KVTree::value(const Vector<String>& key, bool excludeCmds) const
{
    const KeyValue*a   = excludeCmds ? first_noncmd(key) : first(key);
    return a ? a->data : String();
}

Vector<String>     KVTree::values(const String& key) const
{
    Vector<String>  ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

Vector<String>     KVTree::values(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    Vector<String>  ret;
    if(key){
        for(const KeyValue& a : subs)
            if(is_in(key, a.key) && !a.data.empty())
                ret << a.data;
    }
    return ret;
}

Vector<String>     KVTree::values(const std::initializer_list<String>& key) const
{
    Vector<String>  ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

Vector<String>     KVTree::values(const std::initializer_list<const char*>& key) const
{
    Vector<String>  ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}


Vector<String>     KVTree::values(const Vector<String>& key) const
{
    Vector<String>  ret;
    for(const KeyValue& a : subs)
        if(is_in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

StringSet          KVTree::values_set(const char* key, const String& sep) const
{
    assert(key && "KEY pointer cannot be NULL");
    if(!key)    
        return StringSet();
    return make_set(all(key), sep);
}

StringSet          KVTree::values_set(const String&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          KVTree::values_set(const std::initializer_list<const char*>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          KVTree::values_set(const std::initializer_list<String>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          KVTree::values_set(const Vector<String>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

Set<uint16_t>   KVTree::values_set_u16(const std::initializer_list<const char*>&key, const String& sep) const
{
    Set<uint16_t>   ret;
    for(const String& s : values_set(key, sep)){
        auto u  = s.to_uint16();
        if(u.good)
            ret << u.value;
    }
    return ret;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
KVTree&          KVTree::operator+=(KeyValue&& v)
{
    subs << std::move(v);
    return *this;
}

KVTree&          KVTree::operator+=(const KeyValue& v)
{
    subs << v;
    return *this;
}

KVTree&          KVTree::operator+=(KVTree&&rhs)
{
    // FULL TODO (later)
    subs += std::move(rhs.subs);
    return *this;
}

KVTree&          KVTree::operator+=(const KVTree&rhs)
{
    // FULL TODO (later)
    subs += rhs.subs;
    return *this;
}

KVTree&           KVTree::operator<<(KeyValue&&v)
{
    subs << std::move(v);
    return *this;
}

KVTree&           KVTree::operator<<(const KeyValue&v)
{
    subs << v;
    return *this;
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

namespace {

    void    add_attr(Vector<KeyValue>&datas, size_t&i, KeyValue&v)
    {
        size_t      z   = datas[i].indent;
        v.subs << std::move(datas[i]);
        while( (++i) < datas.size()){
            if(datas[i].indent == z){
                v.subs << std::move(datas[i]);
                continue;
            } 
            
            while((i < datas.size()) && (datas[i].indent > z))
                add_attr(datas, i, v.subs.back());

            if(i >= datas.size())
                break;
            if(datas[i].indent < z)
                break;
                
            v.subs << std::move(datas[i]);
        }
    }
    
}

bool    KVTree::parse(const Vector<char>& buffer, String* body, bool recursive, const std::string& fname)
{
    if(buffer.empty())
        return true;
        
    enum class Mode {
        SPACE,          //!< Space at start of line
        CMD,            //!< Cmd at the start of the line
        CMDS,           //!< Space between command and ID
        ID,             //!< ID
        IDS,            //!< Space between ID and key
        KEY,            //!< Key
        KEYS,           //!< Space between key and data
        VALUE,          //!< Everything afterwards to new-line
        BODY            //!< Body of the document
    };

    Mode            mode    = Mode::SPACE;
    if(body)
        body->reserve(buffer.size());
    KeyValue           val;
    unsigned int    line    = 1;
    unsigned int    lident  = 0;
    Vector<KeyValue>   datas;

    auto push   = [&](){
        if(!val.empty()){
            lident  = val.indent;
            if(!val.data.empty() && val.data.back() == '"')
                val.data.pop_back();
            
            datas << std::move(val);
            val     = KeyValue();
        }
        mode    = Mode::SPACE;
    };
    
    for(char ch : buffer){
        if(!ch)
            continue;
        switch(mode){
        case Mode::BODY:
            if(ch)
                *body += ch;
            break;
        case Mode::SPACE:
            switch(ch){
            case '#':
                val.cmd = "#";
                mode    = Mode::VALUE;
                break;
            case '\n':
                if(body){
                    mode    = Mode::BODY;
                } else {
                    //  No body means a new attriubte, clone the last indent
                    KeyValue   a;
                    a.indent    = lident;
                    datas << a;
                }
                ++line;
                break;
            case '%':
                mode    = Mode::KEY;
                val.key += '%';
                break;
            case '[':
                mode    = Mode::ID;
                break;
            default:
                if(ispunct(ch)){
                    mode    = Mode::CMD;
                    val.cmd += ch;
                    break;
                }
                if(isalnum(ch)){
                    mode    = Mode::KEY;
                    val.key += ch;
                    break;
                }
                if(isspace(ch)){
                    if(datas.empty() && !val.indent){
                        yWarning() << "First attribute should always start left-adjusted! " << line << " : " << fname;
                    }
                    ++ val.indent;
                    break;
                }
                //  This state is an error!
                yError() << "Non processible character (in blank-line) found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::CMD:
            if(isspace(ch)){
                mode    = Mode::CMDS;
                break;
            }
            if(isalnum(ch) || ispunct(ch)){
                val.cmd += ch;
                break;
            }
            if(ch == '\n'){
                push();
                break;
            }
            break;
        case Mode::CMDS:
            switch(ch){
            case '[':
                mode    = Mode::ID;
                break;
            case '\n':
                push();
                break;
            default:
                if(isalnum(ch)){
                    val.key += ch;
                    mode    = Mode::KEY;
                    break;
                }
                if(isspace(ch))
                    break;
                yError() << "Non processible character (in post command space) found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::ID:
            switch(ch){
            case ']':
                mode    = Mode::IDS;
                break;
            case '\n':
                yError() << "Line break during ID, on line " << line << " : " << fname;
                push();
                break;
            case '[':
                yError() << "Stray '[' detected on line " << line << " : " << fname;
                break;
      
            default:
                if(isalnum(ch) || ispunct(ch)){
                    val.id += ch;
                    break;
                }
                if(isspace(ch)){
                    yError() << "Erroneous space detected in ID on line " << line << " : " << fname;
                    break;
                }
                break;
            }
            break;
        case Mode::IDS:
            switch(ch){
            case '\n':
                push();
                break;
            default:
                if(isalnum(ch)){
                    val.key += ch;
                    mode    = Mode::KEY;
                    break;
                }
                if(isspace(ch))
                    break;
                    
                yError() << "Non processible character (in post ID space) found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::KEY:
            switch(ch){
            case '\n':
                push();
                break;
            default:
                if(isalnum(ch) || ispunct(ch)){
                    val.key += ch;
                    break;
                }
                if(isspace(ch)){
                    mode        = Mode::KEYS;
                    break;
                }
                yError() << "Non processible character (in post key space) found on line " << line << " : " << fname;
                break;
            }
            break;
        case Mode::KEYS:
            switch(ch){
            case '\n':
                push();
                break;
            case '"':
                mode        = Mode::VALUE;
                break;
            default:
                if(isspace(ch)){
                    break;
                } else {
                    val.data += ch;
                    mode    =    Mode::VALUE;
                    break;
                }
                break;
            }
            break;
        case Mode::VALUE:
            if(ch == '\n'){
                push();
            } else {
                val.data += ch;
            }
            break;
        }
    }
    
    if(datas.empty())
        return true;
        
    if(recursive){
        unsigned int        z   = datas[0].indent;
        for(size_t i=0; i<datas.size(); ++i){
            if(datas[i].indent <= z){
                subs << std::move(datas[i]);
            } else {
                add_attr(datas, i, subs.back());
                if(i<datas.size())
                    subs << std::move(datas[i]);
            }
        }
    } else {
        subs       = std::move(datas);
    }
    
    return true;
}

namespace {
    size_t      max_pad(const KVTree& at)
    {
        size_t  a   = 0;
        for(const KeyValue& v : at.subs){
            if(v.empty())
                continue;
            if(v.cmd == "#")
                continue;
            size_t  b   = v.cmd.size() + v.id.size() + v.key.size();
            if(!v.cmd.empty())
                ++b;
            if(!v.id.empty())
                b += 3;
            a   = std::max({a,b,max_pad(v)});
        }
        ++a;
        return a;
    }

    void       write_subs(Stream& out, const Vector<KeyValue>&datas, unsigned int mxval, unsigned int depth=0) 
    {
        for(const KeyValue& v : datas){
            if(v.empty()){
                out << '\n';
                continue;
            }

            for(unsigned int n = 0;n<depth;++n)
                out << "    ";
                
            if(v.cmd == "#"){
                out << "#" << v.data << '\n';
            } else {
                size_t  tgt = mxval + out.column();
                if(!v.cmd.empty())
                    out << v.cmd << ' ';
                if(!v.id.empty())
                    out << '[' << v.id << ']' << ' ';
                if(!v.key.empty())
                    out << v.key;
                while(out.column() < tgt)
                    out << ' ';
                out << v.data << '\n';
            }
            write_subs(out, v.subs, mxval, depth+1);
        }
    }

}




void    KVTree::write(Stream& out) const
{
    write_subs(out, subs, max_pad(*this));
}

void    KVTree::write(Vector<char>&buffer) const
{
    Stream  out(buffer);
    write(out);
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bool                KeyValue::empty() const
{
    return cmd.empty() && id.empty() && key.empty() && data.empty() && subs.empty(); 
}

