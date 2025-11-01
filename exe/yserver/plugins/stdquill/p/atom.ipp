////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/atom/AtomJson.hpp>
#include <yq/mithril/atom/AtomSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------


    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_atom(WebContext& ctx)
        {
            Atom    v   = arg::atom(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_atom_key(WebContext& ctx)
        {
            Atom    v   = arg::atom(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_atoms(WebContext& ctx)
        {
            AtomVector  ret = search(ctx, ATOM);
            return json{
                { "atoms", json_(ret) }
            };
        }

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
        
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

    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void p_dev_atom(WebHtml& h)
        {
            Atom a = atom(h);
            if(!a)
                throw HttpStatus::BadArgument;
            
            Atom::Info  i   = info(a);
            
            dev_title(h, a);

            auto ta = h.table();
            h.kvrow("ID") << a.id;
            h.kvrow("Name") << i.name;
            h.kvrow("Key") << i.key;
            h.kvrow("Abbreviation") << i.abbr;
            h.kvrow("Brief") << i.brief;
            h.kvrow("Leaf") << dev(i.leaf);
            h.kvrow("Parent") << dev(i.parent);
        }
        
        void p_dev_atom_attributes(WebHtml& h)
        {
            Atom a = atom(h);
            if(!a)
                throw HttpStatus::BadArgument;
            dev_title(h, a, "Attributes");
            dev_table(h, attributes(a, Sorted::YES));
        }
        
        void p_dev_atom_children(WebHtml& h)
        {
            Atom a = atom(h);
            if(!a)
                throw HttpStatus::BadArgument;
            dev_title(h, a, "Children (Atoms)");
            dev_table(h, children(a, Sorted::YES));
        }

        void p_dev_atom_classes(WebHtml&h)
        {
            Atom a = atom(h);
            if(!a)
                throw HttpStatus::BadArgument;
            dev_title(h, a, "Classes");
            dev_table(h, classes(a, Sorted::YES));
        }
        
        //void p_dev_atom_documents(WebHtml& h)
        //{
            //Atom a = atom(h);
            //if(!a)
                //throw HttpStatus::BadArgument;
            //h.title() << "Atom (" << label(a) << "): Documents";
            //dev_table(h, documents(a, Sorted::YES));
        //}

        
        void p_dev_atom_tags(WebHtml& h)
        {
            Atom a = atom(h);
            if(!a)
                throw HttpStatus::BadArgument;
            dev_title(h, a, "Tags");
            dev_table(h, tags(a, Sorted::YES));
        }

        void p_dev_atoms(WebHtml&h)
        {
            h.title() << "All Atoms";
            dev_table(h, all_atoms(Sorted::YES));
        }
    
    //  -----------------------------------------------------------------------

        void p_dev_atom_property(WebHtml& h)
        {
            Atom::Property p = atom_property(h);
            if(!p)
                throw HttpStatus::BadArgument;
            h.title() << "Atom Property (" << p.id << ")";
            auto ta = h.table();
            auto i = info(p);
            h.kvrow("ID") << p.id;
            h.kvrow("Atom") << dev(i.atom);
            h.kvrow("Attribute") << dev(i.attr);
            h.kvrow("Field") << dev(i.field);
            h.kvrow("Source") << dev(i.source);
            h.kvrow("Target") << dev(i.target);
            h.kvrow("Child") << dev(i.child);
        }

        void p_dev_atom_properties(WebHtml& h)
        {
            Atom a = atom(h);
            if(!a)
                throw HttpStatus::BadArgument;
            h.title() << "Atom (" << label(a) << "): Properties";
            dev_table(h, properties(a, Sorted::YES));
        }

    
    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------
    
        void reg_atom_pages()
        {
            reg_webpage<p_api_atom>("/api/atom").argument("ID", "Atom ID");
            reg_webpage<p_api_atom_key>("/api/atom/key").argument("ID", "Atom ID");
            reg_webpage<p_api_atoms>("/api/atoms")
                .argument("class", "Specify class")
                .argument("tag", "Specify tag")
                .description("List of atoms meeting search criteria, or ALL atoms (default)")
            ;
        
            reg_webpage<p_atom>("/atom").argument("ID", "Atom ID");

            reg_webtemplate("/atoms", wksp::shared("www/atoms.ht"sv));
        
            reg_webgroup({
                reg_webpage<p_dev_atom>("/dev/atom").argument("id", "Atom ID").label("Info"),
                reg_webpage<p_dev_atom_attributes>("/dev/atom/attributes").argument("id", "Atom ID").label("Attributes"),
                reg_webpage<p_dev_atom_children>("/dev/atom/children").argument("id", "Atom ID").label("Children"),
                reg_webpage<p_dev_atom_classes>("/dev/atom/classes").argument("id", "Atom ID").label("Classes"),
                //reg_webpage<p_dev_atom_documents>("/dev/atom/documents").argument("id", "Atom ID").label("Docs"),
                reg_webpage<p_dev_atom_properties>("/dev/atom/properties").argument("id", "Atom ID").label("Properties"),
                reg_webpage<p_dev_atom_tags>("/dev/atom/tags").argument("id", "Atom ID").label("Tags")
            });
            reg_webpage<p_dev_atom_property>("/dev/atom_property").argument("id", "Atom Property ID");
            reg_webpage<p_dev_atoms>("/dev/atoms");
        }
}
