////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <map>
#include <vector>
#include <0/basic/IgCase.hpp>

namespace yq::b3 {
    struct B3Command;
    struct B3Line;
    
    using arg_map_t         = std::map<std::string,std::string,IgCase>;
    using arg_vector_t      = std::vector<std::string>;
    using b3line_vector_t   = std::vector<B3Line>;
    using b3cmd_vector_t    = std::vector<B3Command>;
}
