////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

        void    dev_title(WebHtml& h, const RootDir* rt, std::string_view extra=std::string_view())
        {
            auto t = h.title();
            h << "Root \"" << rt->name << " (" << rt->id << " &mdash; " << rt->path  << ")";
            if(!extra.empty())
                h << ": " << extra;
        }
    
    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_root(WebHtml& h)
        {
            const RootDir* rt   = root_dir(h);
            if(!rt)
                rt          = wksp::root_dir(0);
            
            
            dev_title(h, rt);
            
            auto tab   = h.table();
            h.kvrow("ID") << rt->id;
            h.kvrow("Depth") << rt -> depth;
            h.kvrow("Key") << rt -> key;
            h.kvrow("Name") << rt -> name;
            h.kvrow("Path") << rt -> path;
            h.kvrow("Template") << rt -> is_template;
            h.kvrow("Total Directories") << all_directories_count(rt);
            h.kvrow("Total Fragments") << all_fragments_count(rt);
            h <<"<tr><td colspan=\"2\"><hr></td></tr>\n";
            
            for(DataRole dr : DataRole::all_values()){
                std::string n   = "Policy ";
                n += dr.key();
                h.kvrow(n) << rt->policy(dr).key();
            }
            
        }
        
        void    p_dev_root_all_directories(WebHtml& h)
        {
            const RootDir* rt   = root_dir(h);
            if(!rt)
                rt          = wksp::root_dir(0);

            dev_title(h, rt, "All Directories");
            dev_table(h, all_directories(rt, Sorted::YES));
        }
        
        void    p_dev_root_all_fragments(WebHtml& h)
        {
            const RootDir* rt  = root_dir(h);
            if(!rt)
                rt          = wksp::root_dir(0);
                
            dev_title(h, rt, "All Fragments");
            dev_table(h, all_fragments(rt, Sorted::YES));
        }

        void    p_dev_root_directories(WebHtml& h)
        {
            const RootDir* rt   = root_dir(h);
            if(!rt)
                rt          = wksp::root_dir(0);
                
            dev_title(h, rt, "Directories");
            dev_table(h, directories(rt, Sorted::YES));
        }

        void    p_dev_root_fragments(WebHtml& h)
        {
            const RootDir* rt   = root_dir(h);
            if(!rt)
                rt          = wksp::root_dir(0);

            dev_title(h, rt, "All Fragments");
            dev_table(h, fragments(rt, Sorted::YES));
        }
        
        void    p_dev_roots(WebHtml& h)
        {
            h.title() << "All Roots";
            dev_table(h, wksp::root_dirs());
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_root_pages()
        {
            reg_webgroup({
                reg_webpage<p_dev_root>("/dev/root").argument("id", "RootDir ID").label("Info"),
                reg_webpage<p_dev_root_directories>("/dev/root/dirs").argument("id", "RootDir ID").label("Dirs"),
                reg_webpage<p_dev_root_fragments>("/dev/root/frags").argument("id", "RootDir ID").label("Frags"),
                reg_webpage<p_dev_root_all_directories>("/dev/root/all_dirs").argument("id", "RootDir ID").label("AllDirs"),
                reg_webpage<p_dev_root_all_fragments>("/dev/root/all_frags").argument("id", "RootDir ID").label("AllFrags")
            });
            reg_webpage<p_dev_roots>("/dev/roots");
        }
}
