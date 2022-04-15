////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <yq/file/arg.hpp>

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
    auto _tab = html::table(h);
        h << "<tr><th>ID</th><th>Fragments</th><th>Children</th><th>Path</th></tr>\n";
    for(Directory d : dirs){
        h << "<tr><td>" << dev_id(d) << "</td><td>" 
            << cdb::child_fragments_count(d) << "</td><td>" << cdb::child_directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
    }
}

void    dev_table(WebHtml& h, const std::vector<Document>& documents)
{
    auto _tab = html::table(h);
    h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
    for(Document a : documents){
        auto i = cdb::info(a);
        h << "<tr><td>" << dev_id(a)  << "</td><td>" << cdb::fragments_count(a) 
          << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
    }
}

void    dev_table(WebHtml& h, const std::vector<Fragment>& fragments)
{
    auto _tab = html::table(h);
    h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Path</th>\n";
    for(Fragment f : fragments){
        auto i = cdb::info(f);
        h << "<tr><td>" << dev_id(f) << "</td><td>" << i.name 
            << "</td><td>" << i.size << "</td><td>" << i.path << "</td></tr>\n";
    }
}

void    dev_table(WebHtml& h, const std::vector<Folder>&folders)
{
    auto _tab = html::table(h);
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
    auto _tab = html::table(h);
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
        StringMultiMap  args    = h.context().decode_query();
        Fragment x_fragment{to_uint64(args.first("id"))};
        if(!x_fragment)
            throw HttpStatus::NotAcceptable;

        auto i = cdb::info(x_fragment);
        h.title("Fragment (" + i.path.string() + ")");

        auto _t = html::table(h);
        html::kvrow(h, "ID") << x_fragment.id;
        html::kvrow(h, "Document") << dev(i.document);
        html::kvrow(h, "Directory") << dev(i.directory);
        html::kvrow(h, "Folder") << dev(i.folder);
        html::kvrow(h, "Hidden") << i.hidden;
        html::kvrow(h, "Modified") << i.modified << " ns.";
        html::kvrow(h, "Name") << i.name;
        html::kvrow(h, "Path") << i.path;
        html::kvrow(h, "Removed") << i.removed;
        html::kvrow(h, "Rescan") << i.rescan;
        html::kvrow(h, "Root") << dev(i.root);
        html::kvrow(h, "Size") << i.size;
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
        
        std::string t   = "Root (";
        t += rt->name;
        t += ')';
        h.title(t);
        
        auto tab   = html::table(h);
        html::kvrow(h, "ID") << rt->id;
        html::kvrow(h, "Depth") << rt -> depth;
        html::kvrow(h, "Key") << rt -> key;
        html::kvrow(h, "Name") << rt -> name;
        html::kvrow(h, "Path") << rt -> path;
        html::kvrow(h, "Template") << rt -> is_template;
        html::kvrow(h, "Total Directories") << cdb::all_directories_count(rt);
        html::kvrow(h, "Total Fragments") << cdb::all_fragments_count(rt);
    }
    
    
    void    page_dev_root_all_directories(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);

        std::string t   = "Root (";
        t += rt->name;
        t += "): All Directories";
        h.title(t);
        
        dev_table(h, cdb::all_directories(rt, Sorted::YES));
    }
    
    void    page_dev_root_all_fragments(WebHtml& h)
    {
        const Root* rt  = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);
            
        std::string t   = "Root (";
        t += rt->name;
        t += "): All Fragments";
        h.title(t);
        
        dev_table(h, cdb::all_fragments(rt, Sorted::YES));
    }

    void    page_dev_root_directories(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);

        std::string t   = "Root (";
        t += rt->name;
        t += "): Directories";
        h.title(t);
        
        dev_table(h, cdb::directories(rt, Sorted::YES));
    }

    void    page_dev_root_fragments(WebHtml& h)
    {
        const Root* rt   = arg::root(h);
        if(!rt)
            rt          = wksp::root(0);

        std::string t   = "Root (";
        t += rt->name;
        t += "): Fragments";
        h.title(t);
        
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
