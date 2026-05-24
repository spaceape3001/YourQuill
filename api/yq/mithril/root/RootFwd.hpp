////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/enum/DataRole.hpp>
#include <yq/container/Map.hpp>
#include <yq/container/Vector.hpp>
#include <yq/text/IgCase.hpp>

namespace yq {
    template <typename E, typename V> class EnumMap;
}

namespace yq::mithril {
    struct Root;
    struct RootDir;
    using root_map_t            = Map<std::string_view,const RootDir*,IgCase>;
    using root_vector_t         = Vector<const RootDir*>;
    using root_role_map_t       = EnumMap<DataRole, const RootDir*>;
    using root_role_vec_map_t   = EnumMap<DataRole, Vector<const RootDir*>>;
}

