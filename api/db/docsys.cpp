////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "docsys.hpp"

#include <db/filesys.hpp>
#include <db/imagesys.hpp>
#include <db/bit/nki.hpp>
#include <db/core/cdb_sq.hpp>
#include <db/core/id_lock.hpp>
#include <db/core/strings.hpp>

#include <yq/collection/c_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/file/XmlUtils.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>


#include <db/docsys/leaf-arg.ipp>
#include <db/docsys/leaf-cdb.ipp>
#include <db/docsys/leaf-data.ipp>
#include <db/docsys/leaf-file.ipp>
#include <db/docsys/leaf-html.ipp>

#include <db/core/id_lock.ipp>

namespace yq {
    template class IDLock<Leaf>;
}
