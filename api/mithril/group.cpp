////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include "group/GroupArg.ipp"
#include "group/GroupCDB.ipp"
#include "group/GroupHtml.ipp"
//#include "group/GroupPost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Group)

namespace yq::mithril {
    template class IDLock<Group>;
}
