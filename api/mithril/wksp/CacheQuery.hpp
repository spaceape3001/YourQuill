////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Logging.hpp>
#include <0/sql/SqlQueryMap.hpp>
#include <mithril/wksp/CacheStatement.hpp>

#include <set>
#include <vector>
#include <filesystem>

namespace yq::mithril {
    /*! \brief Like SqlQuery but bound with the cache database
    */
    class CacheQuery : public CacheStatement, not_copyable, not_moveable {
    public:
        CacheQuery(std::string_view sql, bool isPersistent=true);
        ~CacheQuery();
    };
}
