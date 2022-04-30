////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/log/Logging.hpp>
#include <yq/sql/SqlQuery.hpp>

#include <set>
#include <vector>
#include <filesystem>

#define cdbAlert           yAlert("cdb")
#define cdbCritical        yCritical("cdb")
#define cdbDebug           yDebug("cdb")
#define cdbError           yError("cdb")
#define cdbEmergency       yEmergency("cdb")
#define cdbFatal           yFatal("cdb")
#define cdbInfo            yInfo("cdb")
#define cdbNotice          yNotice("cdb")
#define cdbWarning         yWarning("cdb")

namespace yq {

    namespace cdb {

        /*! \brief SQL query class geared for simple selects
        
            This is a query class to encapsulate all of the boiler plate code when dealing with cache objects
            and the typical SELECT query.  More complicated queries can use the constructor, but will be advised
            to hand-jam until support is added.
        */
        class SQ : public SqlQuery {
        public:
            SQ(std::string_view);
            
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
            U           as(uint64_t i, std::string_view j)
            {
                return U{u64(i, j)};
            }


            template <typename U>
            U           as(std::string_view  i)
            {
                return U{u64(i)};
            }

            template <typename U>
            U           as(const std::filesystem::path& i)
            {
                return U{u64(i)};
            }

            bool        boolean();
            bool        boolean(uint64_t);
            bool        boolean(uint64_t, uint64_t);
            bool        boolean(uint64_t, std::string_view );
            
                // used to execute where id=? style statements
            bool        exec(uint64_t);
                // summarizes step & output check
            bool        exec();
            
                //! Executes two argument statements
            bool        exec(uint64_t arg1, uint64_t arg2);
            
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
                while(step() == Row){
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
                while(step() == Row){
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
                while(step() == Row){
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
            
            template <typename U>
            std::vector<U>   vec()
            {
                std::vector<U>  ret;
                auto _af  = af();
                while(step() == Row){
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
                while(step() == Row){
                    ret.push_back(U{v_uint64(1)});
                }
                return ret;
            }
            
            template <typename U>
            std::vector<U>   vec(uint64_t i, uint64_t j)
            {
                std::vector<U>  ret;
                auto _af  = af();
                bind(1, (int64_t) i);
                bind(2, (int64_t) j);
                while(step() == Row){
                    ret.push_back(U{v_uint64(1)});
                }
                return ret;
            }

            template <typename U>
            std::vector<U>   vec(uint64_t i, std::string_view  j)
            {
                std::vector<U>  ret;
                auto _af  = af();
                bind(1, (int64_t) i);
                bind(2, j);
                while(step() == Row){
                    ret.push_back(U{v_uint64(1)});
                }
                return ret;
            }

            template <typename U>
            std::vector<U>   vec(std::string_view  i)
            {
                std::vector<U>  ret;
                auto _af  = af();
                bind(1, i);
                while(step() == Row){
                    ret.push_back(U{v_uint64(1)});
                }
                return ret;
            }
        };
    }
}
