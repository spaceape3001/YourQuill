////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "docsys.hpp"

#include <db/filesys.hpp>
#include <db/imagesys.hpp>
#include <db/core/cdb_sq.hpp>

#include <yq/collection/c_utils.hpp>
#include <yq/db/NKI.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/io/Strings.hpp>
#include <yq/io/XmlUtils.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>

#include <db/docsys/attribute-arg.ipp>
#include <db/docsys/attribute-cdb.ipp>
#include <db/docsys/attribute-diff.ipp>
#include <db/docsys/attribute-html.ipp>

#include <db/docsys/leaf-arg.ipp>
#include <db/docsys/leaf-cdb.ipp>
#include <db/docsys/leaf-html.ipp>

