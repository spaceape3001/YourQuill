////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/id/Id.hpp>
#include <0/meta/TypeInfo.hpp>
#include <0/math/Counter.hpp>
#include <tbb/spin_mutex.h>
#include "id_auto.ipp"

namespace yq::mithril {
    struct Id::Repo {
        IdTypes                                 compat[HIGH_ID];
        std::unordered_map<id_t, CountU8>       locks;
        tbb::spin_mutex                         mutex;
        
        Repo()
        {
            compat[0] = {};
            for(IdTypeId i=1;i<HIGH_ID;++i)
                compat[i]    = base_types(i) | (IdTypeId) i;
                
            for(size_t n=0;n<5;++n){    // allow for five levels of parentage
                for(IdTypeId i=1;i<HIGH_ID;++i){     // yeah, O(N^2)... but only one time cost
                    for(IdTypeId j=1;j<HIGH_ID;++j){
                        if(compat[i][j])
                            compat[i] |= compat[j];
                    }
                }
            }
        }

        bool    inc_read(id_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            auto& d = locks[i];
            if(d.cnt == 0xFF)
                return false;
            ++d.cnt;
            return true;
        }
        
        bool    get_write(id_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            return locks.insert({ i, { 0xFF } }).second;
        }
        
        void    free_read(id_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            auto d = locks.find(i);
            if(d != locks.end()){
                if(!--(d->second.cnt))
                    locks.erase(i);
            }
        }
        
        void    free_write(id_t i)
        {
            tbb::spin_mutex::scoped_lock    _lock(mutex);
            locks.erase(i);
        }
    };
    
    Id::Repo&   Id::repo()
    {
        static Repo s_repo;
        return s_repo;
    }

    bool        Id::compatible(IdTypeId from, IdTypeId to)
    {
        Repo& _r = repo();
        return from && to && (from < HIGH_ID) && (to < HIGH_ID) && _r.compat[from][to];
    }

////////////////////////////////////////////////////////////////////////////////

    Id::Lock    Id::lock(bool rw) const
    {
        Repo&   _r  = repo();
        if(rw){
            return Lock(*this, _r.get_write(m_value) ? Lock::Write : Lock::None);
        } else {
            return Lock(*this, _r.inc_read(m_value) ? Lock::Read : Lock::None);
        }
    }

    std::string_view         Id::type_name() const
    {
        return type_name(type());
    }
    
////////////////////////////////////////////////////////////////////////////////
    Id::Lock::Lock(Lock&&mv)
    {
        m_id        = mv.m_id;
        m_mode      = mv.m_mode;
        mv.m_mode   = None;
    }

    Id::Lock& Id::Lock::operator=(Lock&&mv)
    {
        if(this != &mv){
            free();
            m_id        = mv.m_id;
            m_mode      = mv.m_mode;
            mv.m_mode   = None;
        }
        return *this;
    }

    void    Id::Lock::free()
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

////////////////////////////////////////////////////////////////////////////////

    std::set<uint64_t>      ids_for(const std::vector<const TypeInfo*>&types)
    {
        std::set<uint64_t>  ret;
        for(const TypeInfo* ti : types){
            if(!ti)
                continue;
            ret.insert(ti->id());
        }
        return ret;
    }
    
}



