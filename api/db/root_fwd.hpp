////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "enum_data_role.hpp"

namespace yq {

    struct Root;
    template <typename E, typename V> class EnumMap;
    using root_map_t            = Map<std::string_view,const Root*,IgCase>;
    using root_vector_t         = Vector<const Root*>;
    using root_role_map_t       = EnumMap<DataRole, const Root*>;
    using root_role_vec_map_t   = EnumMap<DataRole, Vector<const Root*>>;
    
    

}
