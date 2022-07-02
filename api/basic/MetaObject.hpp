////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Object.hpp>
#include <basic/Ref.hpp>
#include <basic/UniqueID.hpp>

namespace yq {
    class MetaObject : public Object, public RefCount, public UniqueID {
        YQ_OBJECT_DECLARE(MetaObject, Object)
    public:
        uint64_t id() const { return UniqueID::id(); }
        
    protected:
        MetaObject();
        virtual ~MetaObject();
    };
}

