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
    namespace engine {
    
        struct SimData;

        class TaskableInfo : public ObjectInfo {
        };
    
        class Taskable : public Object, public RefCount, public UniqueID {
        public:
            virtual void    step(SimData&) = 0;
            virtual ~Taskable(){}
        };
        
    }
}

