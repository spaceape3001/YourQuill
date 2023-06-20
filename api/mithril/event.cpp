////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "event/EventCDB.ipp"
#include "event/EventHtml.ipp"
#include "event/EventProvider.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Event)

namespace yq::mithril {
    template class IDLock<Event>;
}
