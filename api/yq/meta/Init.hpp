////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "InfoBinder.hpp"
#include "CompoundInfoDynamic.hpp"
#include "CompoundInfoStatic.hpp"
#include "Global.hpp"
#include "GlobalInfoWriter.hpp"
#include "MetaWriter.hpp"
#include "ObjectInfoWriter.hpp"
#include "PropertyInfoWriter.hpp"
#include "TypeInfoWriter.hpp"

#include <basic/Logging.hpp>


#define YQ_TYPE_IMPLEMENT( ... )                                                                            \
    namespace yq {                                                                                          \
        TypeInfo&   InfoBinder<__VA_ARGS__>::edit() {                                                       \
            static auto* s_ret   = new TypeInfo::Final<__VA_ARGS__>(#__VA_ARGS__);                          \
            return *s_ret;                                                                                  \
        }                                                                                                   \
        template <> TypeInfo& TypeInfo::Final<__VA_ARGS__>::s_save  = InfoBinder<__VA_ARGS__>::edit();      \
    }

#define YQ_TYPE_FIXED( ii, ... )                                                                            \
    namespace yq {                                                                                          \
        TypeInfo&   InfoBinder<__VA_ARGS__>::edit() {                                                       \
            static auto* s_ret   = new TypeInfo::Final<__VA_ARGS__>(#__VA_ARGS__, ii);                      \
            return *s_ret;                                                                                  \
        }                                                                                                   \
        template <> TypeInfo& TypeInfo::Final<__VA_ARGS__>::s_save  = InfoBinder<__VA_ARGS__>::edit();      \
    }
    
#define MO_IMPLEMENT( ... )                                                                                 \
    namespace yq {                                                                                          \
    }

namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  MISC WRAPPER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    template <std::string_view (*FN)()>
    std::string    string_view_proxy()
    {
        return std::string(FN());
    }
}
