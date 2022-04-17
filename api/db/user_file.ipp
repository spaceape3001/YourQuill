////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "user_file.hpp"
#include <yq/type/ByteArray.hpp>

namespace yq {


    User::Data&  User::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        bio += b.bio;
        permissions |= b.permissions;
        set_if_empty(brief, b.brief, fOverride);
        return *this;
    }
    
    void  User::Data::reset()
    {
        *this   = Data();
    }
    

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    void  User::File::reset() 
    {
        Data::reset();
    }
    
    bool  User::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQUser);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szUser);
        if(!xn){
            yError() << "No user! In: " << fname << "'";
            return false;
        }
        
        name            = read_child(xn, szName, x_string);
        brief           = read_child(xn, szBrief, x_string);
        authentication  = read_child(xn, szAuthentication, x_authentication);
        if(!skip_bio)
            bio         = read_children(xn, szContext, x_context);
        std::vector<std::string>    theGroups = read_children(xn, szGroup, x_sstring);
        permissions     = read_child(xn, szPermission, x_flag<Permission>);
        return true;
    }
    
    bool  User::File::write(XmlDocument&doc) const 
    {
        xml_start(doc);
        
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        doc.append_node(xroot);
        XmlNode*    xuser   = doc.allocate_node(rapidxml::node_element, szUser);
        xroot -> append_node(xuser);
        
        write_child(xuser, szName, name);
        if(!brief.empty())
            write_child(xuser, szBrief, brief);
        write_child(xuser, szAuthentication, authentication);
        write_children(xuser, szContext, bio);
        if(permissions)
            write_child(xuser, szPermission, permissions);
        return true;
    }

}
