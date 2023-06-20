////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "category/CategoryArg.ipp"
#include "category/CategoryCDB.ipp"
#include "category/CategoryData.ipp"
#include "category/CategoryFile.ipp"
#include "category/CategoryHtml.ipp"
#include "category/CategoryPost.ipp"
#include "category/CategoryProvider.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Category)

namespace yq::mithril {
    template class IDLock<Category>;
}
