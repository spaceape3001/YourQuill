////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "SqlLite.hpp"
#include "SqlLogging.hpp"
#include <sqlite3.h>

namespace yq {

    namespace {
        bool    configUp()
        {
            sqlite3_config(SQLITE_CONFIG_MULTITHREAD );
            return true;
        }
    }

    SqlLite::SqlLite() : m_database(nullptr) 
    {
    }
    
    SqlLite::~SqlLite()
    {
        close();
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
            r = sqlite3_open(file.c_str(), &m_database);
        } else {
            r = sqlite3_open_v2(file.c_str(), &m_database, flags, nullptr);
        }
        
        if(r != SQLITE_OK){
            const char* why = sqlite3_errstr(r);
            if(!why)
                why = "Unknown reason.";
            dbError << "Database (" << file << ") failed to open: " << why;
            m_database = nullptr;
            return false;
        }
        m_file  = file;
        return true;
    }
    
    bool        SqlLite::is_open() const
    {
        return m_database != nullptr;
    }
    
    void        SqlLite::close()
    {
        if(m_database != nullptr){
            sqlite3_close_v2(m_database);
            m_database  = nullptr;
        }
    }

}
