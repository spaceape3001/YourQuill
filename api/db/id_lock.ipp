////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <unordered_map>
#include <tbb/spin_mutex.h>
#include <yq/c++/trait/not_moveable.hpp>

namespace yq {

    struct IDLockRepo : trait::not_copyable, trait::not_moveable {
        struct Data {
            uint8_t count = 0;    // not having more than 254 contentions....
        };

        std::unordered_map<uint64_t, Data>  locks;
        tbb::spin_mutex                     mutex;
        
        bool    inc_read(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            auto& d = locks[i];
            if(d.count == 0xFF)
                return false;
            ++d.count;
            return true;
        }
        
        bool    get_write(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            return locks.insert({ i, { 0xFF } }).second;
        }
        
        void    free_read(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            auto d = locks.find(i);
            if(d != locks.end()){
                if(!--(d->second.count))
                    locks.erase(i);
            }
        }
        
        void    free_write(uint64_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            locks.erase(i);
        }
    };

    template <typename ID>
    IDLockRepo& IDLock<ID>::repo()
    {
        static IDLockRepo s_repo;
        return s_repo;
    }
    
    template <typename ID>
    IDLock<ID> IDLock<ID>::read(ID i)
    {
        if(repo().inc_read(i))
            return IDLock( i.id, Read );
        else
            return IDLock( i.id, None );
    }
        
    template <typename ID>
    IDLock<ID> IDLock<ID>::write(ID i)
    {
        if(repo().inc_read(i))
            return IDLock( i.id, Write );
        else
            return IDLock( i.id, None );
    }
    
        
    template <typename ID>
    IDLock<ID>::IDLock(IDLock&& mv)
    {
        m_id        = mv.m_id;
        m_mode      = mv.m_mode;
        mv.m_mode   = None;
    }
        
    template <typename ID>
    IDLock<ID>& IDLock<ID>::operator=(IDLock&&mv)
    {
        if(this != &mv){
            free();
            m_id        = mv.m_id;
            m_mode      = mv.m_mode;
            mv.m_mode   = None;
        }
        return *this;
    }
        
    template <typename ID>
    void    IDLock<ID>::free()
    {
        switch(m_mode){
        case Read:
            repo().free_read(m_id);
            break;
        case Write:
            repo().free_write(m_id);
            break;
        default:
            break;
        }
    }
}