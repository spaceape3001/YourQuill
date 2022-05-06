////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserFile.hpp"
#include <yq/io/Strings.hpp>

namespace yq {

    void  User::File::reset() 
    {
        Data::reset();
    }

    bool        User::File::read(KVTree&&attrs, std::string_view body, std::string_view fname) 
    { 
        name        = attrs.value(szName);
        brief       = attrs.value(szBrief);
        permissions = Permissions(attrs.value(szPermission));
        
        const KeyValue* a   = attrs.first(szAuthentication);
        if(a){
            authentication = Authentication::load(*a);
            if(!authentication){
                yError() << "Bad Authentication on file " << fname;
                return false;
            }
        }
        
        if(!skip_bio){
            if(!read_kv(bio, body))
                return false;
        }
        
        return true; 
    }
    
    bool        User::File::write(KVTree&attrs, Stream&str) const
    {
        if(!name.empty())
            attrs.set(szName, name);
        if(!brief.empty())
            attrs.set(szBrief, brief);
        if(permissions != Permissions())
            attrs.set(szPermission, permissions.as_string());
        if(authentication){
            KeyValue        kva;
            kva.key         = szAuthentication;
            authentication -> save(kva);
            attrs << kva;
        }
        write_kv(str, bio);
        return true;
    }
    
    
    //bool  User::File::read(const XmlDocument&doc, std::string_view fname) 
    //{
        //const XmlNode*  xn  = doc.first_node(szYQUser);
        //if(!xn){
            //yError() << "No appropriate root element!  In: '" << fname << "'";
            //return false;
        //}
        
        //xn      = xn -> first_node(szUser);
        //if(!xn){
            //yError() << "No user! In: " << fname << "'";
            //return false;
        //}
        
        //name            = read_child(xn, szName, x_string);
        //brief           = read_child(xn, szBrief, x_string);
        //authentication  = read_child(xn, szAuthentication, x_authentication);
        //if(!skip_bio)
            //bio         = read_children(xn, szContext, x_context);
        //std::vector<std::string>    theGroups = read_children(xn, szGroup, x_sstring);
        //permissions     = read_child(xn, szPermission, x_flag<Permission>);
        //return true;
    //}
    
    //bool  User::File::write(XmlDocument&doc) const 
    //{
        //XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        //doc.append_node(xroot);
        //XmlNode*    xuser   = doc.allocate_node(rapidxml::node_element, szUser);
        //xroot -> append_node(xuser);
        
        //write_child(xuser, szName, name);
        //if(!brief.empty())
            //write_child(xuser, szBrief, brief);
        //write_child(xuser, szAuthentication, authentication);
        //write_children(xuser, szContext, bio);
        //if(permissions)
            //write_child(xuser, szPermission, permissions);
        //return true;
    //}

}
