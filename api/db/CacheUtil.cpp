////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CacheUtil.hpp"
#include "Workspace.hpp"

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

    QStringSet  SQ::sset()
    {
        QStringSet  ret;
        auto _af = af();
        if(exec()){
            while(next())
                ret << valueString(0);
        }
        return ret;
    }
    
    QStringSet  SQ::sset(uint64_t i)
    {
        QStringSet  ret;
        auto _af = af();
        bind(0, i);
        if(exec()){
            while(next())
                ret << valueString(0);
        }
        return ret;
    }

    QString     SQ::str()
    {
        auto _af = af();
        if(exec() && next())
            return valueString(0);
        return QString();
    }
    
    QString     SQ::str(uint64_t i)
    {
        auto _af = af();
        bind(0,i);
        if(exec() && next())
            return valueString(0);
        return QString();
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

    uint64_t    SQ::u64(const QString& i)
    {
        auto _af = af();
        bind(0, i);
        if(exec() && next())
            return valueU64(0);
        return 0ULL;
    }
}
