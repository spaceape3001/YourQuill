////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "directory/DirectoryArg.ipp"
#include "directory/DirectoryCDB.ipp"
#include "directory/DirectoryHtml.ipp"
#include "directory/DirectoryPost.ipp"
#include "directory/DirectoryProvider.ipp"

#include "db/IDLock.ipp"

#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Directory)

namespace yq::mithril {
    template class IDLock<Directory>;
}
