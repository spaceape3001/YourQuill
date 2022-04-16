////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/enum/AuthenticationType.hpp>
#include <yq/file/XmlFwd.hpp>

namespace yq {
    struct Authentication {
        std::string         data;
        AuthenticationType  type;
        
        bool    accept(std::string_view) const;
    };

    Authentication     x_authentication(const XmlNode* xn);
    bool    read_xn(Authentication&, const XmlNode*);
    void    write_xn(XmlNode* xn, const Authentication&);
}
