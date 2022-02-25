////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <util/preamble.hpp>
#include <util/text/Utils.hpp>

namespace yq {
    namespace thread {
        unsigned int id()
        {
            static std::atomic<unsigned int>    sNext{0};
            static thread_local unsigned int    sRet    = sNext++;
            return sRet;
        }
        
        static unsigned int kMainThread     = id();
    }

    const char*     build_directory()
    {
        return YQ_BUILD_ROOT;
    }

    const char*     share_directory()
    {
        return YQ_SHARE_DIR;
    }

    bool is_main_thread()
    {
        return thread::id() == 0;
    }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  IGNORE CASE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        bool    IgCase::operator()(const std::string_view&a, const std::string_view&b) const
        {
            return is_less_igCase(a,b);
        }



        bool    RevIgCase::operator()(const std::string_view&a, const std::string_view&b) const
        {
            return is_greater_igCase(a,b);
        }

}
