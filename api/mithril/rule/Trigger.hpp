////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/XmlObject.hpp>
#include <yq/basic/Ref.hpp>

namespace yq::mithril {        
    class Trigger : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Trigger, XmlObject)
    public:
    
        //  TBD....
        virtual bool    trigger() const = 0;
    };

    Ref<Trigger>     x_trigger(const XmlNode& xn);
    void    write_xn(XmlNode& xn, const Ref<Trigger>&);

}
