////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/basic/Logging.hpp>
#include <yq/sql/SqlQueryMap.hpp>
#include <mithril/wksp/CacheStatement.hpp>

#include <set>
#include <vector>
#include <filesystem>

namespace yq::mithril {
    /*! \brief Like SqlQuery but bound with the cache database
    */
    class CacheQuery : public CacheStatement {
    public:
        CacheQuery(std::string_view sql, bool isPersistent=true);
        ~CacheQuery();
    private:
        CacheQuery(const CacheQuery&) = delete;
        CacheQuery(CacheQuery&&) = delete;
        CacheQuery& operator=(const CacheQuery&) = delete;
        CacheQuery& operator=(CacheQuery&&) = delete;
    };
}
