////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <yq/mithril/id/Id.hpp>

namespace yq::mithril {
    struct Graphviz {
        static constexpr const IdTypeId  ID      = 17;
        static constexpr const IdTypes    PARENTS = {};

        uint64_t  id = 0ULL;
        constexpr auto    operator<=>(const Graphviz& rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
        
        struct Node;
        struct Edge;
        struct Builder;
        
        static std::string      dot2html(std::string_view);
        static std::string      dot2svg(std::string_view);
    };
}

YQ_TYPE_DECLARE(yq::mithril::Graphviz)
YQ_TYPE_DECLARE(yq::mithril::GraphvizSet)
YQ_TYPE_DECLARE(yq::mithril::GraphvizVector)
