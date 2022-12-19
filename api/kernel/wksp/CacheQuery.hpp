////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Logging.hpp>
#include <basic/SqlQueryMap.hpp>
#include <kernel/wksp/CacheStatement.hpp>

#include <set>
#include <vector>
#include <filesystem>

namespace yq {

    class CacheQuery : public CacheStatement, public trait::not_copyable, public trait::not_moveable {
    public:
        CacheQuery(std::string_view, bool isPersistent=true);
        ~CacheQuery();
    };
    
    
}
