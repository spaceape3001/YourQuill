////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void    page_dev_folder(WebHtml&h)
    {
        bool    detected;
        Folder  x   = folder(h, &detected);
        if(!x){
            if(detected)
                throw HttpStatus::BadArgument;
            //  redirect...once we get a streaming API to it
            x       = top_folder();
        }

        
        auto i = info(x);
        h.title() << "Folder (" << i.key << ")";
        auto t = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Key") << "'" << i.key << "'";
        h.kvrow("Parent") << dev(i.parent);
        h.kvrow("Hidden") << i.hidden;
        h.kvrow("Removed") << i.removed;
        h.kvrow("Sub-Key") << i.skey;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Folders") << child_folders_count(x);
        h.kvrow("Documents") << child_documents_count(x);
    }
    
    void    page_dev_folder_directories(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        h.title() << "Folder (" << key(x) << "): Directories";
        dev_table(h, directories(x));
    }

    void    page_dev_folder_documents(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Folder (" << key(x) << "): Documents";
        dev_table(h, child_documents(x));
    }

    void    page_dev_folder_fragments(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Folder (" << key(x) << "): Fragments";
        dev_table(h, child_fragments(x));
    }

    void    page_dev_folder_subs(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Folder (" << key(x) << "): Sub-Folders";
        dev_table(h, child_folders(x));
    }

    void    page_dev_folders(WebHtml& h)
    {
        h.title("Listing of Folders");
        dev_table(h, all_folders(Sorted::YES));
    }
    
    void    reg_dev_folder()
    {
        reg_webgroup({
            reg_webpage<page_dev_folder>("/dev/folder").label("Info"),
            reg_webpage<page_dev_folder_directories>("/dev/folder/dirs").label("Dirs"),
            reg_webpage<page_dev_folder_documents>("/dev/folder/docs").label("Docs"),
            reg_webpage<page_dev_folder_fragments>("/dev/folder/frags").label("Frags"),
            reg_webpage<page_dev_folder_subs>("/dev/folder/subs").label("Subs")
        });
        reg_webpage<page_dev_folders>("/dev/folders");
    }
}
