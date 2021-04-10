////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
    res.text    = x_string(xn);
    
    return Result<Copyright>( stance.good && from.good && to.good,res);
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

