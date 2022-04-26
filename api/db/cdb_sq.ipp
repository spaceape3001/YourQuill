////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "cdb_sq.hpp"
#include "workspace.hpp"
#include <yq/collection/Set.hpp>
#include <yq/text/text_utils.hpp>

namespace yq {

    namespace cdb {
        SQ::SQ(const std::string_view stmt) : SqlQuery(wksp::db(), stmt)
        {
        }

        bool        SQ::boolean()
        {
            auto _af = af();
            if(step() == Row)
                return v_bool(1);
            return false;
        }
        
        bool        SQ::boolean(uint64_t i)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            if(step() == Row)
                return v_bool(1);
            return false;
        }

        bool        SQ::boolean(uint64_t i, uint64_t j)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            bind(2, (int64_t) j);
            if(step() == Row)
                return v_bool(1);
            return false;
        }

        bool        SQ::boolean(uint64_t i, std::string_view j)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            bind(2, j);
            if(step() == Row)
                return v_bool(1);
            return false;
        }

        bool        SQ::exec()
        {
            auto _af = af();
            return is_good(step());
        }

        bool        SQ::exec(uint64_t i)
        {
            bind(1, (int64_t) i);
            return exec();
        }

        std::filesystem::path   SQ::path()
        {
            auto _af = af();
            if(step() == Row)
                return std::filesystem::path(v_text(1));
            return std::filesystem::path();
        }
        
        std::filesystem::path   SQ::path(uint64_t i)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            if(step() == Row)
                return std::filesystem::path(v_text(1));
            return std::filesystem::path();
        }

        bool        SQ::present()
        {
            auto _af = af();
            return step() == Row;
        }
        
        bool        SQ::present(uint64_t i)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            return step() == Row;
        }
        
        bool        SQ::present(uint64_t i, uint64_t j)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            bind(2, (int64_t) j);
            return step() == Row;
        }
        

        bool        SQ::present(uint64_t i, std::string_view j)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            bind(2, j);
            return step() == Row;
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

        size_t      SQ::size(std::string_view i)
        {
            return (size_t) u64(i);
        }

        string_set_t  SQ::sset()
        {
            string_set_t  ret;
            auto _af = af();
            while(step() == Row){
                ret << copy(v_text(1));
            }
            return ret;
        }
        
        string_set_t  SQ::sset(uint64_t i)
        {
            StringSet  ret;
            auto _af = af();
            bind(1, (int64_t) i);
            while(step() == Row){
                ret << copy(v_text(1));
            }
            return ret;
        }

        std::string     SQ::str()
        {
            auto _af = af();
            if(step() == Row)
                return copy(v_text(1));
            return std::string();
        }
        
        std::string     SQ::str(uint64_t i)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            if(step() == Row)
                return copy(v_text(1));
            return std::string();
        }
        
        
        uint64_t    SQ::u64()
        {
            auto _af = af();
            if(step() == Row)
                return v_uint64(1);
            return 0ULL;
        }
        
        uint64_t    SQ::u64(uint64_t i)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            if(step() == Row)
                return v_uint64(1);
            return 0ULL;
        }

        uint64_t    SQ::u64(uint64_t i, uint64_t j)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            bind(2, (int64_t) j);
            if(step() == Row)
                return (uint64_t) v_int64(1);
            return 0ULL;
        }

        uint64_t    SQ::u64(uint64_t i, std::string_view j)
        {
            auto _af = af();
            bind(1, (int64_t) i);
            bind(2, j);
            if(step() == Row)
                return v_uint64(1);
            return 0ULL;
        }

        uint64_t    SQ::u64(std::string_view i)
        {
            auto _af = af();
            bind(1, i);
            if(step() == Row)
                return v_uint64(1);
            return 0ULL;
        }


        uint64_t    SQ::u64(const std::filesystem::path&i)
        {
            auto _af = af();
            bind(1, i);
            if(step() == Row)
                return v_uint64(1);
            return 0ULL;
        }
    }
}
