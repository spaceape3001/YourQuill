////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/type/XmlObject.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    
    class Action : public XmlObject, public RefCount {
        YQ_OBJECT_DECLARE(Action, XmlObject)
    public:
    
        //  TBD....
        virtual void    action() const = 0;
    };

    Ref<Action>     x_action(const XmlNode* xn);
    void    write_xn(XmlNode* xn, const Ref<Action>&);

}
