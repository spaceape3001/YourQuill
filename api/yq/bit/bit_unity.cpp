////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/collection/EnumMap.hpp>
#include <yq/file/Strings.hpp>
#include <yq/file/XmlUtils.hpp>
//#include <yq/log/Logging.hpp>
#include <yq/text/Utils.hpp>

#include "Context.hpp"
#include "Copyright.hpp"
#include "Policy.hpp"

namespace yq {

    Context     x_context(const XmlNode* xn)
    {
        auto    fmt = read_attribute(xn, szFormat, x_enum<Format>);
        Context ret;
        ret.format  = fmt.value;
        ret.data    = to_string(xn);
        return ret;
    }

    void                write_xn(XmlNode* xn, const Context&v)
    {
        if(v.format != Format())
            write_attribute(xn, szFormat, v.format);
        if(!v.data.empty()){
            xn->append_node(
                xn->document()->allocate_node(
                    rapidxml::node_data, nullptr, v.data.c_str(), 0ULL, v.data.size()
                )
            );
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    bool    Copyright::empty() const
    {
        return !not_null();
    }

    void    Copyright::merge(const Copyright&b)
    {
        if(text.empty())
            text        = b.text;
        if(!from)
            from        = b.from;
        if(!to)
            to          = b.to;
        if(stance == AssertDeny())
            stance      = b.stance;
    }


    bool    Copyright::not_null() const
    {
        return (!text.empty()) || from || to || (stance != AssertDeny());
    }

    void    Copyright::reset()
    {
        text.clear();
        from = 0;
        to = 0;
        stance = AssertDeny();
    }


    Result<Copyright>   x_copyright(const XmlNode* xn)
    {
        auto    stance  = read_attribute(xn, szStance, x_enum<AssertDeny>);
        auto    from    = read_attribute(xn, szFrom, x_ushort);
        auto    to      = read_attribute(xn, szTo, x_ushort);
        
        Copyright   res;
        res.stance  = stance.value;
        res.from    = from.value;
        res.to      = to.value;
        res.text    = to_string(xn);
        
        return {res, stance.good && from.good && to.good };
    }

    void                write_xn(XmlNode* xb, const Copyright&v)
    {
        if(v.stance != AssertDeny())
            write_attribute(xb, szStance, v.stance);
        if(v.from)
            write_attribute(xb, szFrom, v.from);
        if(v.to)
            write_attribute(xb, szTo, v.to);
        if(!v.text.empty())
            xb -> value(v.text.c_str(), v.text.size());
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Result<Access>      decode_access(const std::string_view&arg)
    {
        auto    ac  = Access::value_for(arg);
        if(ac.good)
            return { ac.value, true };
        if(is_similar(arg, "first"))
            return { Access(Access::WriteFirst), true };
        if(is_similar(arg, "write"))
            return { Access(Access::ReadWrite), true };
        if(is_similar(arg, "read"))
            return { Access(Access::ReadOnly), true };
        if(is_similar(arg, "deny"))
            return { Access(Access::NoAccess), true };
        return {};
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

}



#if 0

#include "KeyValue.hpp"
#include "NKI.hpp"
#include "Policy.hpp"

#include <yq/FileStrings.hpp>

#include <yq/text/ByteArray.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/log/Logging.hpp>
#include <yq/collection/Reverse.hpp>
#include <yq/collection/Set.hpp>
#include <yq/text/Strings.hpp>
#include <yq/Utilities.hpp>
#include <yq/file/XmlUtils.hpp>

#include <yq/stream/LineCol.hpp>
#include <yq/stream/Ops.hpp>

#include <cctype>
#include <iostream>

#include "Context.ipp"
#include "Copyright.ipp"
#include "KeyValue.ipp"
#include "Policy.ipp"
#endif
