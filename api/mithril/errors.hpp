////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/basic/ErrorDB.hpp>
#include <yq/basic/errors.hpp>

namespace yq::errors {
    using bad_copyright_stance  = error_db::entry<"Bad copyright stance">;
    using bad_copyright_from    = error_db::entry<"Bad copyright 'from' value">;
    using bad_copyright_to      = error_db::entry<"Bad copyright 'to' value">;
}
