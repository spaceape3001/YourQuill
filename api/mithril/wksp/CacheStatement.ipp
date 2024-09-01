////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/container/Set.hpp>
#include <0/basic/TextUtils.hpp>
#include <mithril/wksp/CacheStatement.hpp>
#include <mithril/wksp/Workspace.hpp>

namespace yq::mithril {        
    CacheStatement::CacheStatement()
    {
        _database(wksp::db());
    }

    bool        CacheStatement::batch(uint64_t i, const string_set_t& arg2)
    {
        bool ret    = true;
        for(const std::string& j : arg2){
            bind(1, i);
            bind(2, j);
            ret = exec() && ret;
        }
        return ret;
    }
    
    bool        CacheStatement::batch(const string_set_t& arg1, uint64_t j)
    {
        bool    ret = true;
        for(const std::string&i : arg1){
            bind(1, i);
            bind(2, j);
            ret = exec() && ret;
        }
        return ret;
    }

    bool        CacheStatement::batch(uint64_t i, const std::set<uint64_t>& arg2)
    {
        bool ret    = true;
        for(uint64_t j : arg2){
            bind(1, i);
            bind(2, j);
            ret = exec() && ret;
        }
        return ret;
    }
    
    bool        CacheStatement::batch(const std::set<uint64_t>& arg1, uint64_t j)
    {
        bool    ret = true;
        for(uint64_t i : arg1){
            bind(1, i);
            bind(2, j);
            ret = exec() && ret;
        }
        return ret;
    }

    bool        CacheStatement::boolean()
    {
        auto _af = af();
        if(step() == SQResult::Row)
            return v_bool(1);
        return false;
    }
    
    bool        CacheStatement::boolean(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        if(step() == SQResult::Row)
            return v_bool(1);
        return false;
    }

    bool        CacheStatement::boolean(uint64_t i, uint64_t j)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        bind(2, (int64_t) j);
        if(step() == SQResult::Row)
            return v_bool(1);
        return false;
    }

    bool        CacheStatement::boolean(uint64_t i, std::string_view j)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        bind(2, j);
        if(step() == SQResult::Row)
            return v_bool(1);
        return false;
    }

    ByteArray   CacheStatement::bytes()
    {
        auto _af = af();
        if(step() == SQResult::Row)
            return v_bytes(1);
        return ByteArray();
    }
    
    ByteArray   CacheStatement::bytes(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        if(step() == SQResult::Row)
            return v_bytes(1);
        return ByteArray();
    }

    bool        CacheStatement::exec()
    {
        auto _af = af();
        return is_good(step());
    }

    bool        CacheStatement::exec(uint64_t i)
    {
        bind(1, (int64_t) i);
        return exec();
    }

    bool        CacheStatement::exec(uint64_t i, uint64_t j)
    {
        bind(1, i);
        bind(2, j);
        return exec();
    }

    bool        CacheStatement::exec(uint64_t i, std::string_view j)
    {
        bind(1, i);
        bind(2, j);
        return exec();
    }
        
    int         CacheStatement::integer(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        if(step() == SQResult::Row)
            return v_int(1);
        return 0;
    }

    std::filesystem::path   CacheStatement::path()
    {
        auto _af = af();
        if(step() == SQResult::Row)
            return std::filesystem::path(v_text(1));
        return std::filesystem::path();
    }
    
    std::filesystem::path   CacheStatement::path(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        if(step() == SQResult::Row)
            return std::filesystem::path(v_text(1));
        return std::filesystem::path();
    }

    bool        CacheStatement::present()
    {
        auto _af = af();
        return step() == SQResult::Row;
    }
    
    bool        CacheStatement::present(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        return step() == SQResult::Row;
    }
    
    bool        CacheStatement::present(uint64_t i, uint64_t j)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        bind(2, (int64_t) j);
        return step() == SQResult::Row;
    }
    

    bool        CacheStatement::present(uint64_t i, std::string_view j)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        bind(2, j);
        return step() == SQResult::Row;
    }

    size_t      CacheStatement::size()
    {
        return (size_t) u64();
    }
    
    size_t      CacheStatement::size(uint64_t i)
    {
        return (size_t) u64(i);
    }

    size_t      CacheStatement::size(uint64_t i, uint64_t j)
    {
        return (size_t) u64(i, j);
    }

    size_t      CacheStatement::size(std::string_view i)
    {
        return (size_t) u64(i);
    }

    size_t      CacheStatement::size(uint64_t i,std::string_view ck)
    {
        return (size_t) u64(i, ck);
    }

    string_set_t  CacheStatement::sset()
    {
        string_set_t  ret;
        auto _af = af();
        while(step() == SQResult::Row){
            ret.insert(copy(v_text(1)));
        }
        return ret;
    }
    
    string_set_t  CacheStatement::sset(uint64_t i)
    {
        StringSet  ret;
        auto _af = af();
        bind(1, (int64_t) i);
        while(step() == SQResult::Row){
            ret.insert(copy(v_text(1)));
        }
        return ret;
    }

    std::string     CacheStatement::str()
    {
        auto _af = af();
        if(step() == SQResult::Row)
            return copy(v_text(1));
        return std::string();
    }
    
    std::string     CacheStatement::str(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        if(step() == SQResult::Row)
            return copy(v_text(1));
        return std::string();
    }
    
    
    uint64_t    CacheStatement::u64()
    {
        auto _af = af();
        if(step() == SQResult::Row)
            return v_uint64(1);
        return 0ULL;
    }

    uint64_t    CacheStatement::u64(uint64_t i)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        if(step() == SQResult::Row)
            return v_uint64(1);
        return 0ULL;
    }

    uint64_t    CacheStatement::u64(uint64_t i, uint64_t j)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        bind(2, (int64_t) j);
        if(step() == SQResult::Row)
            return (uint64_t) v_int64(1);
        return 0ULL;
    }

    uint64_t    CacheStatement::u64(uint64_t i, std::string_view j)
    {
        auto _af = af();
        bind(1, (int64_t) i);
        bind(2, j);
        if(step() == SQResult::Row)
            return v_uint64(1);
        return 0ULL;
    }

    uint64_t    CacheStatement::u64(std::string_view i)
    {
        auto _af = af();
        bind(1, i);
        if(step() == SQResult::Row)
            return v_uint64(1);
        return 0ULL;
    }


    uint64_t    CacheStatement::u64(const std::filesystem::path&i)
    {
        auto _af = af();
        bind(1, i);
        if(step() == SQResult::Row)
            return v_uint64(1);
        return 0ULL;
    }
}
