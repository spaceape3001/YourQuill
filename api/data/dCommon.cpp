#include "dCommon.hpp"
#include "FileStrings.hpp"

#include "core/Compare.hpp"
#include "core/EnumMap.hpp"
#include "core/Logging.hpp"
#include "core/XmlUtils.hpp"
#include "core/MetaValueImpl.hxx"

StringSet       string_set(const Vector<std::string_view>& data, bool fParse)
{
    if(fParse){
        StringSet   ret;
        for(String s : data){
            for(String t : s.split(','))
                ret << t.trimmed();
        }
        return ret;
    }
    return StringSet(data.begin(), data.end());
}

Result<Access>      decode_access(const String&arg)
{
    auto    ac  = Access::value_for(arg);
    if(ac.good)
        return Access(ac.value);
    if(is_similar(arg, "first"))
        return Access(Access::WriteFirst);
    if(is_similar(arg, "write"))
        return Access(Access::ReadWrite);
    if(is_similar(arg, "read"))
        return Access(Access::ReadOnly);
    if(is_similar(arg, "deny"))
        return Access(Access::NoAccess);
    return Result<Access>();
}


//  Merges, assumes "a" is the superior to "b"
Access  moderate(Access a, Access b)
{
    switch(a){
    case Access::Default:
        //      This allows everything through
        return b;
    case Access::NoAccess:
        //      This denies everything
        return Access::NoAccess;
    case Access::ReadOnly:
        //      This forces subordinates to be read-only
        switch(b){
        case Access::Default:
        case Access::NoAccess:
        case Access::ReadOnly:
            return b;
        case Access::ReadWrite:
        case Access::WriteFirst:
            return Access::ReadOnly;
        }
        break;
    case Access::ReadWrite:
        // this allows everything through
        return b;
    case Access::WriteFirst:
        // parent overrides on write first
        switch(b){
        case Access::Default:
        case Access::NoAccess:
        case Access::ReadOnly:
        case Access::ReadWrite:
            return b;
        case Access::WriteFirst:
            return Access::ReadWrite;
        }
        break;
    }
    return b;   // shouldn't reach here... but we know how this goes, right?
}

Access    moderate(Access a, Access b, Access limit)
{
    return moderate(limit, moderate(a,b));
}

PolicyMap   moderate(Access a, const PolicyMap& b)
{
    PolicyMap   ret;
    for(DataRole dr : DataRole::all_values())
        ret[dr] = moderate(a, b[dr]);
    return ret;
}

PolicyMap   moderate(const PolicyMap& a, const PolicyMap& b)
{
    PolicyMap   ret;
    for(DataRole dr : DataRole::all_values())
        ret[dr] = moderate(a[dr], b[dr]);
    return ret;
}

PolicyMap   moderate(const PolicyMap& a, const PolicyMap& b, Access limit)
{
    PolicyMap   ret;
    for(DataRole dr : DataRole::all_values())
        ret[dr] = moderate(a[dr], b[dr], limit);
    return ret;
}

void        write_xn(XmlNode*xn, const KVString&v)
{
    if(!v.key.empty())
        write_attribute(xn, szKey, v.key);
    if(!v.value.empty())
        write_x(xn, v.value);
}

KVString    xn_kvstring(const XmlNode* xn)
{
    KVString        r;
    r.key           = read_attribute(xn, szKey, x_string);
    r.value         = x_string(xn);
    return r;
}




//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void                 write_xn(XmlNode*xn, const Variant&v)
{
    static const MetaValue& mtString = metaValue<String>();
    XmlDocument*doc = document_for(xn);
    if(doc){
        if(&v.type() != &mtString)
            xn -> append_attribute(doc -> allocate_attribute(szType,v.type().name().c_str()));
        write_x(xn, v.io_format());
    }
}


Variant             x_variant(const XmlNode*xn)
{
    String              s   = x_string(xn);
    const XmlAttribute* xa  = xn -> first_attribute(szType);
    if(!xa) 
        return s;
    
    const MetaValue*     mt  = MetaValue::lookup(x_string(xa));
    if(!mt)
        return s;
    return Variant::io_parse(*mt, s);
}
