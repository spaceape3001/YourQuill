////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Flags.hpp>

namespace yq::mithril {
    /*! Enumerated list of columns 
    */
    enum class Column {
        None,
        Hidden,
        Id,
        Key,
        Name,
        Path,
        Template,
        Title
    };
    
    enum class ColOpt : uint8_t {
        Set,
        Add,
        Icon
    };
    
    using ColOpts   = Flags<ColOpt>;
}
