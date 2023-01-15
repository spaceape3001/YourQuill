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
#include <kernel/db/NKI.hpp>
#include <kernel/wksp/CacheQuery.hpp>
#include <kernel/wksp/Workspace.hpp>

#include <http/HtmlLayout.hpp>
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

#include "common.hpp"

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;

namespace {
    void    p_admin_categories(WebHtml& h)
    {
        h.title() << "Categories for [" << html_escape(wksp::name()) << "]";

        h << "<p><div class=\"explain\">"
          << "Categories are used to group classes and fields into order for display purposes."
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
        const Root* rt      = post::root(ctx);
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
    
        yInfo() << "Created category " << cdb::key(t) << " in root " << rt->path;
    
        if(edit_now){
            Url url;
            url.path    = "/admin/category";
            stream::Text    qu(url.query);
            qu << "id=" << t.id;
            if(rt)
                qu << "&root=" << rt->id;
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
        const Root* rt      = post::root(ctx);
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
                qu << "&root=" << rt->id;
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
        const Root* rt      = post::root(ctx);
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
                qu << "&root=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
    }

    void    p_admin_tags(WebHtml& h)
    {
        h.title() << "Tags for [" << html_escape(wksp::name()) << "]";

        h << "<p><div class=\"explain\">"
          << "Tags can annotate leafs, classes, fields, atoms, and more with a specific trait."
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
        const Root* rt      = post::root(ctx);
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
                qu << "&root=" << rt->id;
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
        const Root* rt      = post::root(ctx);
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
                qu << "&root=" << rt->id;
            throw redirect::see_other(url);
        } else {
            ctx.return_to_sender();
        }
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

