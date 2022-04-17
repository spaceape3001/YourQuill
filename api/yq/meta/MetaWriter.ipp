////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MetaWriter.hpp"

namespace yq {        
    
    Meta::Writer::Writer(Meta* myMeta) : m_meta(myMeta) 
    {
        assert(myMeta);
        assert(thread_safe_write());
    }


    Meta::Writer&     Meta::Writer::alias(std::string_view zAlias)
    {
        m_meta -> m_aliases << std::string_view(zAlias);
        return *this;
    }
    
    Meta::Writer&     Meta::Writer::description(std::string_view zDescription)
    {
        m_meta -> m_description = std::string_view(zDescription);
        return *this;
    }
    
    Meta::Writer&     Meta::Writer::label(std::string_view zLabel)
    {
        m_meta -> m_label = std::string_view(zLabel);
        return *this;
    }
    
    Meta::Writer&     Meta::Writer::tag(std::string_view zKey)
    {
        assert("Tag already set!" && !m_meta->m_tags.has(zKey));
        m_meta -> m_tags[zKey] = Variant(true);
        return *this;
    }
    
    Meta::Writer&     Meta::Writer::tag(std::string_view zKey, Variant&& value)
    {
        assert("Tag already set!" && !m_meta->m_tags.has(zKey));
        m_meta -> m_tags[zKey] = std::move(value);
        return *this;
    }
    

    Meta::Writer&     Meta::Writer::tag(std::string_view zKey, const Variant& value)
    {
        assert("Tag already set!" && !m_meta->m_tags.has(zKey));
        m_meta -> m_tags[zKey] = value;
        return *this;
    }
    
    Meta::Writer&   Meta::Writer::tls()
    {
        m_meta -> m_flags |= TLS;
        return *this;
    }

    Meta::Writer&   Meta::Writer::todo()
    {
        m_meta -> m_flags |= TODO;
        return *this;
    }
}
