////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Logging.hpp>
#include <sql/SqlQueryMap.hpp>
#include <mithril/kernel/wksp/CacheStatement.hpp>

#include <set>
#include <vector>
#include <filesystem>

namespace yq {

    /*! \brief Like SqlQuery but bound with the cache database
    */
    class CacheQuery : public CacheStatement, not_copyable, not_moveable {
    public:
        CacheQuery(std::string_view sql, bool isPersistent=true);
        ~CacheQuery();
    };
    
    
}
