//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include <mithril/atom.hpp>
#include <mithril/attribute.hpp>
#include <mithril/category.hpp>
#include <mithril/class.hpp>
#include <mithril/directory.hpp>
#include <mithril/document.hpp>
#include <mithril/folder.hpp>
#include <mithril/field.hpp>
#include <mithril/fragment.hpp>
#include <mithril/graphviz.hpp>
#include <mithril/image.hpp>
#include <mithril/leaf.hpp>
#include <mithril/meta.hpp>
#include <mithril/root.hpp>
#include <mithril/style.hpp>
#include <mithril/tag.hpp>
#include <mithril/user.hpp>
#include <mithril/value.hpp>

#include <0/basic/CollectionUtils.hpp>
#include <0/basic/DelayInit.hpp>
#include <0/basic/Global.hpp>
#include <0/basic/Random.hpp>
#include <0/io/DirUtils.hpp>
#include <0/io/FileUtils.hpp>
#include <0/gis/Date.hpp>
#include <0/basic/Logging.hpp>
#include <0/sql/SqlLite.hpp>
#include <0/sql/SqlQuery.hpp>
#include <0/io/StreamOps.hpp>
#include <0/basic/TextUtils.hpp>

#include <0/meta/ObjectInfo.hpp>
#include <0/meta/TypeInfo.hpp>

#include <0/io/stream/Text.hpp>

#include <mithril/bit/Copyright.hpp>
#include <mithril/bit/NKI.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

#include <mithril/html/HtmlLayout.hpp>
#include <mithril/util/Markdown.hpp>
#include <mithril/web/JsonAdapter.hpp>
#include <mithril/web/WebAdapters.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebImage.hpp>
#include <mithril/web/WebPage.hpp>
#include <mithril/web/WebRedirect.hpp>
#include <mithril/web/WebTemplate.hpp>
#include <mithril/web/Template.hpp>

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
#include "p/explorer.ipp"
#include "p/field.ipp"
#include "p/folder.ipp"
#include "p/fragment.ipp"
#include "p/graphviz.ipp"
#include "p/image.ipp"
#include "p/leaf.ipp"
#include "p/meta.ipp"
#include "p/misc.ipp"
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
        reg_explorer_pages();
        reg_field_pages();
        reg_folder_pages();
        reg_fragment_pages();
        reg_graphviz_pages();
        reg_image_pages();
        reg_leaf_pages();
        reg_meta_pages();
        reg_misc_pages();
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
        


        reg_webpage<x_markdown>("*.md");

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

