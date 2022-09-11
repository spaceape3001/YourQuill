////////////////////////////////////////////////////////////////////////////////
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

#include "common.hpp"

#include "p_api_wksp.ipp"


namespace {


    void    page_css(WebContext& ctx)
    {
        ctx.tx_content_type = ContentType::css;
        ctx.tx_content = gCss;
    }



    void  page_dev_wksp(WebHtml& h)
    {
        h.title() << "Workspace '" << wksp::name() << "'";
        
        auto t = h.table();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Color (Background)") << gBkColor.get();
        h.kvrow("Color (Text)") << gTextColor.get();
        //h.kvrow("Config") << dev(cdb::config_folder());
        h.kvrow("Copyright") << wksp::copyright().text;
        h.kvrow("Key") << wksp::quill_key();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        h.kvrow("Start") << wksp::start();
        h.kvrow("Templates") << join(wksp::templates(), ", ");
    }

    void    page_dev_wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::roots());
    }

    void    page_image(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;

        ByteArray   by   = bytes(img, SizeDesc::Original);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }

    void    page_leaf(WebHtml& h)
    {
        //  BIG FAT TODO!
    }

    void    page_thumbnail(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;
        
        bool    detected    = false;
        SizeDesc    sz  = size_desc(ctx, "size", &detected);
        if(!detected)
            sz      = ctx.session.icon_size;
        
        ByteArray   by   = bytes(img, sz);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }

    void    page_user(WebHtml& h)
    {
        User    u   = arg::user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << cdb::name(u);
        h.p() << "Fill in the blank... TODO!";
    }
    
    void    reg_me()
    {
        reg_webtemplate("/", wksp::shared("std/index"sv)).source(".index");
        
        reg_api_wksp();

        reg_webimage("/background", std::filesystem::path(), Folder(), ".background").post([](WebImage& wi){
            bool    now = wi.hasImage();
            if(gHasBackground.exchange(now) != now)
                update_css();
        });
        reg_webpage<page_css>("/css");
        reg_webtemplate("/dev", wksp::shared("std/developer"sv)).source(".developer");
        reg_webpage("/dev/**", wksp::shared_all("www/dev"sv));
        reg_webpage<page_dev_mimetypes>("/dev/mimetypes");
        reg_webgroup({
            reg_webpage<page_dev_wksp>("/dev/wksp").label("Info"),
            reg_webpage<page_dev_wksp_roots>("/dev/wksp/roots").label("Roots")
        });
        
        reg_webtemplate("/help", wksp::shared("std/help"sv));
        reg_webpage("/help/**", wksp::shared_all("www/help"sv));
        reg_webpage<page_image>("/image").argument("id", "ID for the image");
        reg_webpage("/img/**", wksp::shared_all("www/img"sv));
        reg_webpage("/img/yquill.svg", wksp::shared("www/img/yquill.svg"sv));   // precaching
        reg_webpage("/js/**", wksp::shared("www/js/jquery.js"sv));
        reg_webpage("/js/jquery.js", wksp::shared("www/js/jquery.js"sv));      // precaching
        reg_webpage<page_leaf>("/leaf").argument("id", "Leaf ID");
        reg_webimage("/logo", wksp::shared("www/img/yquill.svg"sv), Folder(), ".logo").alt_path("/favicon.ico");
        reg_webpage<page_thumbnail>("/thumbnail").argument("id", "ID for the image");
        reg_webpage<page_user>("/user");
    }
    
    
    YQ_INVOKE(reg_me();)
}



