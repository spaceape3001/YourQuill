////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq::mithril {
    struct IdColumn;
    namespace column {
        IdColumn    fragment_id();
        IdColumn    fragment_key();
        IdColumn    fragment_name();
        IdColumn    fragment_path();
    }
}

