////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/type/Ref.hpp>

namespace yq {

    //  This will become a meta object....?
    //  
    //  A constraint is a restriction applied to a class/field/etc
    class Constraint : public RefCount { 
    public:
    
        struct Data;
        using SharedData    = std::shared_ptr<Data>;
        
        virtual void    something() const = 0;
    };

}
