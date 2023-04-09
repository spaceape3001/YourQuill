////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserFile.hpp"
#include <basic/Logging.hpp>
#include <mithril/kernel/bit/KeyValue.hpp>
#include <mithril/kernel/io/Strings.hpp>
#include <basic/ErrorDB.hpp>

namespace yq {

    namespace errors {
        using unable_to_load_authentication = error_db::entry<"Unable to load the authentication from the file">;
    }

    void  User::File::reset() 
    {
        Data::reset();
    }

    std::error_code         User::File::read(KVTree&&attrs, std::string_view body, std::string_view fname) 
    { 
        name        = attrs.value(kv::key({"%", "name"}));
        brief       = attrs.value("brief");
        permissions = Permissions(attrs.value("permission"));
        
        const KeyValue* a   = attrs.first(kv::key({"auth", "authentication"}));
        if(a){
            authentication = Authentication::load(*a);
            if(!authentication){
                yError() << "Bad Authentication on file " << fname;
                return errors::unable_to_load_authentication();
            }
        }
        
        if(!skip_bio){
            std::error_code  ec = read_kv(bio, body);
            if(ec != std::error_code())
                return ec;
        }
        
        return std::error_code(); 
    }
    
    std::error_code         User::File::write(KVTree&attrs, Stream&str) const
    {
        if(!name.empty())
            attrs.set("name", name);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(permissions != Permissions())
            attrs.set("permission", permissions.as_string());
        if(authentication){
            KeyValue        kva;
            kva.key         = "authentication";
            authentication -> save(kva);
            attrs << kva;
        }
        write_kv(str, bio);
        return std::error_code(); 
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
        //std::vector<std::string>    theGroups = read_children(xn, szGroup, x_string);
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
