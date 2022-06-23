////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint.hpp>
#include <unordered_map>

namespace yq {
    namespace engine {

        struct VqObject;

        struct VqObjectFrame {
            std::unordered_map<uint64_t, VqObject*> objects;
        };

    }
}
