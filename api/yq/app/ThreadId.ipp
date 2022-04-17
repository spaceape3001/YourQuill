////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ThreadId.hpp"

#include <atomic>

namespace yq {
    namespace thread {
        unsigned int id()
        {
            static std::atomic<unsigned int>    sNext{0};
            static thread_local unsigned int    sRet    = sNext++;
            return sRet;
        }
        
        static unsigned int kMainThread     = id();
    }

    bool is_main_thread()
    {
        return thread::id() == 0;
    }
}
