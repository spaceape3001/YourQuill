////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Policy.hpp"
#include <basic/EnumMap.hpp>

namespace yq {

    Result<Access>      decode_access(std::string_view arg)
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
