////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "leaf/LeafArg.ipp"
#include "leaf/LeafData.ipp"
#include "leaf/LeafFile.ipp"
#include "leaf/LeafCDB.ipp"
#include "leaf/LeafHtml.ipp"
#include "leaf/LeafPost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Leaf)

namespace yq::mithril {
    template class IDLock<Leaf>;
}

