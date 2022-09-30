////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_admin_categories(WebHtml& h)
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
    
    void    page_admin_categories_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const Root* rt      = post::root(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
            
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
    
    void    page_admin_category(WebHtml& h)
    {
        Category    c = category(h);
        if(!c)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category " << key(c);
        h << "TODO Category " << label(c);
    }
    
    void    page_admin_class(WebHtml& h)
    {
        Class c = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
        
        h.title() << "Class " << key(c);
        h << "TODO Class " << label(c);
    }

    void    page_admin_classes(WebHtml& h)
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
    
    void    page_admin_classes_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const Root* rt      = post::root(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
            
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
    
    void    page_admin_field(WebHtml&h)
    {
        Field f = field(h);
        if(!f)
            throw HttpStatus::BadArgument;
        
        h.title() << "Field " << key(f);
        h << "TODO Field " << label(f);
    }

    void    page_admin_fields(WebHtml& h)
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

    void    page_admin_fields_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const Root* rt      = post::root(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
            
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

    void    page_admin_tags(WebHtml& h)
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

    void    page_admin_tag(WebHtml& h)
    {
        Tag     t   = arg::tag(h);
        if(!t)
            throw HttpStatus::BadArgument;
            
        h.title() << "Tag (" << key(t) << ")";
        h << "TODO... tag " << label(t);
    }

    void    page_admin_tags_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const Root* rt      = post::root(ctx);
            
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

    void    page_admin_user(WebHtml&h)
    {
        User    u   = user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << key(u);
        h << "TODO (User " << name(u) << ")";
    }

    void    page_admin_users(WebHtml&h)
    {
        h.title() << "Users for [" << html_escape(wksp::name()) << "]";
        
        
        if(h.context().can_edit()){
            h << "<table align=\"right\" width=\"30%\"><tr><td>\n";
            new_user_control(h, "/admin/users/create");
            h << "</table>\n";
        }
        
        admin_table(h, cdb::all_users(Sorted::YES));
    }
    
    void    page_admin_users_create(WebContext& ctx)
    {
        if(!ctx.can_edit())
            throw HttpStatus::Unauthorized;
        
        ctx.decode_post();
        
        bool  edit_now      = ctx.edit_now();
        const Root* rt      = post::root(ctx);
        if(!rt)
            throw HttpStatus::BadArgument;
            
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

    void    reg_admin()
    {
        reg_webgroup({
            reg_webtemplate("/admin", wksp::shared("std/admin"sv)).source(".admin").label("Info"),
            reg_webpage<page_admin_categories>("/admin/categories").label("Categories"),
            reg_webpage<page_admin_classes>("/admin/classes").label("Classes"),
            reg_webpage<page_admin_fields>("/admin/fields").label("Fields"),
            reg_webpage<page_admin_tags>("/admin/tags").label("Tags"),
            reg_webpage<page_admin_users>("/admin/users").label("Users")
        });
        reg_webpage<page_admin_categories_create>(hPost, "/admin/categories/create");
        reg_webpage<page_admin_category>("/admin/category").argument("id", "Category ID");
        reg_webpage<page_admin_class>("/admin/class").argument("id", "Class ID");
        reg_webpage<page_admin_classes_create>(hPost, "/admin/classes/create");
        reg_webpage<page_admin_field>("/admin/field").argument("id", "Field ID");
        reg_webpage<page_admin_fields_create>(hPost, "/admin/fields/create");
        reg_webpage<page_admin_tag>("/admin/tag").argument("id", "Tag ID");;
        reg_webpage<page_admin_tags_create>(hPost, "/admin/tags/create");
        reg_webpage<page_admin_user>("/admin/user").argument("id", "User ID");
        reg_webpage<page_admin_users_create>(hPost, "/admin/users/create");
    }
}
