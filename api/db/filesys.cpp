////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/filesys.hpp>

#include <db/imagesys.hpp>
#include <db/bit/nki.hpp>
#include <db/core/cdb_sq.hpp>
#include <db/core/id_lock.hpp>
#include <db/wksp/workspace.hpp>

#include <yq/collection/Set.hpp>
#include <yq/collection/vector_utils.hpp>
#include <yq/file/dir_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

#include <db/filesys/directory-arg.ipp>
#include <db/filesys/document-arg.ipp>
#include <db/filesys/folder-arg.ipp>
#include <db/filesys/fragment-arg.ipp>
#include <db/filesys/root-arg.ipp>

#include <db/filesys/directory-cdb.ipp>
#include <db/filesys/document-cdb.ipp>
#include <db/filesys/folder-cdb.ipp>
#include <db/filesys/fragment-cdb.ipp>
#include <db/filesys/root-cdb.ipp>

#include <db/filesys/directory-html.ipp>
#include <db/filesys/document-html.ipp>
#include <db/filesys/folder-html.ipp>
#include <db/filesys/fragment-html.ipp>
#include <db/filesys/root-html.ipp>

#include <db/core/id_lock.ipp>

namespace yq {
    template class IDLock<Fragment>;
}
