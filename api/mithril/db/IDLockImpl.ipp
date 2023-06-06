////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "IDLock.ipp"

namespace yq {
    namespace mithril {
        bool    IDLockRepo::inc_read(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            auto& d = locks[i];
            if(d.count == 0xFF)
                return false;
            ++d.count;
            return true;
        }
        
        bool    IDLockRepo::get_write(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            return locks.insert({ i, { 0xFF } }).second;
        }
        
        void    IDLockRepo::free_read(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            auto d = locks.find(i);
            if(d != locks.end()){
                if(!--(d->second.count))
                    locks.erase(i);
            }
        }
        
        void    IDLockRepo::free_write(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            locks.erase(i);
        }

    }
}
