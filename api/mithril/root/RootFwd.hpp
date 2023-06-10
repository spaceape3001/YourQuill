////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/enum/DataRole.hpp>

namespace yq {
    template <typename E, typename V> class EnumMap;
}

namespace yq::mithril {
    struct RootDir;
    using root_map_t            = Map<std::string_view,const RootDir*,IgCase>;
    using root_vector_t         = Vector<const RootDir*>;
    using root_role_map_t       = EnumMap<DataRole, const RootDir*>;
    using root_role_vec_map_t   = EnumMap<DataRole, Vector<const RootDir*>>;
}

