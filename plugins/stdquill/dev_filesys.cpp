////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

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


namespace {
    void    dev_table(WebHtml& h, const std::vector<Directory>& dirs)
    {
        auto _tab = html::table(h);
        h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
        for(Directory d : dirs){
            h << "<tr><td>" << dev_id(d) << "</td><td>" 
                << cdb::fragments_count(d) << "</td><td>" << cdb::directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
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
}

YQ_INVOKE(
    reg_webpage<page_dev_dirs>("/dev/directories");
    reg_webpage<page_dev_docs>("/dev/documents");
    reg_webpage<page_dev_folders>("/dev/folders");
    reg_webpage<page_dev_frags>("/dev/fragments");
    reg_webpage<page_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID");
)
