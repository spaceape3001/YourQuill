#include "StdFile.hpp"
#include "Stream.hpp"

#include "yq/Loggers.hpp"
#include "yq/util/Set.hpp"
#include "yq/util/Reverse.hpp"
#include <cctype>

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

Vector<const Attribute*>    Attribute::all(const char* key) const
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

Vector<const Attribute*>    Attribute::all(const String& key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const Attribute*>    Attribute::all(const std::initializer_list<String>& key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const Attribute*>    Attribute::all(const std::initializer_list<const char*>&key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

Vector<const Attribute*>    Attribute::all(const Vector<String>&key) const
{
    Vector<const Attribute*>    ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            ret << &a;
    return ret;
}

bool                Attribute::empty() const 
{ 
    return cmd.empty() && id.empty() && key.empty() && data.empty() && attrs.empty(); 
}

void                Attribute::erase_all(const String& key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                Attribute::erase_all(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return ;
        
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                Attribute::erase_all(const std::initializer_list<String>& key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                Attribute::erase_all(const std::initializer_list<const char*>&key)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                Attribute::erase_all(const Vector<String>& k)
{
    attrs.erase_if([&](const Attribute& a) -> bool {
        return in(key, a.key);
    });
}

void                Attribute::erase_seconds(const String& key)
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

void                Attribute::erase_seconds(const char* key)
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


void                Attribute::erase_seconds(const std::initializer_list<const char*>& key)
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

void                Attribute::erase_seconds(const std::initializer_list<String>& key)
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


void                Attribute::erase_seconds(const Vector<String>& key)
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

Attribute*          Attribute::first(const String& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          Attribute::first(const char* key)
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          Attribute::first(const std::initializer_list<String>& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          Attribute::first(const std::initializer_list<const char*>& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          Attribute::first(const Vector<String>& key)
{
    for(Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::first(const String& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::first(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::first(const std::initializer_list<String>& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::first(const std::initializer_list<const char*>& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::first(const Vector<String>& key) const
{
    for(const Attribute& a : attrs)
        if(in(key, a.key))
            return &a;
    return nullptr;
}

bool                Attribute::has(const String&key) const
{
    return first(key) != nullptr;
}

bool                Attribute::has(const char* key) const
{
    return first(key) != nullptr;
}

bool                Attribute::has(const std::initializer_list<String>& key) const
{
    return first(key) != nullptr;
}

bool                Attribute::has(const std::initializer_list<const char*>& key) const
{
    return first(key) != nullptr;
}

bool                Attribute::has(const Vector<String>&key) const
{
    return first(key) != nullptr;
}

const Attribute*    Attribute::last(const String& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::last(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    if(!key)
        return nullptr;
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}



const Attribute*    Attribute::last(const std::initializer_list<String>& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::last(const std::initializer_list<const char*>& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

const Attribute*    Attribute::last(const Vector<String>& key) const
{
    for(const Attribute& a : reverse(attrs))
        if(in(key, a.key))
            return &a;
    return nullptr;
}

Attribute*          Attribute::set(const String&key, const String& data, bool purge)
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

Attribute*          Attribute::set(const char* key, const String& data, bool purge)
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


Attribute*          Attribute::set(const std::initializer_list<String>&key, const String& data, bool purge)
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

Attribute*          Attribute::set(const std::initializer_list<const char*>&key, const String& data, bool purge)
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


Attribute*          Attribute::set(const Vector<String>&key, const String& data, bool purge)
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

Attribute*          Attribute::set_set(const char*key, const StringSet&vals, const String& sep)
{
    assert(key && "KEY pointer cannot be null");
    return set(key, sep.join(vals), true);
}

Attribute*          Attribute::set_set(const String&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

Attribute*          Attribute::set_set(const std::initializer_list<const char*>&key, const StringSet& vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

Attribute*          Attribute::set_set(const std::initializer_list<String>&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

Attribute*          Attribute::set_set(const Vector<String>&key, const StringSet&vals, const String& sep)
{
    return set(key, sep.join(vals), true);
}

    
String              Attribute::value(const String& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              Attribute::value(const char* key) const
{
    assert(key && "KEY pointer cannot be null");
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              Attribute::value(const std::initializer_list<String>& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              Attribute::value(const std::initializer_list<const char*>& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

String              Attribute::value(const Vector<String>& key) const
{
    const Attribute*a   = first(key);
    return a ? a->data : String();
}

Vector<String>     Attribute::values(const String& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

Vector<String>     Attribute::values(const char* key) const
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

Vector<String>     Attribute::values(const std::initializer_list<String>& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

Vector<String>     Attribute::values(const std::initializer_list<const char*>& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}


Vector<String>     Attribute::values(const Vector<String>& key) const
{
    Vector<String>  ret;
    for(const Attribute& a : attrs)
        if(in(key, a.key) && !a.data.empty())
            ret << a.data;
    return ret;
}

StringSet          Attribute::values_set(const char* key, const String& sep) const
{
    assert(key && "KEY pointer cannot be NULL");
    if(!key)    
        return StringSet();
    return make_set(all(key), sep);
}

StringSet          Attribute::values_set(const String&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          Attribute::values_set(const std::initializer_list<const char*>&key, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          Attribute::values_set(const std::initializer_list<String>&, const String& sep) const
{
    return make_set(all(key), sep);
}

StringSet          Attribute::values_set(const Vector<String>&, const String& sep) const
{
    return make_set(all(key), sep);
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
void    fusion_attrs(Attribute&a, const Attribute&b)
{
    // FULL TODO (later)
    a.attrs += b.attrs;
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void        StdFile::add_attr(Vector<Attribute>& datas, size_t& i, Attribute& v)
{
    size_t      z   = datas[i].indent;
    v.attrs << std::move(datas[i]);
    while( (++i) < datas.size()){
        if(datas[i].indent == z){
            v.attrs << std::move(datas[i]);
            continue;
        } 
        
        while((i < datas.size()) && (datas[i].indent > z))
            add_attr(datas, i, v.attrs.back());

        if(i >= datas.size())
            break;
            
        if(datas[i].indent < z)
            return ;
            
        v.attrs << std::move(datas[i]);
    }
}

bool        StdFile::empty() const
{
    return body.empty() && attrs.empty();
}



bool        StdFile::read(Vector<char>&buffer, const std::string& fname) 
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
    body.reserve(buffer.size());
    Attribute           val;
    unsigned int    line    = 1;
    unsigned int    lident  = 0;
    Vector<Attribute>   datas;

    auto push   = [&](){
        if(!val.empty()){
            lident  = val.indent;
            if(!val.data.empty() && val.data.back() == '"')
                val.data.pop_back();
            
            datas << std::move(val);
            val     = Attribute();
        }
        mode    = Mode::SPACE;
    };
    
    for(char ch : buffer){
        switch(mode){
        case Mode::BODY:
            if(ch)
                body += ch;
            break;
        case Mode::SPACE:
            switch(ch){
            case '#':
                val.cmd = "#";
                mode    = Mode::VALUE;
                break;
            case '\n':
                if(has_body()){
                    mode    = Mode::BODY;
                } else {
                    //  No body means a new attriubte, clone the last indent
                    Attribute   a;
                    a.indent    = lident;
                    datas << a;
                }
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
                yError() << "Non processible character found on line " << line << " : " << fname;
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
                yError() << "Non processible character found on line " << line << " : " << fname;
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
                    
                yError() << "Non processible character found on line " << line << " : " << fname;
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
                yError() << "Non processible character found on line " << line << " : " << fname;
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
        
    if(recursive_attributes()){
        unsigned int        z   = datas[0].indent;
        for(size_t i=0; i<datas.size(); ++i){
            if(datas[i].indent <= z){
                attrs << std::move(datas[i]);
            } else if(datas[i].indent > z){
                add_attr(datas, i, attrs.back());
            }
        }
    } else {
        attrs       = std::move(datas);
    }
    
    return true;
}

void        StdFile::reset() 
{
    attrs.clear();
    body.clear();
}


bool        StdFile::write(Vector<char>& buffer) 
{
    Stream  out(buffer);
    write_out(out, attrs);
    out << '\n' << body;
    return true;
}

void       StdFile::write_out(Stream& out, const Vector<Attribute>&datas, unsigned int depth) const
{
    size_t a = 0;

        // metrics :)
    for(const Attribute& v : datas){
        if(v.empty())
            continue;
        if(v.cmd == "#")
            continue;
        size_t  b   = v.cmd.size() + v.id.size() + v.key.size();
        if(!v.cmd.empty())
            ++b;
        if(!v.id.empty())
            b += 3;
        a   = std::max(a,b);
    }
    ++a;
    
    for(const Attribute& v : datas){
        if(v.empty()){
            out << '\n';
            continue;
        }

        for(unsigned int n = 0;n<depth;++n)
            out << "    ";
            
        if(v.cmd == "#"){
            out << "#" << v.data << '\n';
        } else {
            size_t  tgt = a + out.column();
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
        write_out(out, v.attrs, depth+1);
    }
}

