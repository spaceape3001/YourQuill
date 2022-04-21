////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/db.hpp>

#include <yq/app/DelayInit.hpp>
#include <yq/file/dir_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/Markdown.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/JsonAdapter.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebTemplate.hpp>

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;

#include "common.hpp"

#include "page/_index.ipp"

#include "page/admin_users.ipp"

#include "page/api_workspace.ipp"

#include "page/background.ipp"
#include "page/css.ipp"

#include "page/dev.ipp"
#include "page/dev_directories.ipp"
#include "page/dev_directory.ipp"
#include "page/dev_document.ipp"
#include "page/dev_documents.ipp"
#include "page/dev_echo.ipp"
#include "page/dev_folder.ipp"
#include "page/dev_folders.ipp"
#include "page/dev_fragment.ipp"
#include "page/dev_fragment_image.ipp"
#include "page/dev_fragments.ipp"
#include "page/dev_hello.ipp"
#include "page/dev_image.ipp"
#include "page/dev_images.ipp"
#include "page/dev_meta_types.ipp"
#include "page/dev_mimetypes.ipp"
#include "page/dev_root.ipp"
#include "page/dev_roots.ipp"
#include "page/dev_sql_table.ipp"
#include "page/dev_sql_tables.ipp"
#include "page/dev_tag.ipp"
#include "page/dev_tags.ipp"
#include "page/dev_users.ipp"
#include "page/dev_webdirs.ipp"
#include "page/dev_webexts.ipp"
#include "page/dev_webglobs.ipp"
#include "page/dev_webpages.ipp"
#include "page/dev_webvars.ipp"
#include "page/dev_wksp.ipp"

#include "page/ext_markdown.ipp"
#include "page/favicon.ico.ipp"
#include "page/help.ipp"
#include "page/image.ipp"
#include "page/img.ipp"
#include "page/js.ipp"
#include "page/logo.ipp"

#include "page/thumbnail.ipp"
#include "page/user.ipp"
