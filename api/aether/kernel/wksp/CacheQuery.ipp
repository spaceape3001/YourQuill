////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/wksp/CacheQuery.hpp>

namespace yq {
    CacheQuery::CacheQuery(std::string_view sql, bool isPersistent)
    {
        _prepare(sql, isPersistent);
    }
    
    CacheQuery::~CacheQuery()
    {
        _destroy();
    }
}
