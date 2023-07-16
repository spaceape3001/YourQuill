////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uLeaf.hpp"
#include <mithril/leaf/LeafCDB.hpp>

namespace yq::mithril::update {
    ULeaf&  ULeaf::get(Leaf x)
    {
        return lookup<ULeaf>(x);
    }

    ULeaf::ULeaf(Leaf x) : U<Leaf>(x, cdb::key(x)), doc(cdb::document(x))
    {
    }
}

