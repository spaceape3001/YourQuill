////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "fragment/FragmentArg.ipp"
#include "fragment/FragmentCDB.ipp"
#include "fragment/FragmentHtml.ipp"
#include "fragment/FragmentPost.ipp"
#include "fragment/FragmentProvider.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Fragment)

namespace yq::mithril {
    template class IDLock<Fragment>;
}

