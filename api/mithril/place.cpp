////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include "place/PlaceArg.ipp"
#include "place/PlaceCDB.ipp"
#include "place/PlaceHtml.ipp"
//#include "place/PlacePost.ipp"
#include "place/PlaceProvider.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Place)

namespace yq::mithril {
    template class IDLock<Place>;
}
