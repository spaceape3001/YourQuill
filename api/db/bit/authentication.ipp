////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "authentication.hpp"
#include <yq/meta/ObjectInfoWriter.hpp>

namespace yq {

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
        Object* obj         = oi->create();
        Ref<Authentication>     auth    = dynamic_cast<Authentication*>(obj);
        if(auth && auth->load(xn))
            return auth;
        if(obj)
            delete obj;
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

