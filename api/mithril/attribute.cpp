////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "attribute/AttributeArg.ipp"
#include "attribute/AttributeCDB.ipp"
#include "attribute/AttributeHtml.ipp"
#include "attribute/AttributeDiff.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Attribute)

namespace yq::mithril {
    template class IDLock<Attribute>;
}
