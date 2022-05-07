////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Directory.hpp"
#include "DirectoryArg.hpp"
#include "DirectoryDB.hpp"
#include "DirectoryHtml.hpp"

#include "Document.hpp"
#include "DocumentArg.hpp"
#include "DocumentDB.hpp"
#include "DocumentHtml.hpp"

#include "Folder.hpp"
#include "FolderArg.hpp"
#include "FolderDB.hpp"
#include "FolderHtml.hpp"

#include "Fragment.hpp"
#include "FragmentArg.hpp"
#include "FragmentDB.hpp"
#include "FragmentHtml.hpp"

#include "Root.hpp"
#include "RootArg.hpp"
#include "RootDB.hpp"
#include "RootHtml.hpp"

#include <yq/collection/c_utils.hpp>
#include <yq/db/IDLock.hpp>
#include <yq/db/NKI.hpp>
#include <yq/db/Sq.hpp>
#include <yq/image/Image.hpp>
#include <yq/image/ImageDB.hpp>
#include <yq/io/dir_utils.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>

#include "DirectoryArg.ipp"
#include "DirectoryDB.ipp"
#include "DirectoryHtml.ipp"

#include "DocumentArg.ipp"
#include "DocumentDB.ipp"
#include "DocumentHtml.ipp"

#include "FolderArg.ipp"
#include "FolderDB.ipp"
#include "FolderHtml.ipp"

#include "FragmentArg.ipp"
#include "FragmentDB.ipp"
#include "FragmentHtml.ipp"

#include "RootArg.ipp"
#include "RootDB.ipp"
#include "RootHtml.ipp"

#include <yq/db/IDLock.ipp>

namespace yq {
    template class IDLock<Fragment>;
}
