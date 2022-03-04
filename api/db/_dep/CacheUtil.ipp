////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {

    namespace cdb {
        SQ::SQ(const QString& stmt) : SqlQuery(wksp::cache(), stmt)
        {
        }

        bool        SQ::boolean()
        {
            auto _af = af();
            if(exec() && next())
                return valueAs<bool>(0);
            return false;
        }
        
        bool        SQ::boolean(uint64_t i)
        {
            auto _af = af();
            bind(0,i);
            if(exec() && next())
                return valueAs<bool>(0);
            return false;
        }

        bool        SQ::boolean(uint64_t i, uint64_t j)
        {
            auto _af = af();
            bind(0,i);
            bind(1,j);
            if(exec() && next())
                return valueAs<bool>(0);
            return false;
        }

        bool        SQ::boolean(uint64_t i, const QString& j)
        {
            auto _af = af();
            bind(0,i);
            bind(1,j);
            if(exec() && next())
                return valueAs<bool>(0);
            return false;
        }

        bool        SQ::boolean(uint64_t i, const String& j)
        {
            auto _af = af();
            bind(0,i);
            bind(1,j);
            if(exec() && next())
                return valueAs<bool>(0);
            return false;
        }

        std::filesystem::path   SQ::path()
        {
            auto _af = af();
            if(exec() && next())
                return valueString(0).toStdString();
            return std::filesystem::path();
        }
        
        std::filesystem::path   SQ::path(uint64_t i)
        {
            auto _af = af();
            bind(0, i);
            if(exec() && next())
                return valueString(0).toStdString();
            return std::filesystem::path();
        }

        bool        SQ::present()
        {
            auto _af = af();
            return exec() && next();
        }
        
        bool        SQ::present(uint64_t i)
        {
            auto _af = af();
            bind(0, i);
            return exec() && next();
        }
        
        bool        SQ::present(uint64_t i, uint64_t j)
        {
            auto _af = af();
            bind(0, i);
            bind(1, j);
            return exec() && next();
        }
        
        
        bool        SQ::present(uint64_t i, const QString& j)
        {
            auto _af = af();
            bind(0, i);
            bind(1, j);
            return exec() && next();
        }

        bool        SQ::present(uint64_t i, const String& j)
        {
            auto _af = af();
            bind(0, i);
            bind(1, j);
            return exec() && next();
        }

        size_t      SQ::size()
        {
            return (size_t) u64();
        }
        
        size_t      SQ::size(uint64_t i)
        {
            return (size_t) u64(i);
        }

        size_t      SQ::size(uint64_t i, uint64_t j)
        {
            return (size_t) u64(i, j);
        }

        size_t      SQ::size(const QString& i)
        {
            return (size_t) u64(i);
        }

        size_t      SQ::size(const String& i)
        {
            return (size_t) u64(i);
        }

        StringSet  SQ::sset()
        {
            StringSet  ret;
            auto _af = af();
            if(exec()){
                while(next())
                    ret << valueString(0);
            }
            return ret;
        }
        
        StringSet  SQ::sset(uint64_t i)
        {
            StringSet  ret;
            auto _af = af();
            bind(0, i);
            if(exec()){
                while(next())
                    ret << valueString(0);
            }
            return ret;
        }

        String     SQ::str()
        {
            auto _af = af();
            if(exec() && next())
                return valueString(0);
            return String();
        }
        
        String     SQ::str(uint64_t i)
        {
            auto _af = af();
            bind(0,i);
            if(exec() && next())
                return valueString(0);
            return String();
        }
        
        
        uint64_t    SQ::u64()
        {
            auto _af = af();
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }
        
        uint64_t    SQ::u64(uint64_t i)
        {
            auto _af = af();
            bind(0, i);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }

        uint64_t    SQ::u64(uint64_t i, uint64_t j)
        {
            auto _af = af();
            bind(0, i);
            bind(1, j);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }

        uint64_t    SQ::u64(uint64_t i, const QString& j)
        {
            auto _af = af();
            bind(0, i);
            bind(1, j);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }

        uint64_t    SQ::u64(uint64_t i, const String& j)
        {
            auto _af = af();
            bind(0, i);
            bind(1, j);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }

        uint64_t    SQ::u64(const QString& i)
        {
            auto _af = af();
            bind(0, i);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }

        uint64_t    SQ::u64(const String& i)
        {
            auto _af = af();
            bind(0, i);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }

        uint64_t    SQ::u64(const std::filesystem::path&i)
        {
            auto _af = af();
            bind(0, i);
            if(exec() && next())
                return valueU64(0);
            return 0ULL;
        }
    }
}
