////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <db/directory_arg.hpp>
#include <db/document_arg.hpp>
#include <db/folder_arg.hpp>
#include <db/fragment_arg.hpp>
#include <db/root_arg.hpp>

using namespace yq;

WebHtml&    operator<<(WebHtml&h, const Dev<Directory>&v)
{
    std::string n       = cdb::name(v.data);
    if(n.empty())
        n   = "(no-name)";
    if(v.data)
        h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
    h << "[" << v.data.id << "] " << n;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const Dev<Document>&v)
{
    std::string n       = cdb::name(v.data);
    if(n.empty())
        n   = "(no-name)";
    if(v.data)
        h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
    h << "[" << v.data.id << "] " << n;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const Dev<Folder>&v)
{
    std::string n       = cdb::name(v.data);
    if(n.empty())
        n   = "(no-name)";
    if(v.data)
        h << "<a href=\"/dev/folder?id=" << v.data.id << "\">";
    h << "[" << v.data.id << "] " << n;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const Dev<Fragment>&v)
{
    std::string n       = cdb::name(v.data);
    if(n.empty())
        n   = "(no-name)";
    if(v.data)
        h << "<a href=\"/dev/fragment?id=" << v.data.id << "\">";
    h << "[" << v.data.id << "] " << n;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const Dev<const Root*>&v)
{
    if(!v.data){
        h << "(null root)";
        return h;
    }
    
    h << "<a href=\"/dev/root?id=" << v.data->id << "\">[" << v.data->id << "] " << v.data->key << "</a>";
    return h;
}


WebHtml&    operator<<(WebHtml&h, const DevID<Directory>&v)
{
    if(v.data)
        h << "<a href=\"/dev/directory?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const DevID<Document>&v)
{
    if(v.data)
        h << "<a href=\"/dev/document?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const DevID<Folder>&v)
{
    if(v.data)
        h << "<a href=\"/dev/folder?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml&h, const DevID<Fragment>&v)
{
    if(v.data)
        h << "<a href=\"/dev/fragment?id=" << v.data.id << "\">";
    h << v.data.id;
    if(v.data)
        h << "</a>";
    return h;
}

WebHtml&    operator<<(WebHtml& h, const DevID<const Root*>&v)
{
    if(!v.data){
        h << "(null root)";
        return h;
    }
    
    h << "<a href=\"/dev/root?id=" << v.data->id << "\">" << v.data->id << "</a>";
    return h;
}


void    dev_table(WebHtml& h, const std::vector<Directory>& dirs)
{
    auto _tab = h.table();
        h << "<tr><th>ID</th><th>Fragments</th><th>Children</th><th>Path</th></tr>\n";
    for(Directory d : dirs){
        h << "<tr><td>" << dev_id(d) << "</td><td>" 
            << cdb::child_fragments_count(d) << "</td><td>" << cdb::child_directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
    }
}

void    dev_table(WebHtml& h, const std::vector<Document>& documents)
{
    auto _tab = h.table();
    h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
    for(Document a : documents){
        auto i = cdb::info(a);
        h << "<tr><td>" << dev_id(a)  << "</td><td>" << cdb::fragments_count(a) 
          << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
    }
}

void    dev_table(WebHtml& h, const std::vector<Fragment>& fragments)
{
    auto _tab = h.table();
    h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Path</th>\n";
    for(Fragment f : fragments){
        auto i = cdb::info(f);
        h << "<tr><td>" << dev_id(f) << "</td><td>" << i.name 
            << "</td><td>" << i.size << "</td><td>" << i.path << "</td></tr>\n";
    }
}

void    dev_table(WebHtml& h, const std::vector<Folder>&folders)
{
    auto _tab = h.table();
    h << "<tr><th><ID></th><th>key</th><th>Name</th><th>Documents</th><th>Children</th><th>Brief</th></tr>\n";
    for(Folder f : folders){
        auto i = cdb::info(f);
        h << "<tr><td>" << dev_id(f) << "</td><td>" << i.key
          << "</td><td>" << i.name << "</td><td>" << cdb::documents_count(f) << "</td><td>" << cdb::folders_count(f) 
          << "</td><td>" << i.brief << "</td</tr>\n";
    }
}

void    dev_table(WebHtml&h, const std::vector<const Root*>& roots)
{
    auto _tab = h.table();
    h << "<tr><th><ID></th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
    for(const Root* r : roots){
        if(!r)
            continue;
        h << "<tr><td>" << dev_id(r) <<  "</td><td>" << r->key << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


namespace {

    void    page_dev_dirs(WebHtml& h)
    {
        h.title("Listing of Directories");
        dev_table(h, cdb::all_directories(Sorted::YES));
    }

    void    page_dev_docs(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, cdb::all_documents(Sorted::YES));
    }
    
    void    page_dev_fragment(WebHtml&h)
    {
        Fragment    x_fragment = arg::fragment(h);
        if(!x_fragment)
            throw HttpStatus::NotAcceptable;

        auto i = cdb::info(x_fragment);
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
        dev_table(h, cdb::all_fragments(Sorted::YES));
    }

    void    page_dev_folders(WebHtml& h)
    {
        h.title("Listing of Folders");
        dev_table(h, cdb::all_folders(Sorted::YES));
    }
    
    void    page_dev_root(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
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
        h.kvrow("Total Directories") << cdb::all_directories_count(rt);
        h.kvrow("Total Fragments") << cdb::all_fragments_count(rt);
    }
    
    
    void    page_dev_root_all_directories(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);

        h.title() << "Root (" << rt->name << "): All Directories";
        dev_table(h, cdb::all_directories(rt, Sorted::YES));
    }
    
    void    page_dev_root_all_fragments(WebHtml& h)
    {
        const Root* rt  = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);
            
        h.title() << "Root (" << rt->name << "): All Fragments";
        dev_table(h, cdb::all_fragments(rt, Sorted::YES));
    }

    void    page_dev_root_directories(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);
            
        h.title() << "Root (" << rt->name << "): Directories";
        
        dev_table(h, cdb::directories(rt, Sorted::YES));
    }

    void    page_dev_root_fragments(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);

        h.title() << "Root (" << rt->name << "): Fragments";

        dev_table(h, cdb::fragments(rt, Sorted::YES));
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
