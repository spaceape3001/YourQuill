#pragma once

#include <util/Set.hpp>
#include <util/SqlQuery.hpp>
#include <util/Vector.hpp>

namespace cdb {

    /*! \brief SQL query class geared for simple selects
    
        This is a query class to encapsulate all of the boiler plate code when dealing with cache objects
        and the typical SELECT query.  More complicated queries can use the constructor, but will be advised
        to hand-jam until support is added.
    */
    class SQ : public SqlQuery {
    public:
        SQ(const QString&);
        
        template <typename U>
        U           as()
        {
            return U{u64()};
        }
        
        template <typename U>
        U           as(uint64_t i)
        {
            return U{u64(i)};
        }

        template <typename U>
        U           as(uint64_t i, uint64_t j)
        {
            return U{u64(i, j)};
        }

        template <typename U>
        U           as(uint64_t i, const QString& j)
        {
            return U{u64(i, j)};
        }

        template <typename U>
        U           as(const QString& i)
        {
            return U{u64(i)};
        }

        bool        boolean();
        bool        boolean(uint64_t);
        bool        boolean(uint64_t, uint64_t);
        bool        boolean(uint64_t, const QString&);
        
        //  Present verifies that exec() & next() works, so no need to do a value -> boolean conversion
        bool        present();
        bool        present(uint64_t);
        bool        present(uint64_t, uint64_t);
        bool        present(uint64_t, const QString&);

        template <typename U>
        Set<U>      set()
        {
            Set<U>  ret;
            auto _af  = af();
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }
        
        template <typename U>
        Set<U>      set(uint64_t i)
        {
            Set<U>  ret;
            auto _af  = af();
            bind(0, i);
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }
        
        template <typename U>
        Set<U>      set(uint64_t i, uint64_t j)
        {
            Set<U>  ret;
            auto _af  = af();
            bind(0, i);
            bind(0, j);
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }

        size_t      size();
        size_t      size(uint64_t);
        size_t      size(uint64_t,uint64_t);
        size_t      size(uint64_t,const QString&);
        size_t      size(const QString&);

        QString     str();
        QString     str(uint64_t);
        
        uint64_t    u64();
        uint64_t    u64(uint64_t);
        uint64_t    u64(uint64_t,uint64_t);
        uint64_t    u64(uint64_t,const QString&);
        uint64_t    u64(const QString&);
        
        QStringSet  sset();
        QStringSet  sset(uint64_t);
        
        template <typename U>
        Vector<U>   vec()
        {
            Vector<U>  ret;
            auto _af  = af();
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }
        
        template <typename U>
        Vector<U>   vec(uint64_t i)
        {
            Vector<U>  ret;
            auto _af  = af();
            bind(0, i);
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }
        
        template <typename U>
        Vector<U>   vec(uint64_t i, uint64_t j)
        {
            Vector<U>  ret;
            auto _af  = af();
            bind(0, i);
            bind(1, j);
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }

        template <typename U>
        Vector<U>   vec(uint64_t i, const QString& j)
        {
            Vector<U>  ret;
            auto _af  = af();
            bind(0, i);
            bind(1, j);
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }

        template <typename U>
        Vector<U>   vec(const QString& i)
        {
            Vector<U>  ret;
            auto _af  = af();
            bind(0, i);
            if(exec()){
                while(next())
                    ret << U{valueU64(0)};
            }
            return ret;
        }
    };
}
