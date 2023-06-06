////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "image/ImageArg.ipp"
#include "image/ImageCDB.ipp"
#include "image/ImageHtml.ipp"
#include "image/ImagePost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Image)

namespace yq::mithril {
    template class IDLock<Image>;
}
