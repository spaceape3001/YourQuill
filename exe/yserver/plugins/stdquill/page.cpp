//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <yq/mithril/atom.hpp>
#include <yq/mithril/attribute.hpp>
#include <yq/mithril/category.hpp>
#include <yq/mithril/class.hpp>
#include <yq/mithril/directory.hpp>
#include <yq/mithril/document.hpp>
#include <yq/mithril/doodle.hpp>
#include <yq/mithril/folder.hpp>
#include <yq/mithril/field.hpp>
#include <yq/mithril/fragment.hpp>
#include <yq/mithril/graphviz.hpp>
#include <yq/mithril/image.hpp>
#include <yq/mithril/leaf.hpp>
#include <yq/mithril/meta.hpp>
#include <yq/mithril/novel.hpp>
#include <yq/mithril/root.hpp>
#include <yq/mithril/style.hpp>
#include <yq/mithril/tag.hpp>
#include <yq/mithril/user.hpp>
#include <yq/mithril/value.hpp>

//#include <0/basic/CollectionUtils.hpp>
#include <yq/core/DelayInit.hpp>
#include <yq/core/Global.hpp>
#include <yq/core/Logging.hpp>
#include <yq/core/StreamOps.hpp>
#include <yq/date/Date.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/math/Random.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>


#include <yq/meta/ObjectMeta.hpp>
#include <yq/meta/TypeMeta.hpp>

#include <yq/stream/Text.hpp>

#include <yq/mithril/bit/Copyright.hpp>
#include <yq/mithril/bit/NKI.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

#include <yq/mithril/html/HtmlLayout.hpp>
#include <yq/mithril/util/Markdown.hpp>
#include <yq/mithril/web/JsonAdapter.hpp>
#include <yq/mithril/web/WebAdapters.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/web/WebHtml.hpp>
#include <yq/mithril/web/WebImage.hpp>
#include <yq/mithril/web/WebPage.hpp>
#include <yq/mithril/web/WebRedirect.hpp>
#include <yq/mithril/web/WebTemplate.hpp>
#include <yq/mithril/web/Template.hpp>

#include "common.hpp"

using namespace yq;
using namespace yq::mithril;
using namespace yq::mithril::arg;
using namespace yq::mithril::cdb;
using namespace yq::mithril::html;

#include "p/atom.ipp"
#include "p/attribute.ipp"
#include "p/category.ipp"
#include "p/class.ipp"
#include "p/directory.ipp"
#include "p/document.ipp"
#include "p/doodle.ipp"
#include "p/explorer.ipp"
#include "p/field.ipp"
#include "p/folder.ipp"
#include "p/fragment.ipp"
#include "p/graphviz.ipp"
#include "p/image.ipp"
#include "p/leaf.ipp"
#include "p/meta.ipp"
#include "p/misc.ipp"
#include "p/novel.ipp"
#include "p/quill.ipp"
#include "p/root.ipp"
#include "p/server.ipp"
#include "p/sql.ipp"
#include "p/style.ipp"
#include "p/tag.ipp"
#include "p/user.ipp"
#include "p/value.ipp"
#include "p/wksp.ipp"

namespace {
    void    x_markdown( WebHtml& out )
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
    
    void    x_html_template(WebHtml& out)
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
        out << d;
    }

    path_vector_t       make_dir_set(std::string_view sname, std::string_view wname)
    {
        path_vector_t   ret;
        for(const RootDir* rt : wksp::root_dirs())
            ret.push_back(rt -> resolve(sname));
        for(auto& p : wksp::shared_all(wname))
            ret.push_back(p);
        return ret;
    }


    void    reg_me()
    {
        reg_atom_pages();
        reg_attribute_pages();
        reg_category_pages();
        reg_class_pages();
        reg_directory_pages();
        reg_document_pages();
        reg_doodle_pages();
        reg_explorer_pages();
        reg_field_pages();
        reg_folder_pages();
        reg_fragment_pages();
        reg_graphviz_pages();
        reg_image_pages();
        reg_leaf_pages();
        reg_meta_pages();
        reg_misc_pages();
        reg_novel_pages();
        reg_quill_pages();
        reg_root_pages();
        reg_server_pages();
        reg_sql_pages();
        reg_style_pages();
        reg_tag_pages();
        reg_user_pages();
        reg_value_pages();
        reg_wksp_pages();
    
    
        reg_webgroup({
            reg_webtemplate("/admin", wksp::shared("www/admin.ht"sv)).source(".admin").label("Info"),
            reg_webpage<p_admin_categories>("/admin/categories").label("Categories"),
            reg_webpage<p_admin_classes>("/admin/classes").label("Classes"),
            reg_webpage<p_admin_fields>("/admin/fields").label("Fields"),
            reg_webpage<p_admin_styles>("/admin/styles").label("Styles"),
            reg_webpage<p_admin_tags>("/admin/tags").label("Tags"),
            reg_webpage<p_admin_users>("/admin/users").label("Users")
        });
        
        reg_webpage("/admin/**", make_dir_set(".admin"sv, "www/admin"sv));

        reg_webpage<x_markdown>("*.md");
        reg_webpage<x_html_template>("*.ht");
        reg_webpage<x_html_template>("*.htt");

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
        reg_webpage("/beta/**", make_dir_set(".beta"sv, "www/beta"sv));
        reg_webpage("/help/**", make_dir_set(".help"sv, "www/help"sv));
        reg_webpage("/img/**", make_dir_set(".img", "www/img"sv));
        reg_webpage("/img/yquill.svg", wksp::shared("www/img/yquill.svg"sv));   // precaching
        reg_webpage("/js/**", make_dir_set(".js", "www/js"sv));
        reg_webpage("/js/jquery.js", wksp::shared("www/js/jquery.js"sv));      // precaching
        reg_webimage("/logo", wksp::shared("www/img/yquill.svg"sv), Folder(), ".logo").alt_path("/favicon.ico");
        reg_webpage("/pkg/**", wksp::shared_all("www/pkg"sv));

    }
    
    YQ_INVOKE(reg_me();)
}

