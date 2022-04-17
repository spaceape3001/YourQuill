////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "SqlLastId.hpp"
#include "SqlLite.hpp"
#include <sqlite3.h>

namespace yq {
    SqlLastId::SqlLastId(SqlLite& _db) : m_db(_db)
    {
        sqlite3_set_last_insert_rowid(m_db.db(), 0LL);
    }
    
    uint64_t SqlLastId::get() 
    {
        return (uint64_t) sqlite3_last_insert_rowid(m_db.db());
    }
}
