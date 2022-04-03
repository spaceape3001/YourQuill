////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

using namespace yq;

namespace {
    void    dev_table(WebHtml& h, const std::vector<Directory>& dirs)
    {
        auto _tab = html::table(h);
        h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
        for(Directory d : dirs){
            h << "<tr><td><a href=\"/dev/directory?id=" << d.id << "\">" << d.id << "</a></td><td>" 
                << cdb::fragments_count(d) << "</td><td>" << cdb::directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
        }
    }
    
    void    dev_table(WebHtml& h, const std::vector<Document>& documents)
    {
        auto _tab = html::table(h);
        h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
        for(Document a : documents){
            auto i = cdb::info(a);
            h << "<tr><td><a href=\"/dev/document?id=" << a.id << "\">" << a.id << "</a></td><td>" << cdb::fragments_count(a) 
              << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
        }
    }

    void    dev_table(WebHtml& h, const std::vector<Fragment>& fragments)
    {
        auto _tab = html::table(h);
        h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Path</th>\n";
        for(Fragment f : fragments){
            auto i = cdb::info(f);
            h << "<tr><td><a href=\"/dev/fragment?id=" << f.id << "\">" << f.id << "</td><td>" << i.name 
                << "</td><td>" << i.size << "</td><td>" << i.path << "</td></tr>\n";
        }
    }
    
    void    dev_table(WebHtml& h, const std::vector<Folder>&folders)
    {
        auto _tab = html::table(h);
        h << "<tr><th><ID></th><th>key</th><th>Name</th><th>Documents</th><th>Children</th><th>Brief</th></tr>\n";
        for(Folder f : folders){
            auto i = cdb::info(f);
            h << "<tr><td><a href=\"/dev/folder?id=" << f.id << "\">" << f.id << "</a></td><td>" << i.key
              << "</td><td>" << i.name << "</td><td>" << cdb::documents_count(f) << "</td><td>" << cdb::folders_count(f) 
              << "</td><td>" << i.brief << "</td</tr>\n";
        }
    }
    
    void    dev_dirs(WebHtml& h)
    {
        h.title("Listing of Directories");
        dev_table(h, cdb::all_directories(Sorted::YES));
    }

    void    dev_docs(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, cdb::all_documents(Sorted::YES));
    }

    void    dev_frags(WebHtml& h)
    {
        h.title("Listing of Fragments");
        dev_table(h, cdb::all_fragments(Sorted::YES));
    }

    void    dev_folders(WebHtml& h)
    {
        h.title("Listing of Folders");
        dev_table(h, cdb::all_folders(Sorted::YES));
    }
}

YQ_INVOKE(
    reg_webpage<dev_dirs>("/dev/directories");
    reg_webpage<dev_docs>("/dev/documents");
    reg_webpage<dev_folders>("/dev/folders");
    reg_webpage<dev_frags>("/dev/fragments");
)
