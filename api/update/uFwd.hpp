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


/*
    The UPDATE is here because this analysis can suck in a database, therefore the updater needs a parallel
    set of structures.   Note the idea is to keep this in-memory data LIMITED to what's needed, nothing more.
*/

namespace yq {
    using namespace cdb;
}
