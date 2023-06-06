////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "tag/Tag.ipp"
#include "tag/TagArg.ipp"
#include "tag/TagCDB.ipp"
#include "tag/TagData.ipp"
#include "tag/TagFile.ipp"
#include "tag/TagHtml.ipp"
#include "tag/TagPost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Tag)

namespace yq::mithril {
    template class IDLock<Tag>;
}
