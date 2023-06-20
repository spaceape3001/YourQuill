////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {
    struct IdColumn;
    namespace column {
        IdColumn    root_id();
        IdColumn    root_key();
        IdColumn    root_name();
        IdColumn    root_path();
        IdColumn    root_template();
    }
}

