////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/preamble.hpp>

struct sqlite3_stmt;

namespace yq {
    class SqlLite;
    
    class SqlQuery {
    public:
        enum Result {
            Error   = -2,
            Busy    = -1,
            Done    = 0,
            Row     = 1
        };
        
        static bool exec(SqlLite&, const std::string&);
    
        SqlQuery(SqlLite&, std::string_view, bool isPersistent=true);
        SqlQuery(SqlQuery&&);
        ~SqlQuery();
        SqlQuery&   operator=(SqlQuery&&);
        
        bool                valid() const { return m_stmt != nullptr; }
        sqlite3_stmt*       stmt() const { return m_stmt; }
        std::string_view    sql() const;
        
        //! Reset for a new bind/values
        void                reset();
        
        //! Steps/executes the statement
        Result               step();
        
        //! Binds NULL to column
        bool                bind(int);
        //! Binds a BLOB
        //! \note this data MUST be available until unbind() is called (or reset_all)
        bool                bind(int, std::span<uint8_t>);
        //! \brief Boolean (maps to 0/1)
        bool                bind(int, bool);
        bool                bind(int, double);
        bool                bind(int, int);
        
        bool                bind(int, int64_t);
        
        //! Binds a TEXT
        //! \note this data MUST be available until unbind() is called (or reset_all)
        bool                bind(int, std::string_view);
        
        std::string_view    column_name(int) const;
        
        bool                v_bool(int) const;
        //! \brief Result value as span
        //! \note COPY this off ASAP if desired to be kept or passed-along
        std::span<const uint8_t>  v_bytes(int) const;
        double              v_double(int) const;
        int                 v_int(int) const;
        int64_t             v_int64(int) const;
        
        //! \brief Result value as string
        //! \note COPY this off ASAP if desired to be kept or passed-along
        std::string_view    v_string(int) const;

        
        struct AutoFinish;
        AutoFinish  af();
    private:
    
        SqlQuery(const SqlQuery&) = delete;
        SqlQuery&   operator=(const SqlQuery&) = delete;
        mutable sqlite3_stmt    *m_stmt = nullptr;
    };

    struct SqlQuery::AutoFinish {
        AutoFinish(AutoFinish&&);
        AutoFinish& operator=(AutoFinish&&);
        ~AutoFinish();
    private:
        friend class SqlQuery;
        SqlQuery*       q;
        AutoFinish(SqlQuery* _q);
        AutoFinish(const AutoFinish&) = delete;
        AutoFinish& operator=(const AutoFinish&) = delete;
    };
}
