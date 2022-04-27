////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/type/XmlObject.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    
    class Trigger : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Trigger, XmlObject)
    public:
    
        //  TBD....
        virtual bool    trigger() const = 0;
    };

    Ref<Trigger>     x_trigger(const XmlNode* xn);
    void    write_xn(XmlNode* xn, const Ref<Trigger>&);

}