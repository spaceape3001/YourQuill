////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/user/UserJson.hpp>
#include <yq/mithril/user/UserSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------
        
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_user(WebContext& ctx)
        {
            User    v   = arg::user(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_user_key(WebContext& ctx)
        {
            User    v   = arg::user(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_users(WebContext& ctx)
        {
            UserVector  ret = search(ctx, USER);
            return json{
                { "users", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
        void    p_user(WebHtml& h)
        {
            User    u   = arg::user(h);
            if(!u)
                throw HttpStatus::BadArgument;
            
            h.title() << "User: " << cdb::name(u);
            h.p() << "Fill in the blank... TODO!";
        }

    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

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
    

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_user(WebHtml& h)
        {
            User    u   = arg::user(h);
            if(!u)
                throw HttpStatus::BadArgument;

            auto i   = info(u);
                
            dev_title(h, u);

            auto ta = h.table();
            h.kvrow("ID") << u.id;
            h.kvrow("Name") << i.name;
            h.kvrow("Key") << i.key;
        }

        void    p_dev_users(WebHtml& h)
        {
            h.title() << "All Users";
            dev_table(h, all_users(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_user_pages()
        {
            reg_webpage<p_api_user>("/api/user").argument("ID", "User ID");
            reg_webpage<p_api_user_key>("/api/user/key").argument("ID", "User ID");
            reg_webpage<p_api_users>("/api/users");

            reg_webpage<p_user>("/user");

            reg_webpage<p_admin_user>("/admin/user").argument("id", "User ID");
            //  reg_webpage<p_admin_users>("/admin/users"); // registered in page.cpp as part of a group
            reg_webpage<p_admin_users_create>(hPost, "/admin/users/create");

            reg_webpage<p_dev_user>("/dev/user")
                .argument("id", "User id (number)")
            ;
            reg_webpage<p_dev_users>("/dev/users");
        }
}
