////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Copyright.hpp"
#include <mithril/kernel/errors.hpp>

namespace yq {

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


    Expect<Copyright>   x_copyright(const XmlNode* xn)
    {
        auto    stance  = read_attribute(xn, szStance, x_enum<AssertDeny>);
        if(!stance)
            return errors::bad_copyright_stance();
        auto    from    = read_attribute(xn, szFrom, x_ushort);
        if(!from)
            return errors::bad_copyright_from();
        auto    to      = read_attribute(xn, szTo, x_ushort);
        if(!to)
            return errors::bad_copyright_to();
        
        Copyright   res;
        res.stance  = *stance;
        res.from    = *from;
        res.to      = *to;
        res.text    = to_string(xn);
        return res;
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
}
