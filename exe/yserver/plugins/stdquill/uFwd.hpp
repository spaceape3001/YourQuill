////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <string>
#include <yq/basic/trait/not_copyable.hpp>
#include <yq/basic/trait/not_moveable.hpp>
#include <mithril/db/CacheFwd.hpp>
#include <mithril/db/SQ.hpp>
#include <0/math/Counter.hpp>
#include <mithril/update/DefRes.hpp>
#include <mithril/bit/Connection.hpp>

/*
    The UPDATE is here because this analysis can suck in a database, therefore the updater needs a parallel
    set of structures.   Note the idea is to keep this in-memory data LIMITED to what's needed, nothing more.
*/

namespace yq {
    using namespace cdb;
    
    template <typename T>
    struct URefSet {
        std::set<T>             defined;
        std::map<T,HCountU16>   all;
    };
    
    template <typename T> 
    using UDefResolved  = update::DefRes<T>;
}
