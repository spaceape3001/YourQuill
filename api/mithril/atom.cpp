////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "atom/Atom.ipp"
#include "atom/AtomArg.ipp"
#include "atom/AtomCDB.ipp"
#include "atom/AtomHtml.ipp"
#include "atom/AtomPost.ipp"
#include "atom/AtomProvider.ipp"
#include "atom/AtomSpec.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Atom)

namespace yq::mithril {
    template class IDLock<Atom>;
}
