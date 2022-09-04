////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <string>
#include <kernel/db/CacheFwd.hpp>
#include <kernel/db/SQ.hpp>
#include <math/Count16.hpp>

/*
    The UPDATE is here because this analysis can suck in a database, therefore the updater needs a parallel
    set of structures.   Note the idea is to keep this in-memory data LIMITED to what's needed, nothing more.
*/

namespace yq {
    using namespace cdb;
    
    template <typename T>
    struct URefSet {
        std::set<T>             defined;
        std::map<T,Count16>     all;
    };
    
    template <typename T>
    struct UDefResolved {
        T   defined;
        T   resolved;
    };
    
    template <typename F, typename T>
    struct UConnection {
        F           from;
        T           to;
        constexpr auto operator<=>(const UConnection&) const = default;
    };
    
    using UClassCxn = UConnection<Class, Class>;
}
