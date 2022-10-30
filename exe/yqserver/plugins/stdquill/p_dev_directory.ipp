////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    dev_title(WebHtml& h, Directory x, std::string_view extra=std::string_view())
    {
        auto t = h.title();
        const Root* rt  = root(x);
        
        h << "Directory ";
        if(rt)
            h << '[' << rt->key << ']';
        h << '(' << key(x) << ')';
        if(!extra.empty()){
            h << ": " << extra;
        }
    }

    void    p_dev_directories(WebHtml& h)
    {
        h.title("All Directories");
        dev_table(h, all_directories(Sorted::YES));
    }
    
    void    p_dev_directory(WebHtml& h)
    {
        Directory   x   = directory(h);
        if(!x)
            throw HttpStatus::BadArgument;
        Directory   p   = parent(x);
        Folder      f   = folder(x);
        const Root* rt  = root(x);
        
        dev_title(h, x);
        auto ta = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Key") << key(x);
        h.kvrow("Folder") << dev(f);
        h.kvrow("Hidden") << hidden(x);
        if(p)
            h.kvrow("Parent") << dev(p);
        if(rt)
            h.kvrow("Root") << dev(rt);
    }
    
    void    p_dev_directory_fragments(WebHtml& h)
    {
        Directory   x   = directory(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Fragments");
        dev_table(h, child_fragments(x, Sorted::YES));
    }

    void    p_dev_directory_subdirs(WebHtml& h)
    {
        Directory   x   = directory(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Subdirectories");
        dev_table(h, child_directories(x, Sorted::YES));
    }
    
    void    reg_dev_directory()
    {
        reg_webpage<p_dev_directories>("/dev/directories");
        reg_webgroup({
            reg_webpage<p_dev_directory>("/dev/directory").argument("id", "Directory ID").label("Info"),
            reg_webpage<p_dev_directory_fragments>("/dev/directory/fragments").argument("id", "Directory ID").label("Frags"),
            reg_webpage<p_dev_directory_subdirs>("/dev/directory/subdirs").argument("id", "Directory ID").label("Subdirs")
        });
    }
}
