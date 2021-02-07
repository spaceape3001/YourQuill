////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Enum.hpp"
#include "Vector.hpp"

namespace {
    void    clean_it(String& text)
    {
        char*    begin   = &text[0];
        char*    c       = begin;
        char*    s       = begin;
        char*    end     = c + text.size();
        
        bool    cmtcxx  = false;
        bool    cmtc    = false;
        char    last    = ' ';
        
        //  scrub the input of comments, non-definition characters
        for(;c<end;++c){
            switch(*c){
            case '/':
                if(cmtcxx)
                    break;
                if(cmtc && (last == '*')){
                    cmtc    = false;
                    break;
                }
                if(cmtc)
                    break;
                if(last == '/')
                    cmtcxx  = true;
                break;
            case '*':
                if(cmtc || cmtcxx)
                    break;
                if(last == '/')
                    cmtc    = true;
                break;
            case '\n':
            case '\r':
                if(cmtcxx)
                    cmtcxx  = false;
                break;
            case ',':
            case '=':
            case '_':
            case '-':
                if(cmtc || cmtcxx)
                    break;
                *s++ = *c;
                break;
            default:
                if(cmtc || cmtcxx)
                    break;
                if(Char8(*c).is_alnum())
                    *s++ = *c;
                break;
            }
            
            last = *c;
        }
        
        if(s != end){
            *s  = '\0';
            text.resize(s - begin);
        }
    }
}



Vector<StringPair>  EnumDef::parse_definition(const String& str)
{
    Vector<StringPair>  ret;
    String          arg = str;
    clean_it(arg);
    for(const String& s : arg.split(',')){
        Vector<String> bits = s.split('=');
        String  key, def;
        switch(bits.size()){
        case 0:
            continue;
        case 1:
            key = bits[0];
            break;
        case 2:
        default:
            key = bits[0];
            def = bits[1];
            break;
        }
        if(!key.empty())
            ret << StringPair(key, def);
    }
    return ret;
}

EnumDef::EnumDef(const String& name, const String& def) : 
    m_name(name), m_defValue(-1)
{
    Vector<StringPair>  pairs   = parse_definition(def);

    int         nextValue   = 0;
    bool        first       = true;
    for(auto &itr : pairs)
    {
        int     theValue = 0;
        if(!itr.second.empty()){
            theValue    = m_name2value.get(itr.second, itr.second.to_integer());
            nextValue   = 1 + theValue;
        } else {
            theValue        = nextValue++;
        }
        m_name2value[itr.first]    = theValue;
        m_keys << itr.first;
        if(!m_value2name.has(theValue)){
            m_value2name[theValue]      = itr.first;
        }
        if(first){
            m_defValue      = theValue;
            first       = false;
        }
    }
    m_keys.sort(std::less<String>());
}


bool    EnumDef::has_key(const String&key) const
{
    return m_name2value.has(key.strip_spaces());
}

bool    EnumDef::has_value(int v) const
{
    return m_value2name.has(v);
}


string_r EnumDef::key_of(int v) const
{
    return m_value2name(v);
}


int     EnumDef::minimum_value() const
{
    if(m_value2name.empty())
        return -1;
    return m_value2name.begin()->first;
}

int     EnumDef::maximum_value() const
{
    if(m_value2name.empty())
        return -1;
    return m_value2name.rbegin()->first;
}


Vector<int>  EnumDef::parse_comma_list(const String&str)  const
{
    Vector<int>  ret;
    for(String s : str.strip_spaces().split(',')){
        auto itr = m_name2value.find(s);
        if(itr != m_name2value.end())
            ret << itr->second;
    }
    return ret;
}

String     EnumDef::make_comma_list(const Vector<int>&vals) const
{
    String::Join  ret(',');
    for(int v : vals){
        auto itr = m_value2name.find(v);
        if(itr != m_value2name.end())
            ret << itr->second;
    }
    return ret;
}

Vector<int>             EnumDef::all_values() const
{
    return m_value2name.keys();
}

Enum            EnumDef::make_enum(int v) const
{
    return Enum(this, v);
}


int_r     EnumDef::value_of(const String& key) const
{
    return m_name2value(key.strip_spaces());
}




//  ============================================================================


Enum::Enum(const EnumDef*def, int v) : m_def(def)
{
    assert(def);
    if(def){
        if(def->has_value(v)){
            m_value = v;
        } else {
            m_value = def->default_value();
        }
    } else {
        m_value     = -1;
    }
}

String Enum::key() const
{
    if(m_def){
        return m_def->key_of(m_value);
    } else {
        return String();
    }
}

Enum&  Enum::operator=(int v)
{
    if(m_def){
        if(m_def->has_value(v)){
            m_value     = v;
        }
    }
    return *this;
}



