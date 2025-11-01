////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/mithril/io/StdObject.hpp>


namespace yq::mithril {        
    class Authentication : public StdObject {
        YQ_STD_OBJECT_ABSTRACT(Authentication, StdObject);
    public:
        virtual bool  accept(std::string_view) const = 0;

        Authentication();
        ~Authentication();

        static void init_meta();
    };

    //Ref<Authentication>     x_authentication(const XmlNode* xn);
    //void    write_xn(XmlNode* xn, const Ref<Authentication>&);
}
