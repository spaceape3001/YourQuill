////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "bit_authentication.hpp"

namespace yq {

    bool    Authentication::accept(std::string_view p) const
    {
        switch(type){
        case AuthenticationType::None:
            return true;
        case AuthenticationType::Plain:
            return p == data;
        
        //  TODO the other types
        //  NOTE, we're not doing *FAKE* crypto because that'd lead to a false
        //  sense of security.    
        
        default:
            return false;
        }
    }

    bool    read_xn(Authentication&ret, const XmlNode*xn)
    {
        ret.type    = read_attribute(xn, szType, x_enum<AuthenticationType>).value;
        ret.data    = read_child(xn, szData, x_string);
        return true;
    }

    Authentication     x_authentication(const XmlNode* xn)
    {
        Authentication  ret;
        read_xn(ret, xn);
        return ret;
    }
    
    void    write_xn(XmlNode* xn, const Authentication&v)
    {
        if(v.type != AuthenticationType::None){
            write_attribute(xn, szType, v.type);
            if(!v.data.empty())
                write_child(xn, szData, v.data);
        }
    }

}
