////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/category/CategoryJson.hpp>
#include <yq/mithril/category/CategorySearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_category(WebContext& ctx)
        {
            Category    v   = arg::category(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_category_key(WebContext& ctx)
        {
            Category    v   = arg::category(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_categories(WebContext& ctx)
        {
            CategoryVector  ret = search(ctx, CATEGORY);
            return json{
                { "categories", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

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
    
    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_categories(WebHtml&h)
        {
            h.title() << "All Categories!";
            dev_table(h, all_categories(Sorted::YES));
        }

        void    p_dev_category(WebHtml& h)
        {
            Category    cat = category(h);
            if(!cat)
                throw HttpStatus::BadArgument;
            Category::Info  i   = info(cat);
            
            dev_title(h, cat);
            auto ta = h.table();
            
            h.kvrow("ID") << cat.id;
            h.kvrow("Key") << i.key;
            h.kvrow("Name") << i.name;
            h.kvrow("Brief") << i.brief;
        }
        
        void    p_dev_category_classes(WebHtml& h)
        {
            Category    cat = category(h);
            if(!cat)
                throw HttpStatus::BadArgument;
            
            dev_title(h, cat, "In Classes");
            dev_table(h, classes(cat));
        }

        void    p_dev_category_fields(WebHtml& h)
        {
            Category    cat = category(h);
            if(!cat)
                throw HttpStatus::BadArgument;
            
            dev_title(h, cat, "In Fields");
            dev_table(h, fields(cat));
        }


    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_category_pages()
        {
            reg_webpage<p_api_category>("/api/category").argument("ID", "Category ID");
            reg_webpage<p_api_category_key>("/api/category/key").argument("ID", "Category ID");
            reg_webpage<p_api_categories>("/api/categories");

            //reg_webpage<p_admin_categories>(hPost, "/admin/categories");  // registered in page.cpp
            reg_webpage<p_admin_categories_create>(hPost, "/admin/categories/create");
            reg_webpage<p_admin_category>("/admin/category").argument("id", "Category ID");

            reg_webpage<p_dev_categories>("/dev/categories"); 
            reg_webgroup({
                reg_webpage<p_dev_category>("/dev/category").argument("id", "Category ID").label("Info"),
                reg_webpage<p_dev_category_classes>("/dev/category/classes").argument("id", "Category ID").label("Classes"),
                reg_webpage<p_dev_category_fields>("/dev/category/fields").argument("id", "Category ID").label("Fields")
            });
        }
}
