////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <mithril/kernel/io/StdObject.hpp>


namespace yq {
    class Authentication : public StdObject {
        YQ_STD_OBJECT_ABSTRACT(Authentication, StdObject);
    public:
        virtual bool  accept(std::string_view) const = 0;
    };

    //Ref<Authentication>     x_authentication(const XmlNode* xn);
    //void    write_xn(XmlNode* xn, const Ref<Authentication>&);
}
