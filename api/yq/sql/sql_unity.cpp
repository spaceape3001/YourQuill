////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SqlLite.hpp"
#include "SqlLogging.hpp"
#include "SqlQuery.hpp"
#include <sqlite3.h>
#include <yq/collection/Set.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/text/Utils.hpp>

namespace yq {

    namespace {
        bool    configUp()
        {
            sqlite3_config(SQLITE_CONFIG_MULTITHREAD );
            return true;
        }
    }

    struct SqlError {
        int     result = SQLITE_OK;
    };
    
    log4cpp::CategoryStream&    operator<<(log4cpp::CategoryStream& str, SqlError ec)
    {
        const char* why = sqlite3_errstr(ec.result);
        if(!why)
            why = "Unknown Reason";
        str << why;
        return str;
    }

    SqlLite::SqlLite() : m_database(nullptr) 
    {
    }
    
    SqlLite::~SqlLite()
    {
        close();
    }
    
    void        SqlLite::close()
    {
        if(m_database != nullptr){
            sqlite3_close_v2(m_database);
            m_database  = nullptr;
        }
    }

    bool        SqlLite::has_table(std::string_view s) const
    {
        SqlQuery    sql((SqlLite&) *this, "SELECT COUNT(1) FROM sqlite_master WHERE type='table' AND name=?");
        sql.bind(1, s);
        if(sql.step() == SqlQuery::Row){
            return sql.v_bool(1);
        } else
            return false;
    }

    bool        SqlLite::is_open() const
    {
        return m_database != nullptr;
    }
    
    bool        SqlLite::open(const std::filesystem::path& file, int flags)
    {
        if(is_open())
            return false;
        if(file.empty())
            return false;
            
        static bool fConfig = configUp();
        if(!fConfig)
            return false;
            
        int     r;
        if(flags){
            r = sqlite3_open_v2(file.c_str(), &m_database, flags, nullptr);
        } else {
            r = sqlite3_open(file.c_str(), &m_database);
        }
        
        if(r != SQLITE_OK){
            dbError << "Database (" << file << ") failed to open: " << SqlError(r);
            m_database = nullptr;
            return false;
        }
        m_file  = file;
        return true;
    }
    
    namespace {
        int     string_set_callback(void* ret, int, char**argv, char**)  
        {
            (*(string_set_t*) ret) << argv[0];
            return 0;
        }
    }
    
    string_set_t    SqlLite::tables() const
    {
        string_set_t    ret;
        if(m_database){
            char* zErrMsg   = nullptr;
            if(sqlite3_exec(m_database, "SELECT name FROM sqlite_master WHERE type='table'", 
                string_set_callback, &ret, &zErrMsg) != SQLITE_OK
            ){
                dbError << "SqlLite::tables() : " << zErrMsg;
                sqlite3_free(zErrMsg);
            }
        }
        return ret;
    }
    
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool SqlQuery::exec(SqlLite&db, const std::string& sql)
    {
        char* zErrMsg   = nullptr;
        if( sqlite3_exec(db.db(), sql.c_str(), nullptr, nullptr, &zErrMsg) != SQLITE_OK){
            dbError << "SqlQuery::exec(" << sql << "): " << zErrMsg;
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }


    SqlQuery::SqlQuery(SqlLite&db, std::string_view sql, bool isPersistent) : m_stmt(nullptr)
    {
        if(!db.db()){
            dbError  << "SqlQuery(" << sql << "): Database is CLOSED!";
            return;
        }
    
        int flags = 0;
        if(isPersistent)
            flags |= SQLITE_PREPARE_PERSISTENT;
        int r = sqlite3_prepare_v3(db.db(), sql.data(), sql.size(), flags, &m_stmt, nullptr);
        if(r!= SQLITE_OK){
            dbError << "SqlQuery(" << sql << "): " << SqlError(r);
            if(m_stmt){
                sqlite3_finalize(m_stmt);
                m_stmt = nullptr;
            }
        }
    }
    
    SqlQuery::SqlQuery(SqlQuery&&mv) : m_stmt(mv.m_stmt)
    {
        mv.m_stmt    = nullptr;
    }
    
    SqlQuery::~SqlQuery()
    {
        if(m_stmt){
            sqlite3_finalize(m_stmt);
            m_stmt = nullptr;
        }
    }
    
    SqlQuery&   SqlQuery::operator=(SqlQuery&&mv)
    {
        if(this != &mv){
            if(m_stmt)
                sqlite3_finalize(m_stmt);
            m_stmt  = mv.m_stmt;
            mv.m_stmt = nullptr;
        }
        return *this;
    }
    
    
    SqlQuery::AutoFinish  SqlQuery::af()
    {
        return AutoFinish(this);
    }

    bool  SqlQuery::bind(int c)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_null(m_stmt, c);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    bool  SqlQuery::bind(int c, std::span<uint8_t> v)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_blob(m_stmt, c, v.data(), v.size(), SQLITE_STATIC);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    bool  SqlQuery::bind(int c, bool v)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_int(m_stmt, c, v ? 1 : 0);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    bool  SqlQuery::bind(int c, double v)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_double(m_stmt, c, v);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    bool  SqlQuery::bind(int c, int v)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_int(m_stmt, c, v);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    bool  SqlQuery::bind(int c, int64_t v)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_int64(m_stmt, c, v);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    bool  SqlQuery::bind(int c, std::string_view v)
    {
        if(!m_stmt){
            dbError << "SqlQuery::bind(" << c << "): Not properly prepared!";
            return false;
        }
        if(c<1){
            dbError << "SqlQuery::bind(" << c << "): Bad Column Number!";
            return false;
        }
        
        int r = sqlite3_bind_text(m_stmt, c, v.data(), v.size(), SQLITE_STATIC);
        if(r != SQLITE_OK){
            dbError << "SqlQuery::bind(" << c << "): " << SqlError(r);
            return false;
        }
        return true;
    }
    
    int                 SqlQuery::column_count() const
    {
        if(!m_stmt)
            return 0;
        return sqlite3_column_count(m_stmt);
    }
    

    std::string_view    SqlQuery::column_name(int i) const
    {
        if(!m_stmt)
            return std::string_view();
        const char* zname = sqlite3_column_name(m_stmt, i-1);
        if(zname)
            return std::string_view(zname);
        return std::string_view();
    }

    void  SqlQuery::reset()
    {
        if(m_stmt){
            sqlite3_reset(m_stmt);
            sqlite3_clear_bindings(m_stmt);
        }
    }


    std::string_view    SqlQuery::sql() const
    {
        if(m_stmt){
            return sqlite3_sql(m_stmt);
        } else
            return std::string_view();
    }

    SqlQuery::Result SqlQuery::step()
    {
        if(!m_stmt){
            dbError <<"SqlQuery::step(): Calling on an uninitalized statement!";
            return Error;
        }
        
        int r   = sqlite3_step(m_stmt);
        switch(r){
        case SQLITE_OK:
        case SQLITE_DONE:
            return Done;
        case SQLITE_ROW:
            return Row;
        case SQLITE_BUSY:
            dbError << "SqlQuery::step(): BUSY, try again later.";
            return Busy;
        default:
            dbError << "SqlQuery::step(): " << SqlError{r};
            return Error;
        }
    }

    bool                SqlQuery::v_bool(int c) const
    {
        if(!m_stmt)
            return false;
        return sqlite3_column_int(m_stmt, c-1) ? true : false;
    }
    
    std::span<const uint8_t>       SqlQuery::v_bytes(int c) const
    {
        if(!m_stmt)
            return {};
        unsigned char*  data = (uint8_t*) sqlite3_column_blob(m_stmt, c-1);
        size_t          sz  = sqlite3_column_bytes(m_stmt, c-1);
        return std::span<const uint8_t>( data, sz );
    }
    
    double              SqlQuery::v_double(int c) const
    {
        if(!m_stmt)
            return 0.0;
        return sqlite3_column_double(m_stmt, c-1);
    }
    
    int                 SqlQuery::v_int(int c) const
    {
        if(!m_stmt)
            return 0;
        return sqlite3_column_int(m_stmt, c-1);
    }
    
    int64_t             SqlQuery::v_int64(int c) const
    {
        if(!m_stmt)
            return 0;
        return sqlite3_column_int64(m_stmt, c-1);
    }
    
    std::string_view    SqlQuery::v_text(int c) const
    {
        if(!m_stmt)
            return std::string_view();
        const char* data    = (const char*) sqlite3_column_text(m_stmt, c-1);
        if(!data)
            return std::string_view();
        int  sz = sqlite3_column_bytes(m_stmt, c-1);
        if(data && (sz>0) && !data[sz-1]) // delete final NULL
            --sz;
        return std::string_view(data, sz);
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    SqlQuery::AutoFinish::AutoFinish(SqlQuery* _q) : q(_q)
    {
    }
    
    SqlQuery::AutoFinish::AutoFinish(AutoFinish&&mv) : q(mv.q)
    {
        mv.q    = nullptr;
    }
    
    SqlQuery::AutoFinish& SqlQuery::AutoFinish::operator=(AutoFinish&&mv)
    {
        if(this != &mv){
            if(q)
                q -> reset();
            q       = mv.q;
            mv.q    = nullptr;
        }
        return *this;
    }
    
    SqlQuery::AutoFinish::~AutoFinish()
    {
        if(q){
            q -> reset();
            q   = nullptr;
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    
    bool    _runscript(std::string_view script, SqlLite& db)
    {
        std::string         buffer;
        buffer.reserve(script.size());
    
        vsplit(script, '\n', [&](std::string_view l){
            l = trimmed(l);
            if(is_blank(l))
                return;
            if(starts(l, "--")){
                return ;
            }
            buffer += l;
            buffer += ' ';
        });
        
        for(std::string_view sql : split(buffer, ';')){
            std::string s = copy(trimmed(sql));
            if(!SqlQuery::exec(db, s))
                return false;
        }
        return true;
    }
    
    bool    db_run_script(std::string_view script, SqlLite& db)
    {
        return _runscript(script, db);
    }
    
    bool    db_run_script_file(const std::filesystem::path& file, SqlLite& db)
    {
        dbInfo << "db_script_file " << file;
        if(!std::filesystem::exists(file)){
            dbError << "db_run_script_file(" << file << "): does not exist!";
            return false;
        }
        
        std::string     text    = file_string(file);
        if(text.empty())
            return true;
        return _runscript(text, db);
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
}
