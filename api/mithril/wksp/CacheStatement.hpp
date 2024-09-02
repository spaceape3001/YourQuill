////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/preamble.hpp>
#include <mithril/id/Id.hpp>
#include <yq/container/ByteArray.hpp>
#include <yq/sql/SqlStatement.hpp>
#include <filesystem>
#include <set>

namespace yq::mithril {        
    /*! \brief SQL bound against the cache 
    */
    class CacheStatement : public SqlStatement {
    public:
        CacheStatement();

        template <IdType U>
        U           as()
        {
            return U{u64()};
        }
        
        template <IdType U>
        U           as(uint64_t i)
        {
            return U{u64(i)};
        }

        template <IdType U>
        U           as(uint64_t i, uint64_t j)
        {
            return U{u64(i, j)};
        }

        template <IdType U>
        U           as(uint64_t i, std::string_view j)
        {
            return U{u64(i, j)};
        }


        template <IdType U>
        U           as(std::string_view  i)
        {
            return U{u64(i)};
        }

        template <IdType U>
        U           as(const std::filesystem::path& i)
        {
            return U{u64(i)};
        }

        bool        batch(uint64_t argi, const string_set_t& arg2);
        bool        batch(const string_set_t& arg1, uint64_t arg2);
        bool        batch(uint64_t argi, const std::set<uint64_t>& arg2);
        bool        batch(const std::set<uint64_t>& arg1, uint64_t arg2);

        bool        boolean();
        bool        boolean(uint64_t);
        bool        boolean(uint64_t, uint64_t);
        bool        boolean(uint64_t, std::string_view );
        
        ByteArray   bytes();
        ByteArray   bytes(uint64_t);

        
            // used to execute where id=? style statements
        bool        exec(uint64_t);
            // summarizes step & output check
        bool        exec();
        
            //! Executes two argument statements
        bool        exec(uint64_t arg1, uint64_t arg2);
        bool        exec(uint64_t arg1, std::string_view arg2);
        
        int         integer(uint64_t);
        
        std::filesystem::path   path();
        std::filesystem::path   path(uint64_t);
        
        //  Present verifies that exec() & next() works, so no need to do a value -> boolean conversion
        bool        present();
        bool        present(uint64_t);
        bool        present(uint64_t, uint64_t);
        bool        present(uint64_t, std::string_view );

        template <typename U>
        std::set<U>      set()
        {
            std::set<U>  ret;
            auto _af  = af();
            while(step() == SQResult::Row){
                ret.insert(U{v_uint64(1)});
            }
            return ret;
        }
        
        template <typename U>
        std::set<U>      set(uint64_t i)
        {
            std::set<U>  ret;
            auto _af  = af();
            bind(1, (int64_t) i);
            while(step() == SQResult::Row){
                ret.insert(U{v_uint64(1)});
            }
            return ret;
        }
        
        template <typename U>
        std::set<U>      set(uint64_t i, uint64_t j)
        {
            std::set<U>  ret;
            auto _af  = af();
            bind(1, (int64_t) i);
            bind(2, (int64_t) j);
            while(step() == SQResult::Row){
                ret.insert(U{v_uint64(1)});
            }
            return ret;
        }

        size_t      size();
        size_t      size(uint64_t);
        size_t      size(uint64_t,uint64_t);
        size_t      size(uint64_t,std::string_view );
        size_t      size(std::string_view );

        std::string str();
        std::string str(uint64_t);
        
        uint64_t    u64();
        uint64_t    u64(uint64_t);
        uint64_t    u64(uint64_t,uint64_t);
        uint64_t    u64(uint64_t,std::string_view );
        uint64_t    u64(std::string_view );
        uint64_t    u64(const std::filesystem::path&);
        
        string_set_t   sset();
        string_set_t   sset(uint64_t);
        
        template <IdType U>
        std::vector<U>   vec()
        {
            std::vector<U>  ret;
            auto _af  = af();
            while(step() == SQResult::Row){
                ret.push_back(U{v_uint64(1)});
            }
            return ret;
        }
        
        template <typename U>
        std::vector<U>   vec(uint64_t i)
        {
            std::vector<U>  ret;
            auto _af  = af();
            bind(1, (int64_t) i);
            while(step() == SQResult::Row){
                ret.push_back(U{v_uint64(1)});
            }
            return ret;
        }
        
        template <IdType U>
        std::vector<U>   vec(uint64_t i, uint64_t j)
        {
            std::vector<U>  ret;
            auto _af  = af();
            bind(1, (int64_t) i);
            bind(2, (int64_t) j);
            while(step() == SQResult::Row){
                ret.push_back(U{v_uint64(1)});
            }
            return ret;
        }

        template <IdType U>
        std::vector<U>   vec(uint64_t i, std::string_view  j)
        {
            std::vector<U>  ret;
            auto _af  = af();
            bind(1, (int64_t) i);
            bind(2, j);
            while(step() == SQResult::Row){
                ret.push_back(U{v_uint64(1)});
            }
            return ret;
        }

        template <IdType U>
        std::vector<U>   vec(std::string_view  i)
        {
            std::vector<U>  ret;
            auto _af  = af();
            bind(1, i);
            while(step() == SQResult::Row){
                ret.push_back(U{v_uint64(1)});
            }
            return ret;
        }
    };
}

