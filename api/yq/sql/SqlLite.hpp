////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/preamble.hpp>

struct sqlite3;

namespace yq {
    
    class SqlLite {
    public:
    
        enum {
            ReadOnly        = 0x1,
            ReadWrite       = 0x2,
            Create          = 0x4,
            DeleteOnClose   = 0x8,
            Exclusive       = 0x10,
            AutoProxy       = 0x20,
            Uri             = 0x40,
            Memory          = 0x80,
            MainDB          = 0x100,
            TempDB          = 0x200,
            TransientDB     = 0x400,
            MainJournal     = 0x800,
            TempJournal     = 0x1000,
            SubJournal      = 0x2000,
            SuperJournal    = 0x4000,
            NoMutex         = 0x8000,
            FullMutex       = 0x10000,
            SharedCache     = 0x20000,
            PrivateCache    = 0x40000,
            Wal             = 0x80000,
            NoFollow        = 0x0100000,
            ExRescode       = 0x0200000
        };
    
        SqlLite();
        ~SqlLite();
        bool        open(const std::filesystem::path&, int flags=0);
        sqlite3*    db() const { return m_database; }
        bool        is_open() const;
        void        close();
        const std::filesystem::path&    file() const { return m_file; }
    
    private:
    
        SqlLite(const SqlLite&) = delete;
        SqlLite(SqlLite&&) = delete;
        SqlLite&    operator=(const SqlLite&);
    
        std::filesystem::path   m_file;
        sqlite3*                m_database = nullptr;
    };

}
