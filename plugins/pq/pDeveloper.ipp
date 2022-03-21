////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#pragma once

namespace {
    void    _atoms(HtmlWriter& h, const std::vector<Atom>& atoms)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
        for(Atom a : atoms){
            auto i = cdb::info(a);
            h << "<tr><td><a href=\"/dev/atom?id="<< a.id << "\">" << a.id << "</a></td><td>" 
                << i.key << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }

    void    _classes(HtmlWriter& h, const std::vector<Class>& classes)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
        for(Class c : classes){
            auto i = cdb::info(c);
            h << "<tr><td><a href=\"/dev/class?id="<< c.id << "\">" << c.id << "</a></td><td>" 
                << i.key << "</td><td>" << i.name << "</td><td>" << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }
    
    void    _documents(HtmlWriter& h, const std::vector<Document>& documents)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Fragments</th><th>Key</th><th>Name</th><th>Suffix</th>\n";
        for(Document a : documents){
            auto i = cdb::info(a);
            h << "<tr><td><a href=\"/dev/document?id=" << a.id << "\">" << a.id << "</a></td><td>" << cdb::fragments_count(a) 
              << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" << i.suffix << "</td></tr>\n";
        }
    }
    
    void    _directories(HtmlWriter&h, const std::vector<Directory>& dirs)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Fragments</th><th>Children</th><th>Path</th></tr>\n";
        for(Directory d : dirs)
            h << "<tr><td><a href=\"/dev/directory?id=" << d.id << "\">" << d.id << "</a></td><td>" 
                << cdb::fragments_count(d) << "</td><td>" << cdb::directories_count(d) << "</td><td>" << cdb::path(d) << "</td></tr>\n";
    }
    
    void    _fields(HtmlWriter& h, const std::vector<Field>&  fields)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Class</th><th>Key</th><th>Brief</th>\n";
        for(Field f : fields)
            h << "<tr><td>" << f.id << "</td><td>" << dev(cdb::class_(f)) << "</td><td>" << cdb::key(f) <<"</td><td>" 
                << cdb::brief(f) <<"</td></tr>\n";
    }

    void    _fragments(HtmlWriter& h, const std::vector<Fragment>& fragments)
    {
        auto t = h.table();
        h << "<tr><th>ID</th><th>Name</th><th>Size</th><th>Path</th>\n";
        for(Fragment f : fragments){
            auto i = cdb::info(f);
            h << "<tr><td><a href=\"/dev/fragment?id=" << f.id << "\">" << f.id << "</td><td>" << i.name 
                << "</td><td>" << i.size << "</td><td>" << i.path << "</td></tr>\n";
        }
    }
    
    void    _folders(HtmlWriter& h, const std::vector<Folder>&folders)
    {
        auto t = h.table();
        h << "<tr><th><ID></th><th>key</th><th>Name</th><th>Documents</th><th>Children</th><th>Brief</th></tr>\n";
        for(Folder f : folders){
            auto i = cdb::info(f);
            h << "<tr><td><a href=\"/dev/folder?id=" << f.id << "\">" << f.id << "</a></td><td>" << i.key
              << "</td><td>" << i.name << "</td><td>" << cdb::documents_count(f) << "</td><td>" << cdb::folders_count(f) 
              << "</td><td>" << i.brief << "</td</tr>\n";
        }
    }
    
    void    _leafs(HtmlWriter& h, const std::vector<Leaf>&leafs)
    {
        auto t=h.table();
        h << "<tr><th><ID></th><th>key</th><th>Title</th></tr>\n";
        for(Leaf l : leafs){
            auto    i   = cdb::info(l);
            h << "<tr><td><a href=\"/dev/leaf?id="<< l.id << "\">" << l.id << "</a></td><td><a href=\"/leaf?id=" << l.id << "\">"
                << i.key << "<a></td><td>" << i.title << "</td></tr>\n";
        }
    }
    
    
    void    _roots(HtmlWriter& h, const std::vector<const Root*>&roots)
    {
        auto t = h.table();
        h << "<tr><th><ID></th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
        for(const Root* r : roots){
            if(!r)
                continue;
            h << "<tr><td><a href=\"/dev/root?id=" << r->id << "\">" << r->id << "</a></td><td>" << r->key 
                << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
        }
    }
    
    void    _tags(HtmlWriter& h, const std::vector<Tag>&tags)
    {
        auto ta = h.table();
        h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Leaf</th><th>Brief</th></tr>\n";
        for(Tag t : tags){
            auto i  = cdb::info(t);
            h << "<tr><td><a href=\"/dev/tag?id=" << t.id << "\">" << t.id <<"</a></td><td>" << i.key << "</td><td>" << i.name
                << "</td><td>" << dev(i.doc) << "</td><td>" << dev(cdb::leaf(t)) << "</td><td>" << i.brief << "</td></tr>\n";
        }
    }
    
    void    dev_atom(HtmlWriter& h)
    {
        test(decode_atom_prime());
        auto i = cdb::info(x_atom);
        h.title("Atom (" + i.key + ")");
        auto t = h.table();
        h.key("ID") << x_atom.id;
        h.key("Brief") << i.brief;
        //h.key("Leaf") << dev(i.leaf);
        //h.key("Document") << dev(i.doc);
        h.key("Key") << i.key;
    }
    
    void    dev_atoms(HtmlWriter& h)
    {
        h.title("Listing of Atoms");
        _atoms(h, cdb::all_atoms(Sorted::YES));
    }
    
    void    dev_class(HtmlWriter& h)
    {
        test(decode_class_prime());
        auto i = cdb::info(x_class,true);
        h.title("Class: " + i.name);

        h << link("/dev/classes", icon_for("/icon/parent.svg", "Classes")) << link("/dev/classes", QString("Back to Class Listing"));
        
        auto t = h.table();
        h.key("ID") << x_class.id;
        h.key("Brief") << i.brief;
        h.key("Document") << dev(i.doc);
        h.key("Edge?") << i.edge;
        h.key("Key") << i.key;
        h.key("Name") << i.name;
        h.key("Plural") << i.plural;
    }
    
    void    dev_class_reverses(HtmlWriter&h)
    {
        test(decode_class_prime());
        h.title("Class (" + cdb::key(x_class) + ") Reverses");
        _classes(h, cdb::reverses(x_class, Sorted::YES));
    }
    
    void    dev_class_sources(HtmlWriter&h)
    {
        test(decode_class_prime());
        h.title("Class (" + cdb::key(x_class) + ") Sources");
        _classes(h, cdb::sources(x_class, Sorted::YES));
    }

    void    dev_class_targets(HtmlWriter&h)
    {
        test(decode_class_prime());
        h.title("Class (" + cdb::key(x_class) + ") Targets");
        _classes(h, cdb::targets(x_class, Sorted::YES));
    }

    void    dev_class_uses(HtmlWriter&h)
    {
        test(decode_class_prime());
        h.title("Class (" + cdb::key(x_class) + ") Uses");
        _classes(h, cdb::uses(x_class));
    }

    void    dev_class_subs(HtmlWriter&h)
    {
        test(decode_class_prime());
        h.title("Class (" + cdb::key(x_class) + ") SubClasses");
        _classes(h, cdb::dependents(x_class, Sorted::YES));
    }

    void    dev_classes(HtmlWriter& h)
    {
        h.title("Listing of Classes");
        _classes(h, cdb::all_classes(Sorted::YES));
    }
    
    void    dev_db_tables(HtmlWriter& h)
    {
        h.title("Listing of DB Tables");
        h << "<table>\n";
        for(QString s : wksp::cache().tables()){
            h << "</tr><td>" << s << "</td></tr>\n";
        }
        h << "</table>\n";
    }
    

    void    dev_directories(HtmlWriter& h)
    {
        h.title("Listing of Directories");
        _directories(h, cdb::all_directories(Sorted::YES));
    }
    
    void    dev_directory(HtmlWriter& h)
    {
        test(decode_directory_prime());
        auto i = cdb::info(x_directory);
        h.title("Directory (" + i.path.string() + ")");
        auto t = h.table();
        h.key("ID") << x_directory.id;
        h.key("Directories") << cdb::directories_count(x_directory);
        h.key("Fragments") << cdb::fragments_count(x_directory);
        h.key("Folder") << dev(i.folder);
        h.key("Hidden") << i.hidden;
        h.key("Name") << i.name;
        h.key("Parent") << dev(i.parent);
        h.key("Path") << i.path;
        h.key("Root") << dev(i.root);
        h.key("Removed") << i.removed;
    }

    void    dev_directory_children(HtmlWriter& h)
    {
        test(decode_directory_prime());
        h.title("Directory (" + cdb::path(x_directory).string() + "): Directories");
        _directories(h, cdb::directories(x_directory, Sorted::YES));
    }

    void    dev_directory_fragments(HtmlWriter& h)
    {
        test(decode_directory_prime());
        h.title("Directory (" + cdb::path(x_directory).string() + "): Fragments");
        _fragments(h, cdb::fragments(x_directory, Sorted::YES));
    }
    

    
    void    dev_document(HtmlWriter& h)
    {
        test(decode_document_prime());
        x_leaf  = cdb::leaf(x_document);
        auto i = cdb::info(x_document);

        h.title("Document (" + i.key + ")");
        auto t = h.table();
        h.key("ID") << x_document.id;
        h.key("Base") << i.base;
        h.key("Folder") << dev(i.folder);
        h.key("Fragments") << cdb::fragments_count(x_document);
        h.key("Hidden") << i.hidden;
        h.key("Key") << i.key;
        if(x_leaf)
            h.key("Leaf") << dev(x_leaf);
        h.key("Name") << i.name;
        h.key("Removed") << i.removed;
        h.key("Short-Key") << i.skey;
        h.key("Suffix") << i.suffix;
    }
    
    void    dev_document_fragments(HtmlWriter&h)
    {
        test(decode_document_prime());
        h.title("Document (" + cdb::key(x_document) + "): Fragments");
        _fragments(h, cdb::fragments(x_document, Sorted::YES));
    }
    
    void    dev_document_roots(HtmlWriter& h)
    {
        test(decode_document_prime());
        h.title("Document (" + cdb::key(x_document) + "): Roots");
        _roots(h, cdb::roots(x_document));
    }
    
    void    dev_documents(HtmlWriter&h)
    {
        h.title("Listing of Documents");
        _documents(h, cdb::all_documents(Sorted::YES));
    }

    void    _echo(HtmlWriter& h, bool canSubmit)
    {
        h.title("Developer's Echo");
        h << H1("Hello World!");
        
        auto pm = x_request->getParameterMap();
        if(canSubmit){
            String   url = x_request -> getParameter("url");
            if(!url.empty()){
                h << "<form action=\"" << url;
                String  args = x_request -> getParameter("args");
                if(!args.empty())
                    h << "?" << args;
                h << "\" method=\"" << String(x_request->getMethod()) << "\">\n";
                for(auto i = pm.begin(); i!=pm.end(); ++i)
                    h << "<input type=\"hidden\" name=\"" << i.key().constData() << "\" id=\"" << i.key().constData() << "\" value=\"" 
                        << utf8(i.value()).toHtmlEscaped() << "\" >\n";
                h << "<input type=\"submit\" value=\"Forward\">\n"
                  << "</form>\n";
            }
        }
        
        h << H2("Token Parameters");
        {
            auto   t   = h.table();
            t.key("Session ID") << x_session -> id;
            t.key("User") << x_session -> user;
            t.key("Time") << x_time;
            t.key("Path") << x_request -> getPath();
            t.key("Method") << x_request -> getMethod();
            t.key("Version") << x_request -> getVersion();
        }
        
        h << H2("Parameters");
        {
            auto   t   = h.table();
            for(auto i = pm.begin(); i!=pm.end(); ++i)
                t.key( i.key()) << i.value();
        }

        h << H2("Headers");
        {
            auto   t   = h.table();
            auto pm = x_request->getHeaderMap();
            for(auto i = pm.begin(); i!=pm.end(); ++i)
                t.key( i.key()) << i.value();
        }
        
        h << H2("Body");
        h << x_request->getBody();
    }

    void    dev_echo_get(HtmlWriter& h)
    {
        _echo(h, false);
    }
    
    void    dev_echo_post(HtmlWriter&h)
    {
        _echo(h, true);
    }
    
    void    dev_fields(HtmlWriter&h)
    {
        test(decode_class(), true);
        Vector<Field>       fields;
        if(x_class)
            fields  = cdb::fields(x_class, Sorted::YES);
        else
            fields  = cdb::all_fields(Sorted::YES);
        _fields(h, fields);
    }
    
    void    dev_fragment(HtmlWriter&h)
    {
        test(decode_fragment_prime());
        auto i = cdb::info(x_fragment);
        h.title("Fragment (" + i.path.string() + ")");
        h.key("ID") << x_fragment.id;
        h.key("Document") << dev(i.document);
        h.key("Directory") << dev(i.directory);
        h.key("Folder") << dev(i.folder);
        h.key("Hidden") << i.hidden;
        h.key("Modified") << i.modified << " ns.";
        h.key("Name") << i.name;
        h.key("Path") << i.path;
        h.key("Removed") << i.removed;
        h.key("Rescan") << i.rescan;
        h.key("Root") << dev(i.root);
        h.key("Size") << i.size;
    }

    void    dev_fragments(HtmlWriter&h)
    {
        h.title("Listing of Fragments");
        _fragments(h, cdb::all_fragments(Sorted::YES));
    }
    
    void    dev_folder(HtmlWriter&h)
    {
        test(decode_folder_prime());
        auto i = cdb::info(x_folder);
        h.title("Folder (" + i.key + ")");
        auto t = h.table();
        h.key("ID") << x_folder.id;
        h.key("Name") << i.name;
        h.key("Key") << "'" << i.key << "'";
        h.key("Parent") << dev(i.parent);
        h.key("Hidden") << i.hidden;
        h.key("Removed") << i.removed;
        h.key("Sub-Key") << i.skey;
        h.key("Brief") << i.brief;
        h.key("Folders") << cdb::folders_count(x_folder);
        h.key("Documents") << cdb::documents_count(x_folder);
    }
    
    void    dev_folder_documents(HtmlWriter& h)
    {
        test(decode_folder_prime());
        h.title("Folder (" + cdb::key(x_folder) + "): Documents");
        _documents(h, cdb::documents(x_folder, Sorted::YES));
    }

    void    dev_folder_children(HtmlWriter& h)
    {
        test(decode_folder_prime());
        h.title("Folder (" + cdb::key(x_folder) + "): Children");
        _folders(h, cdb::folders(x_folder, Sorted::YES));

    }
    
    void    dev_folder_directories(HtmlWriter& h)
    {
        test(decode_folder_prime());
        h.title("Folder (" + cdb::key(x_folder) + "): Directories");
        _directories(h, cdb::directories(x_folder, Sorted::YES));
    }
    
    void    dev_folder_fragments(HtmlWriter& h)
    {
        test(decode_folder_prime());
        h.title("Folder (" + cdb::key(x_folder) + "): Fragments");
        _fragments(h, cdb::fragments(x_folder, Sorted::YES));
    }
    
    void    dev_folders(HtmlWriter&h)
    {
        h.title("Listing of Folders");
        _folders(h, cdb::all_folders(Sorted::YES));
    }

    void    dev_getters(HtmlWriter&h)
    {
        h.title("Listing of Static Getters");
        auto t = h.table();
        for(auto& ge : Page::static_getters())
            h.key(ge.first.qString()) << "<PRE>" << ge.second() << "</PRE>";
    }
    
    void    dev_globals(HtmlWriter&h)
    {
        h.title("Listing of Globals");
        auto t = h.table();
        h << "<tr><th>ID</th><th>Name</th><th>Value</th><th>Description</th></tr>\n";
        for(const Global*  g : Global::all())
            h << "<tr><td>" << g->id() << "</td><td>" << g->name() << "</td><td>" << g->value().print() << "</td><td>" 
                << g->description() << "</td></tr>\n";
    }
    
    void    dev_leaf(HtmlWriter& h)
    {
        test(decode_leaf_prime());
        auto i = cdb::info(x_leaf, true);
            
        h.title(i.title);
        auto t = h.table();
        h.key("ID") << x_leaf.id;
        h.key("Key") << i.key;
        h.key("Document") << dev(i.doc);
        h.key("Atom") << dev(cdb::atom(x_leaf));
        h.key("Title") << i.title;
    }
    
    void    dev_leafs(HtmlWriter& h)
    {
        h.title("Listing of Leafs");
        _leafs(h, cdb::all_leafs(Sorted::YES));
    }
    
    void    dev_metas(HtmlWriter& h)
    {
        h.title("Listing of Metas");
        auto t = h.table();
        h << "<tr><th>ID</th><th>Name</th><th>Generic</th><th>Description</th></tr>\n";
        for(const Meta* m  : Meta::all())
            h << "<tr><td>" << m->id() << "</td><td>" << m->name() << "</td><td>" << m->generic() << "</td><td>" << m->description() << "</td></tr>\n";
    }

    void    dev_pages(HtmlWriter& h)
    {
        Vector<const Page*>  all = Page::all();
        all.sort(
            [](const Page* a, const Page *b) -> bool
            {
                return is_less( 
                    compare_igCase(a->path(), b->path()),
                    compare(a->httpOp(), b->httpOp())
                );
            }
        );
        
        h.title("All Registered Pages");
        h << "<TABLE>\n";
        h << "<TR><TH>Op</TH><TH>Path</TH><TH>Description</TH></TR>\n";
        for(const Page* p : all){
            h << "<TR><TD>" << p->httpOp() << "</TD><TD>";
            
            bool    link    = (p->httpOp() == hGet) && p->arguments().empty();
            if(link){
                h << "<A HREF=\"" << p->path() << "\">" << p->path() << "</A>";
            } else
                h << p->path();
            h << "</TD><TD>" << p -> description() << "</TD></TR>\n";
        }
        h << "</TABLE>\n";
    }
    
    void    dev_root(HtmlWriter& h)
    {
        test(decode_root_prime());
        h.title("Root (" + x_root -> path.string() + ")");
        auto t   = h.table();
        h.key("ID") << x_root->id;
        h.key("Depth") << x_root -> depth;
        h.key("Key") << x_root -> key;
        h.key("Name") << x_root -> name;
        h.key("Path") << x_root -> path;
        h.key("Template") << x_root -> is_template;
        h.key("Total Directories") << cdb::all_directories_count(x_root);
        h.key("Total Fragments") << cdb::all_fragments_count(x_root);
    }
    
    void    dev_root_all_directories(HtmlWriter&h)
    {
        test(decode_root_prime());
        h.title("Root (" + x_root -> path.string() + "): All Directories" );
        _directories(h, cdb::all_directories(x_root, Sorted::YES));
    }

    void    dev_root_all_fragments(HtmlWriter&h)
    {
        test(decode_root_prime());
        h.title("Root (" + x_root -> path.string() + "): All Fragments" );
        _fragments(h, cdb::all_fragments(x_root, Sorted::YES));
    }
    
    void    dev_roots(HtmlWriter& h)
    {
        h.title("Root Listing");
        _roots(h, wksp::roots());
    }
    
    void    dev_server(HtmlWriter& h)
    {
        h.title("Server Information");
        h << "<TABLE>";
        
        h.key("Abbreviation") << wksp::abbreviation();
        h.key("Author") << wksp::author();
        h.key("Auxillary Ports") << join(wksp::aux_ports(), ", ");
        h.key("Cache") << wksp::cache_db();
        h.key("Dot") << wksp::dot();
        h.key("Getters") << join(Page::static_getters().keys(), ", ");
        h.key("GIT") << wksp::git();
        h.key("Home") << wksp::home();
        h.key("Host") << wksp::host();
        h.key("Ini") << wksp::ini();
        h.key("Local User") << wksp::local_user();
        h.key("Log Directory") << wksp::log_dir();
        h.key("Markdown") << wksp::markdown();
        h.key("Name") << wksp::name();
        h.key("Perl") << wksp::perl();
        h.key("Port") << wksp::port();
        h.key("Quill Key") << wksp::quill_key();
        h.key("Quill Path") << wksp::quill_file();
        h.key("Read Timeout") << wksp::read_timeout();
        
        {
            auto r = h.key("Roots");
            r << "<OL>";
            for(const Root* rt : wksp::roots())
                r << "<LI>[" << rt -> key << "]: " << rt->path;
            r << "</OL>";
        }
        
        h.key("Smartypants") << wksp::smartypants();
        h.key("Start") << wksp::start();
        
        h.key("Temp Path") << wksp::temp_dir();
        
        {
            auto r = h.key("Templates");
            r << "<OL>";
            for(const String& t : wksp::templates())
                r << "<LI>" << t;
            r << "</OL>";
        }
        
        h << "</TABLE>\n";
    }
    
    void    dev_tables(HtmlWriter& h)
    {
        QSqlDatabase c  = wksp::cache();
        QStringList tables  = c.tables();
        tables.sort();
        h.title("SQL Tables in the Cache");
        h << "<TABLE>\n";
         for(QString t : tables){
            QSqlIndex   i   = c.primaryIndex(t);
        
            h << "<TR><TH valign=\"TOP\">" << t << "</TH><TD>" << i.name() << "</TD><TD>";
            h << "<UL>";
            
            QSqlRecord  r   = c.record(t);
            int rc  = r.count();
            for(int i=0;i<rc;++i){
                h << "<LI>" << r.fieldName(i);
            }
            h << "</UL></TD></TR>\n";
        }
        h << "</TABLE>\n";
   }
   
   void dev_tag(HtmlWriter& h)
   {
        test(decode_tag_prime());
        auto i = cdb::info(x_tag);
        
        h.title("Tag \"" + i.name + "\" (" + i.key + ")");
        auto t = h.table();
        h.key("Name") << i.name;
        h.key("Key") << i.key;
        h.key("Brief") << i.brief;
        h.key("Document") << dev(i.doc);
        h.key("Leaf") << dev(cdb::leaf(x_tag));
        
   }
   
   void dev_tags(HtmlWriter& h)
   {
        h.title("Tag Listing");
        _tags(h, cdb::all_tags(Sorted::YES));
   }
   
   void  dev_workspace(HtmlWriter& h)
   {
        h.title("Workspace '" + wksp::name() + "'");
        
        auto t = h.table();
        h.key("Name") << wksp::name();
        h.key("Author") << wksp::author();
        h.key("Abbreviation") << wksp::abbreviation();
        h.key("Cache") << wksp::cache_db();
        h.key("Config") << dev(cdb::config_folder());
        h.key("Copyright") << wksp::copyright().text;
        h.key("Key") << wksp::quill_key();
        h.key("Host") << wksp::host();
        h.key("Port") << wksp::port();
        h.key("Read Timeout") << wksp::read_timeout();
        h.key("Start") << wksp::start();
        h.key("Templates") << wksp::templates();
   }
 
 
 
    INVOKE(
        reg_page(hGet, "/dev/atom", dev_atom).description("Atom Information").id().key();
        reg_page(hGet, "/dev/atoms", dev_atoms).description("List of Atoms");
        reg_page(hGet, "/dev/cache/tables", dev_tables).description("Cache tables.");
        reg_tabbar({
            reg_page(hGet, "/dev/class", dev_class).description("Class Information").id().key().label("Overview"),
            reg_page(hGet, "/dev/class/reverses", dev_class_reverses).description("Class Uses").id().key().label("Reverses"),
            reg_page(hGet, "/dev/class/sources", dev_class_sources).description("Class Uses").id().key().label("Sources"),
            reg_page(hGet, "/dev/class/targets", dev_class_targets).description("Class Uses").id().key().label("Targets"),
            reg_page(hGet, "/dev/class/uses", dev_class_uses).description("Class Uses").id().key().label("Uses"),
            reg_page(hGet, "/dev/class/subs", dev_class_subs).description("Class Uses").id().key().label("Subs")
        });
        reg_page(hGet, "/dev/classes", dev_classes).description("List of classes.");
        reg_page(hGet, "/dev/db/tables", dev_db_tables).description("List of DB Tables.");
        reg_page(hGet, "/dev/directories", dev_directories).description("List of directories");
        reg_tabbar({
            reg_page(hGet, "/dev/directory", dev_directory).description("Directory Information").id().label("Overview"),
            reg_page(hGet, "/dev/directory/children", dev_directory_children).description("Directory Children").id().label("Children"),
            reg_page(hGet, "/dev/directory/fragments", dev_directory_fragments).description("Directory Fragments").id().label("Fragments")
        });
        reg_page(hGet, "/dev/echo", dev_echo_get).description("Echo's the http request");
        reg_page(hPost, "/dev/echo", dev_echo_post).description("Echo's the http request");
        reg_tabbar({
            reg_page(hGet, "/dev/document", dev_document).description("Document Information").id().key().label("Overview"),
            reg_page(hGet, "/dev/document/fragments", dev_document_fragments).description("List of Document Fragments").id().key().label("Fragments"),
            reg_page(hGet, "/dev/document/roots", dev_document_roots).description("List of Document Roots").id().key().label("Roots")
        });
        reg_page(hGet, "/dev/documents", dev_documents).description("List of documents");
        reg_page(hGet, "/dev/fields", dev_fields).description("List of fields");
        reg_page(hGet, "/dev/fragment", dev_fragment).description("Fragment Information").id().key();
        reg_page(hGet, "/dev/fragments", dev_fragments).description("List of Fragments");
        reg_tabbar({
            reg_page(hGet, "/dev/folder", dev_folder).description("Folder Information").id().key().label("Overview"),
            reg_page(hGet, "/dev/folder/children", dev_folder_children).description("Folder Children").id().key().label("Children"),
            reg_page(hGet, "/dev/folder/directories", dev_folder_directories).description("Folder Directories").id().key().label("Directories"),
            reg_page(hGet, "/dev/folder/documents", dev_folder_documents).description("Folder Documents").id().key().label("Documents"),
            reg_page(hGet, "/dev/folder/fragments", dev_folder_fragments).description("Folder Fragments").id().key().label("Fragments")
        });
        reg_page(hGet, "/dev/folders", dev_folders).description("List of folders");
        reg_page(hGet, "/dev/getters", dev_getters).description("List of (universal) getters");
        reg_page(hGet, "/dev/globals", dev_globals).description("List of Global Variables");
        reg_page(hGet, "/dev/leaf", dev_leaf).description("Leaf Information").id().key();
        reg_page(hGet, "/dev/leafs", dev_leafs).description("List of Leafs");
        reg_page(hGet, "/dev/metas", dev_metas).description("List of Metas");
        reg_page(hGet, "/dev/pages", dev_pages).description("List of Dynamic Pages");
        reg_tabbar({
            reg_page(hGet, "/dev/root", dev_root).description("Root Information").id().key().label("Overview"),
            reg_page(hGet, "/dev/root/all_directories", dev_root_all_directories).description("List of all directories in root").id().key().label("All Dirs"),
            reg_page(hGet, "/dev/root/all_fragments", dev_root_all_fragments).description("List of all fragments in root").id().key().label("All Fragments")
        });
        reg_page(hGet, "/dev/roots", dev_roots).description("List of Roots");
        reg_page(hGet, "/dev/server", dev_server).description("Server information.");
        reg_page(hGet, "/dev/tag", dev_tag).description("Tag Information").argument("id");
        reg_page(hGet, "/dev/tags", dev_tags).description("List of Tags");
        reg_page(hGet, "/dev/wksp", dev_workspace).description("Workspace Information");
    );

}

