////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "authentication.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

namespace yq {
#if 0
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
#endif

    //bool    read_xn(Authentication&ret, const XmlNode*xn)
    //{
        //ret.type    = read_attribute(xn, szType, x_enum<AuthenticationType>).value;
        //ret.data    = read_child(xn, szData, x_string);
        //return true;
    //}

    Ref<Authentication>     x_authentication(const XmlNode* xn)
    {
        std::string     s   = read_attribute(xn, szType, x_sstring);
        const ObjectInfo*   oi  = ObjectInfo::find(s);
        if(!oi)
            return Ref<Authentication>();
            
        Ref<Authentication>     auth    = static_cast<Authentication*>(oi->create());
        if(auth && auth->load(xn))
            return auth;
    
        return Ref<Authentication>();
    }
    
    void    write_xn(XmlNode* xn, const Ref<Authentication>&v)
    {
        if(v.valid()){
            write_attribute(xn, szType, v->metaInfo().name());
            v->save(xn);
        }
    }
}

YQ_OBJECT_IMPLEMENT(yq::Authentication)

