#pragma once
#include <yq/enum/Format.hpp>

namespace yq {

    struct LeafData {
        struct Context;
        KVTree                  attributes;
        std::vector<Context>    context;
    };

    struct Context {
        std::string     title, icon, data;
        Format          format;
        int             order = 0;
        
        bool operator==(const LeafContext&) const == default;
    };

}
