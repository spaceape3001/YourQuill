////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/xml/XmlObject.hpp>
#include <yq/core/Ref.hpp>

namespace yq::mithril {        
    class Constraint : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Constraint, XmlObject)
    public:
    
        Constraint();
        virtual ~Constraint();
    
        //  TBD....
        virtual void    constraint() const = 0;

        static void init_info();
    };

    Ref<Constraint>     x_constraint(const XmlNode& xn);
    void    write_xn(XmlNode& xn, const Ref<Constraint>&);

}

