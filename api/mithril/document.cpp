////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "document/DocumentArg.ipp"
#include "document/DocumentCDB.ipp"
#include "document/DocumentHtml.ipp"
#include "document/DocumentPost.ipp"
#include "document/DocumentProvider.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Document)

namespace yq::mithril {
    template class IDLock<Document>;
}
