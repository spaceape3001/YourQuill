////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/wksp/CacheQuery.hpp>

namespace yq::mithril {        
    CacheQuery::CacheQuery(std::string_view sql, bool isPersistent)
    {
        _prepare(sql, isPersistent);
    }
    
    CacheQuery::~CacheQuery()
    {
        _destroy();
    }
}
