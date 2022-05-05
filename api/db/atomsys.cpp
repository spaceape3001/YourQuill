////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <db/atomsys.hpp>

#include <db/filesys.hpp>
#include <db/imagesys.hpp>
#include <db/core/cdb_sq.hpp>
#include <db/core/id_lock.hpp>
#include <db/orgsys/tag.hpp>
#include <db/wksp/workspace.hpp>

#include <yq/db/NKI.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/file/Strings.hpp>
#include <yq/file/XmlUtils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

#include <db/atomsys/atom-arg.ipp>
#include <db/atomsys/atom-cdb.ipp>
#include <db/atomsys/atom-html.ipp>

#include <db/atomsys/class-arg.ipp>
#include <db/atomsys/class-cdb.ipp>
#include <db/atomsys/class-data.ipp>
#include <db/atomsys/class-file.ipp>
#include <db/atomsys/class-html.ipp>

#include <db/atomsys/field-arg.ipp>
#include <db/atomsys/field-cdb.ipp>
#include <db/atomsys/field-data.ipp>
#include <db/atomsys/field-file.ipp>
#include <db/atomsys/field-html.ipp>

#include <db/atomsys/value-arg.ipp>
#include <db/atomsys/value-cdb.ipp>
#include <db/atomsys/value-data.ipp>
#include <db/atomsys/value-file.ipp>
#include <db/atomsys/value-html.ipp>

#include <db/core/id_lock.ipp>

namespace yq {
    template class IDLock<Atom>;
    template class IDLock<Class>;
    template class IDLock<Field>;
}
