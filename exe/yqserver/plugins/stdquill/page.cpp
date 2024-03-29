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
#include <mithril/tag.hpp>
#include <mithril/user.hpp>

#include <0/basic/CollectionUtils.hpp>
#include <0/basic/DelayInit.hpp>
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

namespace {
    void    p_admin_categories(WebHtml& h)
    {
        h.title() << "Categories for [" << html_escape(wksp::name()) << "]";

        h << "<p><div class=\"explain\">"
          << "Categories are used to group classes and fields into order for display purposes."
          << "See <a href=\"/help/category.md\">HELP</a> for assistance."
          << "</div><p>\n";

        if(h.context().can_edit()){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            new_category_control(h, "/admin/categories/create");
            h << "</table>\n";
        }

        admin_table(h, cdb::all_categories(Sorted::YES));
    }
    
    void    p_admin_categories_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const RootDir* rt      = post::root_dir(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
        if(!rt->is_writable(DataRole::Config))
            throw HttpStatus::Forbidden;
            
        std::string     k   = post::key(ctx);
        if(k.empty())
            throw HttpStatus::BadArgument;

        bool    created = false;
        Category     t = cdb::make_category(k, rt, 0, &created);
        if(!t)
            throw HttpStatus::UnableToPerform;
    
        yInfo() << "Created category " << cdb::key(t) << " in root_dir " << rt->path;
    
        if(edit_now){
            Url url;
            url.path    = "/admin/category";
            stream::Text    qu(url.query);
            qu << "id=" << t.id;
            if(rt)
                qu << "&root_dir=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
    }
    
    void    p_admin_category(WebHtml& h)
    {
        Category    c = category(h);
        if(!c)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category " << key(c);
        h << "TODO Category " << label(c);
    }
    
    void    p_admin_class(WebHtml& h)
    {
        Class c = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
        
        h.title() << "Class " << key(c);
        h << "TODO Class " << label(c);
    }

    void    p_admin_classes(WebHtml& h)
    {
        h.title() << "Classes for [" << html_escape(wksp::name()) << "]";

        h << "<p><div class=\"explain\">"
          << "Classes declare the atom classifications that are to be tracked in the cache database."
          << "See <a href=\"/help/class.md\">HELP</a> for assistance."
          << "</div><p>\n";

        if(h.context().can_edit()){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            new_class_control(h, "/admin/classes/create");
            h << "</table>\n";
        }
        
        admin_table(h, cdb::all_classes(Sorted::YES));
    }
    
    void    p_admin_classes_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const RootDir* rt      = post::root_dir(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
        if(!rt->is_writable(DataRole::Config))
            throw HttpStatus::Forbidden;
            
        std::string     k   = post::key(ctx);
        if(k.empty())
            throw HttpStatus::BadArgument;

        bool    created = false;
        Class     t = cdb::make_class(k, rt, 0, &created);
        if(!t)
            throw HttpStatus::UnableToPerform;
    
        if(edit_now){
            Url url;
            url.path    = "/admin/class";
            stream::Text    qu(url.query);
            qu << "id=" << t.id;
            if(rt)
                qu << "&root_dir=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
    }
    
    void    p_admin_field(WebHtml&h)
    {
        Field f = field(h);
        if(!f)
            throw HttpStatus::BadArgument;
        
        h.title() << "Field " << key(f);
        h << "TODO Field " << label(f);
    }

    void    p_admin_fields(WebHtml& h)
    {
        h.title() << "Fields for [" << html_escape(wksp::name()) << "]";

        h << "<p><div class=\"explain\">"
          << "Fields declare the atom properties that are to be tracked in the cache database."
          << "See <a href=\"/help/field.md\">HELP</a> for assistance."
          << "</div><p>\n";

        if(h.context().can_edit()){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            new_field_control(h, "/admin/fields/create");
            h << "</table>\n";
        }

        admin_table(h,  cdb::all_fields(Sorted::YES));
    }

    void    p_admin_fields_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const RootDir* rt      = post::root_dir(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
        if(!rt->is_writable(DataRole::Config))
            throw HttpStatus::Forbidden;
            
        std::string     k   = post::key(ctx);
        if(k.empty())
            throw HttpStatus::BadArgument;
            
        Class           c   = post::class_(ctx);

        bool    created = false;
        Field     t = cdb::make_field(k, c, rt, 0, &created);
        if(!t)
            throw HttpStatus::UnableToPerform;
    
        if(edit_now){
            Url url;
            url.path    = "/admin/field";
            stream::Text    qu(url.query);
            qu << "id=" << t.id;
            if(rt)
                qu << "&root_dir=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
    }

    void    p_admin_tags(WebHtml& h)
    {
        h.title() << "Tags for [" << html_escape(wksp::name()) << "]";

        h << "<p><div class=\"explain\">"
          << "Tags can annotate leafs, classes, fields, atoms, and more with a specific trait.  "
          << "See <a href=\"/help/tag.md\">HELP</a> for assistance."
          << "</div><p>\n";

        if(h.context().can_edit()){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            new_tag_control(h, "/admin/tags/create");
            h << "</table>\n";
        }

        admin_table(h, cdb::all_tags(Sorted::YES));

    }

    void    p_admin_tag(WebHtml& h)
    {
        Tag     t   = arg::tag(h);
        if(!t)
            throw HttpStatus::BadArgument;
            
        h.title() << "Tag (" << key(t) << ")";
        h << "TODO... tag " << label(t);
    }

    void    p_admin_tags_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const RootDir* rt      = post::root_dir(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
        if(!rt->is_writable(DataRole::Config))
            throw HttpStatus::Forbidden;
            
        std::string     k   = post::key(ctx);
        if(k.empty())
            throw HttpStatus::BadArgument;
        
        
        bool    created = false;
        Tag     t = cdb::make_tag(k, rt, 0, &created);
        if(!t)
            throw HttpStatus::UnableToPerform;
        
        if(edit_now){
            Url url;
            url.path    = "/admin/tag";
            stream::Text    qu(url.query);
            qu << "id=" << t.id;
            if(rt)
                qu << "&root_dir=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
    }

    void    p_admin_user(WebHtml&h)
    {
        User    u   = user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << key(u);
        h << "TODO (User " << name(u) << ")";
    }

    void    p_admin_users(WebHtml&h)
    {
        h.title() << "Users for [" << html_escape(wksp::name()) << "]";
        
        h << "<p><div class=\"explain\">"
          << "Users are people allowed to view/modify the database from the web-interface."
          << "See <a href=\"/help/user.md\">HELP</a> for assistance."
          << "</div><p>\n";
        
        if(h.context().can_edit()){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            new_user_control(h, "/admin/users/create");
            h << "</table>\n";
        }
        
        admin_table(h, cdb::all_users(Sorted::YES));
    }
    
    void    p_admin_users_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const RootDir* rt      = post::root_dir(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
        if(!rt->is_writable(DataRole::Users))
            throw HttpStatus::Forbidden;
            
        std::string     k   = post::key(ctx);
        if(k.empty())
            throw HttpStatus::BadArgument;

        bool    created = false;
        User     t = cdb::make_user(k, rt, 0, &created);
        if(!t)
            throw HttpStatus::UnableToPerform;
    
        if(edit_now){
            Url url;
            url.path    = "/admin/user";
            stream::Text    qu(url.query);
            qu << "id=" << t.id;
            if(rt)
                qu << "&root_dir=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
    }
    
    json    p_api_easter(WebContext& ctx)
    {
        std::string     arg_string = ctx.find_query("year");
        if(arg_string.empty())
            throw HttpStatus::BadArgument;
        auto    x   = to_int16(arg_string);
        if(!x)
            throw HttpStatus::BadArgument;
            
        int16_t y   = *x;
        Date    d   = easter(y);
        
        json    ret{
            { "month", to_string(d.month) },
            { "day", to_string(d.day) }
        };
        return ret;
    }

    json    p_api_wksp(WebContext&ctx)
    {
        json    ret{
            { "author", wksp::author() },
            { "abbreviation", wksp::abbreviation() },
            { "bkcolor", gBkColor },
            { "copyright", wksp::copyright().text },
            { "c_stance", wksp::copyright().stance.key() },
            { "c_from", wksp::copyright().from },
            { "c_to", wksp::copyright().to },
            { "color", gTextColor },
            { "name", wksp::name() }
        };
        if(ctx.is_local()){
            ret["quill"] = wksp::quill_file().string();
            ret["cache"] = wksp::cache().string();
        }
        return ret;
    }

    json    p_api_wksp_quill(WebContext&ctx)
    {
        json    ret;
        if(ctx.is_local()){
            ret["quill"] = wksp::quill_file().string();
        }
        return ret;
    }

    void p_atom(WebHtml& h)
    {
        //   this will redirect the atom appropriately
        h << "TODO";
    }
    
    void p_atoms(WebHtml& h)
    {
        //   this will be a search-spot for the appropriate atoms
        h << "TODO";
    }

    void p_class(WebHtml& h)
    {
        Class x = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Info for class " << cdb::label(x);
        h << "TODO!";
    }
    
    void p_class_atoms(WebHtml&h)
    {
        Class x = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << cdb::plural(x);
        h << "TODO!";
    }

    void    p_css(WebContext& ctx)
    {
        ctx.tx_content_type = ContentType::css;
        ctx.tx_content = gCss;
    }

    void    p_graphviz(WebHtml& h)
    {
        Graphviz    gviz = graphviz(h);
        if(!gviz)
            throw HttpStatus::BadArgument;
        h.title() << cdb::name(gviz);
        h << gviz;
    }

    void    p_graphviz_svg(WebContext& ctx)
    {
        Graphviz    gviz = graphviz(ctx);
        if(!gviz)
            throw HttpStatus::BadArgument;
        ctx.tx_content_type = ContentType::svg;
        ctx.tx_content      = std::make_shared<ByteArray>(cdb::svg_bytes(gviz));
    }

    void    p_image(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;

        ByteArray   by   = bytes(img, SizeDesc::Original);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }

    void    p_thumbnail(WebContext& ctx)
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

    void    p_user(WebHtml& h)
    {
        User    u   = arg::user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << cdb::name(u);
        h.p() << "Fill in the blank... TODO!";
    }

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
}

#include "p_dev.ipp"
#include "p_explorer.ipp"
#include "p_wiki.ipp"


namespace {

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
        reg_webgroup({
            reg_webtemplate("/admin", wksp::shared("www/admin.ht"sv)).source(".admin").label("Info"),
            reg_webpage<p_admin_categories>("/admin/categories").label("Categories"),
            reg_webpage<p_admin_classes>("/admin/classes").label("Classes"),
            reg_webpage<p_admin_fields>("/admin/fields").label("Fields"),
            reg_webpage<p_admin_tags>("/admin/tags").label("Tags"),
            reg_webpage<p_admin_users>("/admin/users").label("Users")
        });
        reg_webpage<p_admin_categories_create>(hPost, "/admin/categories/create");
        reg_webpage<p_admin_category>("/admin/category").argument("id", "Category ID");
        reg_webpage<p_admin_class>("/admin/class").argument("id", "Class ID");
        reg_webpage<p_admin_classes_create>(hPost, "/admin/classes/create");
        reg_webpage<p_admin_field>("/admin/field").argument("id", "Field ID");
        reg_webpage<p_admin_fields_create>(hPost, "/admin/fields/create");
        reg_webpage<p_admin_tag>("/admin/tag").argument("id", "Tag ID");;
        reg_webpage<p_admin_tags_create>(hPost, "/admin/tags/create");
        reg_webpage<p_admin_user>("/admin/user").argument("id", "User ID");
        reg_webpage<p_admin_users_create>(hPost, "/admin/users/create");
        
        reg_webpage<p_api_easter>("/api/easter"sv);
        reg_webpage<p_api_wksp>("/api/wksp"sv); 
        reg_webpage<p_api_wksp_quill>("/api/wksp/quill"sv); 

        reg_webpage<p_atom>("/atom").argument("ID", "Atom ID");
        reg_webpage<p_atoms>("/atoms");

        reg_webgroup({
            reg_webpage<p_class>("/class").argument("ID", "Class ID").label("Overview"),
            reg_webpage<p_class_atoms>("/class/atoms").argument("ID", "Class ID").label("Atoms")
        });

        reg_webpage<p_css>("/css");
        
        reg_dev();



        reg_explorer();
        
        reg_webpage<p_graphviz>("/graphviz").argument("id", "ID for the graphviz").description("Shows a graphviz graph");
        reg_webpage<p_graphviz_svg>("/graphviz/svg").argument("id", "ID for the graphviz").description("Graphviz SVG");
        
        reg_webpage<p_image>("/image").argument("id", "ID for the image");
        
        reg_wiki();
        
        reg_webpage<p_thumbnail>("/thumbnail").argument("id", "ID for the image");
        reg_webpage<p_user>("/user");
        
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

