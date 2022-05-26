////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Meta.hpp"
#include <basic/DelayInit.hpp>

namespace yq {

    namespace {
        std::string_view     str_start(std::string_view z, const char* pat)
        {
            size_t i;
            for(i=0; (i<z.size()) && *pat; ++i, ++pat)
                if(z[i] != *pat)
                    return z;
            return z.substr(i);
        }
    }

    const Vector<const Meta*>&   Meta::all()
    {
        assert(thread_safe_read());
        return repo().all;
    }


    void    Meta::freeze()
    {
        init();
        repo().openReg  = false;
    }

    void    Meta::init()
    {
        if(thread_safe_write()){
            DelayInit::init_all();
            sweep_all();
        }
    }

    void    Meta::sweep_all()
    {
        if(thread_safe_write()){
            Repo&   r = repo();
            for(const Meta* m : r.all)
                if(m)   // gaps can show
                    const_cast<Meta*>(m) -> m_flags &= ~SWEPT;
            for(const Meta* m : r.all)
                if(m)
                    const_cast<Meta*>(m) -> sweep();
        }
    }

    bool    Meta::thread_safe_read()
    {
        return (!repo().openReg) || (!thread::id());
    }

    bool    Meta::thread_safe_write()
    {
        return repo().openReg && !thread::id();
    }

    bool    Meta::unlocked()
    {
        return repo().openReg;
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Meta::Meta(std::string_view zName, const std::source_location& sl, id_t i) : Meta(zName, nullptr, sl, i)
    {
    }
    
    Meta::Meta(std::string_view zName, Meta* parent, const std::source_location& sl, id_t i) 
    {
        assert(thread_safe_write());
        
        //  strip out the yq namespace
        m_name      = str_start(zName, "yq::");
        m_label     = m_name;                       // default (can be overriden)
        m_parent    = parent;
        m_source    = sl;

        auto& _r     = repo();
        if(i < M_USER){
            m_id    = i;
            _r.all[i]   = this;
        } else {
            m_id    = _r.all.size();
            _r.all << this;
        }
        if(i >= MT_String)
            _r.metas << this;
    }
    
    Meta::~Meta()
    {
        assert(false && "Do NOT delete Meta!  It's not meant to be deleted.");

    }

    bool  Meta::has_tag(std::string_view k) const
    {
        return m_tags.contains(k);
    }

    void  Meta::sweep()
    {
        assert(thread_safe_write());

        if(!(m_flags & SWEPT)){
            sweep_impl();
            m_flags |= SWEPT;
        }
    }

    const Any&  Meta::tag(std::string_view k) const
    {
        static Any bad;
        auto i = m_tags.find(k);
        if(i != m_tags.end())
            return i->second;
        return bad;
    }
}

