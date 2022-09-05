////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_root(WebHtml& h)
    {
        const Root* rt   = root(h);
        if(!rt)
            rt          = wksp::root(0);
        
        h.title() << "Root (" << rt->name << ")";
        
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
    
    void    page_dev_root_all_directories(WebHtml& h)
    {
        const Root* rt   = root(h);
        if(!rt)
            rt          = wksp::root(0);

        h.title() << "Root (" << rt->name << "): All Directories";
        dev_table(h, all_directories(rt, Sorted::YES));
    }
    
    void    page_dev_root_all_fragments(WebHtml& h)
    {
        const Root* rt  = root(h);
        if(!rt)
            rt          = wksp::root(0);
            
        h.title() << "Root (" << rt->name << "): All Fragments";
        dev_table(h, all_fragments(rt, Sorted::YES));
    }

    void    page_dev_root_directories(WebHtml& h)
    {
        const Root* rt   = root(h);
        if(!rt)
            rt          = wksp::root(0);
            
        h.title() << "Root (" << rt->name << "): Directories";
        
        dev_table(h, directories(rt, Sorted::YES));
    }

    void    page_dev_root_fragments(WebHtml& h)
    {
        const Root* rt   = root(h);
        if(!rt)
            rt          = wksp::root(0);

        h.title() << "Root (" << rt->name << "): Fragments";

        dev_table(h, fragments(rt, Sorted::YES));
    }
    
    void    page_dev_roots(WebHtml& h)
    {
        h.title() << "All Roots";
        dev_table(h, wksp::roots());
    }
    
    void    reg_dev_root()
    {
        reg_webgroup({
            reg_webpage<page_dev_root>("/dev/root").argument("id", "Root ID").label("Info"),
            reg_webpage<page_dev_root_directories>("/dev/root/dirs").argument("id", "Root ID").label("Dirs"),
            reg_webpage<page_dev_root_fragments>("/dev/root/frags").argument("id", "Root ID").label("Frags"),
            reg_webpage<page_dev_root_all_directories>("/dev/root/all_dirs").argument("id", "Root ID").label("AllDirs"),
            reg_webpage<page_dev_root_all_fragments>("/dev/root/all_frags").argument("id", "Root ID").label("AllFrags")
        });
        reg_webpage<page_dev_roots>("/dev/roots");
    }
}
