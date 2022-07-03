////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include "Taskable.hpp"

namespace yq {
    namespace engine {
        class SimEngine : trait::not_copyable, trait::not_moveable {
        public:
            SimEngine();
            
            void    step(Second dt);
        
            void    add_taskable(Ref<Taskable>);
            void    remove_taskable(uint64_t);
        
        private:
            std::vector<Ref<Taskable>>   m_taskables;
            Second                      m_time;
        };
    }
}

