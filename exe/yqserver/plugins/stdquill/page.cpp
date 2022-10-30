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
#include <kernel/db/SQ.hpp>
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

#include "common.hpp"

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;


#include "p_admin.ipp"
#include "p_api_wksp.ipp"
#include "p_atom.ipp"
#include "p_class.ipp"
#include "p_css.ipp"
#include "p_dev_atom.ipp"
#include "p_dev_category.ipp"
#include "p_dev_class.ipp"
#include "p_dev_directory.ipp"
#include "p_dev_document.ipp"
#include "p_dev_echo.ipp"
#include "p_dev_field.ipp"
#include "p_dev_folder.ipp"
#include "p_dev_fragment.ipp"
#include "p_dev_hello.ipp"
#include "p_dev_image.ipp"
#include "p_dev_leaf.ipp"
#include "p_dev_meta.ipp"
#include "p_dev_mimetypes.ipp"
#include "p_dev_root.ipp"
#include "p_dev_server.ipp"
#include "p_dev_sql.ipp"
#include "p_dev_tag.ipp"
#include "p_dev_user.ipp"
#include "p_dev_web.ipp"
#include "p_dev_wksp.ipp"
#include "p_explorer.ipp"
#include "p_image.ipp"
#include "p_leaf.ipp"
#include "p_user.ipp"

#include "x_markdown.ipp"

namespace {
    void    reg_me()
    {
        reg_admin();
        reg_api_wksp();
        reg_atom();
        reg_class();
        reg_css();
        reg_dev_atom();
        reg_dev_category();
        reg_dev_class();
        reg_dev_directory();
        reg_dev_document();
        reg_dev_echo();
        reg_dev_field();
        reg_dev_folder();
        reg_dev_fragment();
        reg_dev_hello();
        reg_dev_image();
        reg_dev_leaf();
        reg_dev_meta();
        reg_dev_mimetypes();
        reg_dev_root();
        reg_dev_server();
        reg_dev_sql();
        reg_dev_tag();
        reg_dev_user();
        reg_dev_web();
        reg_dev_wksp();
        reg_explorer();
        reg_image();
        reg_leaf();
        reg_user();
        
        reg_x_markdown();        

        reg_webtemplate("/", wksp::shared("www/index.ht"sv)).source(".index");

        reg_webimage("/background", std::filesystem::path(), Folder(), ".background").post([](WebImage& wi){
            bool    now = wi.hasImage();
            if(gHasBackground.exchange(now) != now)
                update_css();
        });
        reg_webtemplate("/dev", wksp::shared("www/developer.ht"sv)).source(".developer");
        reg_webpage("/dev/**", wksp::shared_all("www/dev"sv));
        
        reg_webtemplate("/files", wksp::shared("www/files.ht"sv));
        reg_webtemplate("/help", wksp::shared("www/help.ht"sv));
        reg_webpage("/help/**", wksp::shared_all("www/help"sv));
        reg_webpage("/img/**", wksp::shared_all("www/img"sv));
        reg_webpage("/img/yquill.svg", wksp::shared("www/img/yquill.svg"sv));   // precaching
        reg_webpage("/js/**", wksp::shared("www/js/jquery.js"sv));
        reg_webpage("/js/jquery.js", wksp::shared("www/js/jquery.js"sv));      // precaching
        reg_webimage("/logo", wksp::shared("www/img/yquill.svg"sv), Folder(), ".logo").alt_path("/favicon.ico");
        reg_webpage("/pkg/**", wksp::shared_all("www/pkg"sv));
    }
    
    
    
    YQ_INVOKE(reg_me();)
}

