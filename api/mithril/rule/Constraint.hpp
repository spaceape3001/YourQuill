////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/XmlObject.hpp>
#include <basic/Ref.hpp>

namespace yq::mithril {        
    class Constraint : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Constraint, XmlObject)
    public:
    
        //  TBD....
        virtual void    constraint() const = 0;
    };

    Ref<Constraint>     x_constraint(const XmlNode* xn);
    void    write_xn(XmlNode* xn, const Ref<Constraint>&);

}
