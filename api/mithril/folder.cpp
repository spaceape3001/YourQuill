////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "folder/FolderArg.ipp"
#include "folder/FolderCDB.ipp"
#include "folder/FolderHtml.ipp"
#include "folder/FolderPost.ipp"
#include "folder/FolderProvider.ipp"

#include "db/IDLock.ipp"
#include <meta/Init.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Folder)

namespace yq::mithril {
    template class IDLock<Folder>;
}
