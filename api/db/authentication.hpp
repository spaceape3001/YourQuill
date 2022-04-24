////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
//#include "enum_authentication_type.hpp"
#include <yq/file/XmlFwd.hpp>
#include <yq/type/XmlObject.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    struct Authentication : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Authentication, XmlObject);
        virtual bool  accept(std::string_view) const = 0;
    };

    Ref<Authentication>     x_authentication(const XmlNode* xn);
//    bool    read_xn(Ref<const Authentication>&, const XmlNode*);
    void    write_xn(XmlNode* xn, const Ref<Authentication>&);
}
