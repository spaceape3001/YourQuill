////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

//#include "game/GameArg.ipp"
#include "game/GameCDB.ipp"
#include "game/GameHtml.ipp"
//#include "game/GamePost.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Game)

namespace yq::mithril {
    template class IDLock<Game>;
}
