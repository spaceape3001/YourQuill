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


    void    page_dev_echo(WebHtml& h)
    {
        auto& ctx   = h.context();
        ctx.decode_query();
        ctx.decode_post();

        h.title("Developer's Echo");
        
        bool    isPost  = ctx.method == hPost;
        
        h.h2("Environment");
        {
            auto ta = h.table();
            h.kvrow("Session ID") << ctx.session.ssid;
            h.kvrow("User") << ctx.session.username;
            h.kvrow("Time") << ctx.timestamp;
            h.kvrow("Path") << ctx.url.path;
            h.kvrow("Method") << ctx.method;
            h.kvrow( "From") << ctx.remote_addr.to_string() << ":" << ctx.remote_port;
        }
        
        h.h2("URL Query");
        h << ctx.rx_query;

        if(isPost){
            h.h2("Post Parameters");
            std::string     url = ctx.rx_post.first("url");
            if(!url.empty()){
                h << "<form action=\"" << url << "\" method=\"post\">\n";
                for(auto& i : ctx.rx_post_raw)
                    h << "<input type=\"hidden\" name=\"" << i.first << "\" id=\"" << i.first << "\" value=\"" << i.second << "\">\n";
                h << "<input type=\"submit\" value=\"Forward\">\n"
                  << "</form>\n";
            }
            
            h << ctx.rx_post;
        }
        
        h.h2("Headers");
        h << ctx.rx_headers;
        
        if(!ctx.rx_body.empty()){
            h.h2("Body");
            h << "<pre>\n";
            html_escape_write(h, ctx.rx_body);
            h << "</pre>\n";
        }
    }


    void    page_dev_hello(WebHtml& out)
    {
        out.title("HELLO WORLD!");
        
        auto _t     = out.table();
        
        out.kvrow("Method") << out.context().method.key() << "\n";
        out.kvrow("URL") << out.context().url << "\n";
        out.kvrow("Client HTTP") << out.context().version << "</td></tr>\n";
        for(auto& hv : out.context().rx_headers)
            out.kvrow(hv.first) << hv.second;
    }
    
    void page_dev_mimetypes(WebHtml&h)
    {
        h.title("Mime Types");
        auto ta = h.table();
        h << "<tr><th>ID</th><th>KEY</th><th>MIME-TYPE</th></tr>\n";
        for(ContentType ct : ContentType::all_values())
            h << "<tr><td>" << ct.value() << "</td><td>" << ct.key() << "</td><td>" << mimeType(ct) << "</td></tr>\n";
    }


    void page_dev_server(WebHtml&h)
    {
        h.title("Server Information");
        auto ta  = h.table();
        
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Auxillary Ports") << join(wksp::aux_ports(), ", ");
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Dot") << wksp::dot();
        h.kvrow("GIT") << wksp::git();
        h.kvrow("Home") << wksp::home();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Local User") << wksp::local_user();
        h.kvrow("Log Directory") << wksp::log_dir();
        h.kvrow("Markdown") << wksp::markdown();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Perl") << wksp::perl();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Quill Key") << wksp::quill_key();
        h.kvrow("Quill Path") << wksp::quill_file();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        
        {
            auto r = h.kvrow("Roots");
            r << "<OL>";
            for(const Root* rt : wksp::roots())
                r << "<LI>[" << rt -> key << "]: " << rt->path;
            r << "</OL>";
        }
        
        h.kvrow("Smartypants") << wksp::smartypants();
        h.kvrow("Start") << wksp::start();
        
        h.kvrow("Temp Path") << wksp::temp_dir();
        
        {
            auto r = h.kvrow("Templates");
            r << "<OL>";
            for(const std::string& t : wksp::templates())
                r << "<LI>" << t;
            r << "</OL>";
        }
    }


    void    dev_page_users(WebHtml& h)
    {
        h.title() << "All Users";
        dev_table(h, cdb::all_users(Sorted::YES));
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

    void    page_extension_markdown( WebHtml& out )
    {
        WebContext&ctx  = out.context();
        std::string     bytes   = file_string(ctx.resolved_file);
        if(bytes.empty())
            throw hNotFound;

        std::string_view    t   = web_title(bytes);
        std::string_view    d;
        
        if(!t.empty()){
            const char* z0  = t.data()+t.size();
            const char* z1  = bytes.data() + bytes.size();
            d               = std::string_view(z0, z1);
            ctx.var_title   = trimmed(t);
        } else {
            d               = bytes;
            ctx.var_title   = ctx.resolved_file.filename().string();
        }
        
        out << markdown(d);

        //stream::Text    render(ctx.var_body);
        //Markdown::Config    config;
        //Markdown            parser(render, config);
        //parser.process(d);
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
        reg_webpage<page_dev_echo>({hGet, hPost}, "/dev/echo");
        reg_webpage<page_dev_hello>("/dev/hello");
        reg_webpage<page_dev_mimetypes>("/dev/mimetypes");
        reg_webpage<page_dev_server>("/dev/server").local();
        reg_webpage<dev_page_users>("/dev/users");
        reg_webgroup({
            reg_webpage<page_dev_wksp>("/dev/wksp").label("Info"),
            reg_webpage<page_dev_wksp_roots>("/dev/wksp/roots").label("Roots")
        });
        reg_webpage<page_extension_markdown>("*.md");
        
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



