////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/class/ClassJson.hpp>
#include <mithril/class/ClassSearch.hpp>
#include <yq/text/join.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_class(WebContext& ctx)
        {
            Class    v   = arg::class_(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }

        json p_api_class_key(WebContext& ctx)
        {
            Class    v   = arg::class_(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json{
                { "key", cdb::key(v) }
            };
        }

        json p_api_classes(WebContext& ctx)
        {
            ClassVector ret = search(ctx, CLASS);
            return json{
                { "classes", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    

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

    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------
        

        
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
    
    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_class(WebHtml& h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;
                
            Class::Info i  = info(c);
            dev_title(h, c);
            auto ta = h.table();
            h.kvrow("ID") << c.id;
            h.kvrow("Name") << i.name;
            h.kvrow("Plural") << i.plural;
            h.kvrow("Category") << dev(i.category);
            h.kvrow("Brief") << i.brief;
            h.kvrow("Aliases") << join(cdb::def_alias_set(c), ", ");
            h.kvrow("Prefixes") << join(cdb::def_prefix_set(c), ", ");
            h.kvrow("Reverses") << dev(cdb::def_reverse_set(c));
            h.kvrow("Sources") << dev(cdb::def_source_set(c));
            h.kvrow("Suffixes") << join(cdb::def_suffix_set(c), ", ");
            h.kvrow("Targets") << dev(cdb::def_target_set(c));
            h.kvrow("Uses") << dev(cdb::def_use_set(c));
            h.kvrow("Used") << dev(cdb::def_rev_use_set(c));
            h.kvrow("Binding") << i.binding;
        }
        
        void    p_dev_class_atoms(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Atoms");
            dev_table(h, all_atoms(c, Sorted::YES));
        }
        
        void    p_dev_class_bases(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;
            
            dev_title(h, c, "Bases");
            dev_table(h, base_classes_ranked(c, Sorted::YES), "Hops");
        }
        
        void    p_dev_class_derived(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Derived");
            dev_table(h, derived_classes_ranked(c, Sorted::YES), "Hops");
        }

        void    p_dev_class_fields(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Fields");
            dev_table(h, fields(c, Sorted::YES));
        }

        void    p_dev_class_inbound(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Inbound");
            dev_table(h, inbound_classes_ranked(c, Sorted::YES), "Hops");
        }
        
        void    p_dev_class_outbound(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Outbound");
            dev_table(h, outbound_classes_ranked(c, Sorted::YES), "Hops");
        }

        void    p_dev_class_rev_uses(WebHtml& h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;
                
            dev_title(h, c, "Used By");
            dev_table(h, cdb::def_rev_use(c, Sorted::YES));
            
        }

        void    p_dev_class_reverses(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Reverses");
            dev_table(h, reverse_classes_ranked(c, Sorted::YES), "Hops");
        }
        

        void    p_dev_class_sources(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Sources");
            dev_table(h, source_classes_ranked(c, Sorted::YES), "Hops");
        }

        void    p_dev_class_tags(WebHtml& h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;
                
            dev_title(h, c, "Tags");
            dev_table(h, tags(c, Sorted::YES));
            
        }
        
        void    p_dev_class_targets(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            dev_title(h, c, "Targets");
            dev_table(h, target_classes_ranked(c, Sorted::YES), "Hops");
        }

        
        void    p_dev_class_uses(WebHtml& h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;
                
            dev_title(h, c, "Uses");
            dev_table(h, def_use(c, Sorted::YES));
            
        }
        
        /*
        void    p_dev_class_def_fields(WebHtml&h)
        {
            Class   c   = class_(h);
            if(!c)
                throw HttpStatus::BadArgument;

            h.title() << "Class (" << label(c) << "): Def Fields";
            dev_table(h, def_fields(c, Sorted::YES));
        }
        */

        void    p_dev_classes(WebHtml&h)
        {
            h.title() << "All Classes";
            dev_table(h, all_classes(Sorted::YES));
        }
    
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_class_pages()
        {
            reg_webpage<p_admin_class>("/admin/class").argument("id", "Class ID");
            // reg_webpage<p_admin_classes>("/admin/classes"); // registered in page.cpp
            reg_webpage<p_admin_classes_create>(hPost, "/admin/classes/create");

            reg_webpage<p_api_class>("/api/class");
            reg_webpage<p_api_class_key>("/api/class/key");
            reg_webpage<p_api_classes>("/api/classes")
                .argument("category", "Specify category")
                .description("Lists out classes according to search criteria, or ALL classes by default")
            ;

            reg_webgroup({
                reg_webpage<p_class>("/class").argument("ID", "Class ID").label("Overview"),
                reg_webpage<p_class_atoms>("/class/atoms").argument("ID", "Class ID").label("Atoms")
            });


            reg_webgroup({
                reg_webpage<p_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
                reg_webpage<p_dev_class_bases>("/dev/class/base").argument("id", "Class ID").label("Base"),
                //reg_webpage<p_dev_class_uses>("/dev/class/uses").argument("id", "Class ID").label("Uses"),
                //reg_webpage<p_dev_class_rev_uses>("/dev/class/used").argument("id", "Class ID").label("Used"),
                reg_webpage<p_dev_class_derived>("/dev/class/derived").argument("id", "Class ID").label("Derived"),
                reg_webpage<p_dev_class_sources>("/dev/class/sources").argument("id", "Class ID").label("Sources"),
                reg_webpage<p_dev_class_targets>("/dev/class/targets").argument("id", "Class ID").label("Targets"),
                reg_webpage<p_dev_class_inbound>("/dev/class/inbound").argument("id", "Class ID").label("Inbound"),
                reg_webpage<p_dev_class_outbound>("/dev/class/outbound").argument("id", "Class ID").label("Outbound"),
                reg_webpage<p_dev_class_reverses>("/dev/class/reverses").argument("id", "Class ID").label("Reverses"),
                reg_webpage<p_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags"),
                reg_webpage<p_dev_class_fields>("/dev/class/fields").argument("id", "Class ID").label("Fields"),
                //reg_webpage<p_dev_class_def_fields>("/dev/class/def_fields").argument("id", "Class ID").label("Def Fields"),
                reg_webpage<p_dev_class_atoms>("/dev/class/atoms").argument("id", "Class ID").label("Atoms")
            });
            reg_webpage<p_dev_classes>("/dev/classes");
        }
}
