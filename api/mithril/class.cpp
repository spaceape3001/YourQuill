////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "class/Class.ipp"
#include "class/ClassArg.ipp"
#include "class/ClassCDB.ipp"
#include "class/ClassData.ipp"
#include "class/ClassFile.ipp"
#include "class/ClassHtml.ipp"
#include "class/ClassPost.ipp"
#include "class/ClassProvider.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Class)

namespace yq::mithril {
    template class IDLock<Class>;
}
