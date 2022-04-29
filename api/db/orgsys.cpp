////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "orgsys.hpp"

#include <db/cdb_sq.hpp>
#include <db/atomsys/class.hpp>
#include <db/atomsys/field.hpp>
#include <db/filesys.hpp>
#include <db/id_lock.hpp>
#include <db/imagesys.hpp>
#include <db/bit/nki.hpp>
#include <db/wksp/workspace.hpp>
#include <yq/collection/Set.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/file/Strings.hpp>
#include <yq/file/XmlUtils.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

#include <db/orgsys/category-arg.ipp>
#include <db/orgsys/category-cdb.ipp>
#include <db/orgsys/category-data.ipp>
#include <db/orgsys/category-file.ipp>
#include <db/orgsys/category-html.ipp>

#include <db/orgsys/tag-arg.ipp>
#include <db/orgsys/tag-cdb.ipp>
#include <db/orgsys/tag-data.ipp>
#include <db/orgsys/tag-file.ipp>
#include <db/orgsys/tag-html.ipp>

#include <db/id_lock.ipp>

namespace yq {
    template class IDLock<Category>;
    template class IDLock<Tag>;
}
