////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/CacheFwd.hpp>
#include <db/Image.hpp>


namespace yq {

    namespace cdb {
        struct NKI {
            String      key, name;
            Image       icon;
        };
    }
}
