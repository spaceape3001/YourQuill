////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


//#include "thing/ThingArg.ipp"
#include "thing/ThingCDB.ipp"
#include "thing/ThingHtml.ipp"
//#include "thing/ThingPost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Thing)

namespace yq::mithril {
    template class IDLock<Thing>;
}
