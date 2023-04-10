////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {

    template <typename F, typename T>
    struct Connection {
        F           from;
        T           to;
        constexpr auto operator<=>(const Connection&) const = default;
    };
}
