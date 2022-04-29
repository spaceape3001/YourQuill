////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/c++/trait/not_copyable.hpp>
#include <unordered_map>
#include <tbb/spin_mutex.h>

namespace yq {

    struct IDLockRepo;

    template <typename ID>
    class IDLock : trait::not_copyable {
    
    public:
    
        enum Mode {
            None = 0,
            Read,
            Write
        };
        
        static IDLock read(ID i);
        static IDLock write(ID i);
    
        IDLock(){}
        
        IDLock(IDLock&&);
        
        IDLock& operator=(IDLock&&);
        
        ~IDLock() { free(); }
        
        Mode        mode() const { return m_mode; }
        ID          id() const { return ID{m_id}; }
        
        operator bool () const noexcept { return m_mode != None; }
        void    free();

    private:

        uint64_t    m_id        = 0;
        Mode        m_mode      = None;
        
        IDLock(uint64_t i, Mode m) : m_id(i), m_mode(m) {}

        static IDLockRepo& repo();
    };
}
