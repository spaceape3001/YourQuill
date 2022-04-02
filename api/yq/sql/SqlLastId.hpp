////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cstdint>

namespace yq {
    class SqlLite;

    /*! Helper for last ids
    
        This helper will set the last ID to zero in the constructor, will
        return the result afterwards.
    */
    class SqlLastId {
    public:
        
        SqlLastId(SqlLite& _db);
        uint64_t get();
        
    private:
        SqlLite&    m_db;
    };
    
}
