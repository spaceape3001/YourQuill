//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <yq/atom.hpp>
#include <yq/attribute.hpp>
#include <yq/category.hpp>
#include <yq/class.hpp>
#include <yq/directory.hpp>
#include <yq/document.hpp>
#include <yq/folder.hpp>
#include <yq/field.hpp>
#include <yq/fragment.hpp>
#include <yq/image.hpp>
#include <yq/leaf.hpp>
#include <yq/meta.hpp>
#include <yq/property.hpp>
#include <yq/root.hpp>
#include <yq/tag.hpp>
#include <yq/user.hpp>

#include <basic/CollectionUtils.hpp>
#include <basic/DelayInit.hpp>
#include <basic/DirUtils.hpp>
#include <basic/FileUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/SqlLite.hpp>
#include <basic/SqlQuery.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>

#include <basic/meta/ObjectInfo.hpp>
#include <basic/meta/TypeInfo.hpp>

#include <basic/stream/Text.hpp>

#include <kernel/bit/Copyright.hpp>
#include <http/HtmlLayout.hpp>
#include <kernel/db/NKI.hpp>
#include <http/util/Markdown.hpp>
#include <http/web/JsonAdapter.hpp>
#include <http/web/WebAdapters.hpp>
#include <http/web/WebContext.hpp>
#include <http/web/WebHtml.hpp>
#include <http/web/WebImage.hpp>
#include <http/web/WebPage.hpp>
#include <http/web/WebRedirect.hpp>
#include <http/web/WebTemplate.hpp>
#include <http/web/Template.hpp>
#include <kernel/wksp/Workspace.hpp>

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;


#include "p_admin.ipp"
#include "p_atom.ipp"
#include "p_class.ipp"
#include "p_dev_atom.ipp"
#include "p_dev_category.ipp"
#include "p_dev_class.ipp"
#include "p_dev_directory.ipp"
#include "p_dev_document.ipp"
#include "p_dev_field.ipp"
#include "p_dev_folder.ipp"
#include "p_dev_fragment.ipp"
#include "p_dev_image.ipp"
#include "p_dev_leaf.ipp"
#include "p_dev_meta.ipp"
#include "p_dev_root.ipp"
#include "p_dev_sql.ipp"
#include "p_dev_tag.ipp"
#include "p_dev_web.ipp"

namespace {
    void    reg_me()
    {
        reg_admin();
        reg_atom();
        reg_class();
        reg_dev_atom();
        reg_dev_category();
        reg_dev_class();
        reg_dev_directory();
        reg_dev_document();
        reg_dev_field();
        reg_dev_folder();
        reg_dev_fragment();
        reg_dev_image();
        reg_dev_leaf();
        reg_dev_meta();
        reg_dev_root();
        reg_dev_sql();
        reg_dev_tag();
        reg_dev_web();        
    }
    
    
    
    YQ_INVOKE(reg_me();)
}

