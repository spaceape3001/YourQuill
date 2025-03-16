////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/xml/XmlObject.hpp>
#include <yq/core/Ref.hpp>

namespace yq::mithril {
    class Action : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Action, XmlObject)
    public:
    
        Action();
        virtual ~Action();
    
        //  TBD....
        virtual void    action() const = 0;
        
        static void init_info();
    };

    Ref<Action>     x_action(const XmlNode& xn);
    void    write_xn(XmlNode& xn, const Ref<Action>&);

}
