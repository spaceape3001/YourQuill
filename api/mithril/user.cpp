////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "user/UserArg.ipp"
#include "user/UserCDB.ipp"
#include "user/UserData.ipp"
#include "user/UserFile.ipp"
#include "user/UserHtml.ipp"
#include "user/UserPost.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::User)

namespace yq::mithril {
    template class IDLock<User>;
}
