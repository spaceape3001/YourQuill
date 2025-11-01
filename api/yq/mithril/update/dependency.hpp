////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>

namespace yq::mithril::dependency {
    template <IdType T>
    struct Outbound {
        T       x;
        constexpr bool operator==(const Outbound&) const noexcept = default;
        constexpr operator T() const noexcept { return x; }
    };

    template <IdType T>
    struct Inbound {
        T       x;
        constexpr bool operator==(const Inbound&) const noexcept = default;
        constexpr operator T() const noexcept { return x; }
    };

    template <IdType T>
    struct Node {
        T       x;
        constexpr bool operator==(const Node&) const noexcept = default;
        constexpr operator T() const noexcept { return x; }
    };

    template <IdType T>
    struct Edge {
        T       x;
        constexpr bool operator==(const Edge&) const noexcept = default;
        constexpr operator T() const noexcept { return x; }
    };
}
