////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <db/filesys_arg.hpp>
#include <db/filesys_html.hpp>

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace {

    void    page_dev_dirs(WebHtml& h)
    {
        h.title("Listing of Directories");
        dev_table(h, all_directories(Sorted::YES));
    }

    void    page_dev_docs(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, all_documents(Sorted::YES));
    }
    
    void    page_dev_fragment(WebHtml&h)
    {
        Fragment    x_fragment = fragment(h);
        if(!x_fragment)
            throw HttpStatus::NotAcceptable;

        auto i = info(x_fragment);
        h.title() << "Fragment (" << i.path.string() << ")";

        auto _t = h.table();
        h.kvrow("ID") << x_fragment.id;
        h.kvrow("Document") << dev(i.document);
        h.kvrow("Directory") << dev(i.directory);
        h.kvrow("Folder") << dev(i.folder);
        h.kvrow("Hidden") << i.hidden;
        h.kvrow("Modified") << i.modified << " ns.";
        h.kvrow("Name") << i.name;
        h.kvrow("Path") << i.path;
        h.kvrow("Removed") << i.removed;
        h.kvrow("Rescan") << i.rescan;
        h.kvrow("Root") << dev(i.root);
        h.kvrow("Size") << i.size;
    }

    void    page_dev_frags(WebHtml& h)
    {
        h.title("Listing of Fragments");
        dev_table(h, all_fragments(Sorted::YES));
    }

    void    page_dev_folders(WebHtml& h)
    {
        h.title("Listing of Folders");
        dev_table(h, all_folders(Sorted::YES));
    }
    
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
 
    void    reg_me()
    {
        reg_webpage<page_dev_dirs>("/dev/directories");
        reg_webpage<page_dev_docs>("/dev/documents");
        reg_webpage<page_dev_folders>("/dev/folders");
        reg_webpage<page_dev_frags>("/dev/fragments");
        reg_webpage<page_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID");
        
        reg_webgroup({
            reg_webpage<page_dev_root>("/dev/root").argument("id", "Root ID").label("Info"),
            reg_webpage<page_dev_root_directories>("/dev/root/dirs").argument("id", "Root ID").label("Dirs"),
            reg_webpage<page_dev_root_fragments>("/dev/root/frags").argument("id", "Root ID").label("Frags"),
            reg_webpage<page_dev_root_all_directories>("/dev/root/all_dirs").argument("id", "Root ID").label("AllDirs"),
            reg_webpage<page_dev_root_all_fragments>("/dev/root/all_frags").argument("id", "Root ID").label("AllFrags")
        });
    }
    
    
}

YQ_INVOKE( reg_me(); )
