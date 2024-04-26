////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Global.hpp>
#include <mithril/field/FieldInfo.hpp>
#include <mithril/folder/FolderInfo.hpp>

namespace {
    void p_dev_atom(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        
        Atom::Info  i   = info(a);
        
        h.title() << "Atom (" << label(a) << ")";

        auto ta = h.table();
        h.kvrow("ID") << a.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Key") << i.key;
        h.kvrow("Abbreviation") << i.abbr;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Leaf") << dev(i.leaf);
        h.kvrow("Parent") << dev(i.parent);
    }
    
    void p_dev_atom_attributes(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Attributes";
        dev_table(h, attributes(a, Sorted::YES));
    }
    
    void p_dev_atom_children(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Children (Atoms)";
        dev_table(h, children(a, Sorted::YES));
    }

    void p_dev_atom_classes(WebHtml&h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Classes";
        dev_table(h, classes(a, Sorted::YES));
    }
    
    //void p_dev_atom_documents(WebHtml& h)
    //{
        //Atom a = atom(h);
        //if(!a)
            //throw HttpStatus::BadArgument;
        //h.title() << "Atom (" << label(a) << "): Documents";
        //dev_table(h, documents(a, Sorted::YES));
    //}

    void p_dev_atom_property(WebHtml& h)
    {
        Atom::Property p = atom_property(h);
        if(!p)
            throw HttpStatus::BadArgument;
        h.title() << "Atom Property (" << p.id << ")";
        auto ta = h.table();
        auto i = info(p);
        h.kvrow("ID") << p.id;
        h.kvrow("Atom") << dev(i.atom);
        h.kvrow("Attribute") << dev(i.attr);
        h.kvrow("Field") << dev(i.field);
        h.kvrow("Source") << dev(i.source);
        h.kvrow("Target") << dev(i.target);
        h.kvrow("Child") << dev(i.child);
    }

    void p_dev_atom_properties(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Properties";
        dev_table(h, properties(a, Sorted::YES));
    }
    
    void p_dev_atom_tags(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Tags";
        dev_table(h, tags(a, Sorted::YES));
    }

    void p_dev_atoms(WebHtml&h)
    {
        h.title() << "All Atoms";
        dev_table(h, all_atoms(Sorted::YES));
    }
    
    void    p_dev_attribute(WebHtml& h)
    {
        Attribute a = attribute(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Attribute " << a.id;
        auto t = h.table();
        h.kvrow("Atom") << a.id;
        h.kvrow("Field") << dev(field(a));
    }

    void    p_dev_attributes(WebHtml&h)
    {
        h.title() << "All Attributes";
        dev_table(h, all_attributes());
    }

    void    p_dev_categories(WebHtml&h)
    {
        h.title() << "All Categories!";
        dev_table(h, all_categories(Sorted::YES));
    }

    void    p_dev_category(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        Category::Info  i   = info(cat);
        
        h.title() << "Category (" << label(cat) << ")";
        auto ta = h.table();
        
        h.kvrow("ID") << cat.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Brief") << i.brief;
    }
    
    void    p_dev_category_classes(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category (" << label(cat) << "): In Classes";
        dev_table(h, classes(cat));
    }

    void    p_dev_category_fields(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category (" << label(cat) << "): In Fields";
        dev_table(h, fields(cat));
    }


    void    p_dev_class(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        Class::Info i  = info(c);
        h.title() << "Class (" << label(c) << ")";
        auto ta = h.table();
        h.kvrow("ID") << c.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Plural") << i.plural;
        h.kvrow("Category") << dev(i.category);
        h.kvrow("Brief") << i.brief;
        h.kvrow("Aliases") << join(cdb::def_alias_set(c), ", ");
        h.kvrow("Prefixes") << join(cdb::def_prefix_set(c), ", ");
        h.kvrow("Reverses") << dev(cdb::def_reverse_set(c));
        h.kvrow("Sources") << dev(cdb::def_source_set(c));
        h.kvrow("Suffixes") << join(cdb::def_suffix_set(c), ", ");
        h.kvrow("Targets") << dev(cdb::def_target_set(c));
        h.kvrow("Uses") << dev(cdb::def_use_set(c));
        h.kvrow("Used") << dev(cdb::def_rev_use_set(c));
        h.kvrow("Binding") << i.binding;
    }
    
    void    p_dev_class_atoms(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Atoms";
        dev_table(h, all_atoms(c, Sorted::YES));
    }
    
    void    p_dev_class_bases(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Class (" << label(x) << "): Bases";
        dev_table(h, base_classes_ranked(x, Sorted::YES), "Hops");
    }
    
    void    p_dev_class_derived(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Derived";
        dev_table(h, derived_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_fields(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, fields(c, Sorted::YES));
    }

    void    p_dev_class_inbound(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Inbound";
        dev_table(h, inbound_classes_ranked(x, Sorted::YES), "Hops");
    }
    
    void    p_dev_class_outbound(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Outbound";
        dev_table(h, outbound_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_rev_uses(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Used By";
        dev_table(h, cdb::def_rev_use(c, Sorted::YES));
        
    }

    void    p_dev_class_reverses(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Reverses";
        dev_table(h, reverse_classes_ranked(x, Sorted::YES), "Hops");
    }
    

    void    p_dev_class_sources(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Sources";
        dev_table(h, source_classes_ranked(x, Sorted::YES), "Hops");
    }

    void    p_dev_class_tags(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Tags";
        dev_table(h, tags(c, Sorted::YES));
        
    }
    
    void    p_dev_class_targets(WebHtml&h)
    {
        Class   x   = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(x) << "): Targets";
        dev_table(h, target_classes_ranked(x, Sorted::YES), "Hops");
    }

    
    void    p_dev_class_uses(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Uses";
        dev_table(h, def_use(c, Sorted::YES));
        
    }
    
    /*
    void    p_dev_class_def_fields(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, def_fields(c, Sorted::YES));
    }
    */

    void    p_dev_classes(WebHtml&h)
    {
        h.title() << "All Classes";
        dev_table(h, all_classes(Sorted::YES));
    }
    
    void    dev_title(WebHtml& h, Directory x, std::string_view extra=std::string_view())
    {
        auto t = h.title();
        const RootDir* rt  = root_dir(x);
        
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
        const RootDir* rt  = root_dir(x);
        
        dev_title(h, x);
        auto ta = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Key") << key(x);
        h.kvrow("Folder") << dev(f);
        h.kvrow("Hidden") << hidden(x);
        if(p)
            h.kvrow("Parent") << dev(p);
        if(rt)
            h.kvrow("RootDir") << dev(rt);
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
    
    void    p_dev_document(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
            
        auto i = info(doc);

        h.title() << "Document (" << i.key << ")";

        auto t = h.table();
        h.kvrow("ID") << doc.id;
        h.kvrow("Folder") << dev(i.folder);
        h.kvrow("Fragments") << fragments_count(doc);
        h.kvrow("Hidden") << i.hidden;
        
        if(is_similar(i.suffix, "class")){
            h.kvrow("As Class") << dev(class_(doc));
        } else if(is_similar(i.suffix, "field")){
            h.kvrow("As Field") << dev(field(doc));
        } else if(is_similar(i.suffix, "user")){
            h.kvrow("As User") << dev(user(doc));
        } else if(is_similar(i.suffix, "tag")){
            h.kvrow("As Tag") << dev(find_tag(doc));
        } else if(is_similar(i.suffix, "y")){
            h.kvrow("As Leaf") << dev(leaf(doc));
        }
        
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Removed") << i.removed;
        h.kvrow("SKey") << i.skey;
        h.kvrow("SKey B") << i.skeyb;
        h.kvrow("SKey C") << i.skeyc;
        h.kvrow("Suffix") << i.suffix;
    }
    
    void    p_dev_document_attributes(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << key(doc) << "): Attributes";
        dev_table(h, attributes(doc));
    }
    
    void    p_dev_document_fragments(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << key(doc) << "): Fragments";
        dev_table(h, fragments(doc));
    }
    
    void    p_dev_document_roots(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << key(doc) << "): Roots";
        dev_table(h, root_dirs(doc));
    }
    
    void    p_dev_documents(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, all_documents(Sorted::YES));
    }
    
    void    p_dev_echo(WebHtml& h)
    {
        auto& ctx   = h.context();
        ctx.decode_query();
        ctx.decode_post();

        h.title("Developer's Echo");
        
        bool    isPost  = ctx.method == hPost;
        
        h.h2("Environment");
        {
            auto ta = h.table();
            h.kvrow("Session ID") << ctx.session.ssid;
            h.kvrow("User") << ctx.session.username;
            h.kvrow("Time") << ctx.timestamp;
            h.kvrow("Path") << ctx.url.path;
            h.kvrow("Method") << ctx.method;
            h.kvrow( "From") << ctx.remote_addr.to_string() << ":" << ctx.remote_port;
        }
        
        h.h2("URL Query");
        h << ctx.rx_query;

        if(isPost){
            h.h2("Post Parameters");
            std::string     url = ctx.rx_post.first("url");
            if(!url.empty()){
                h << "<form action=\"" << url << "\" method=\"post\">\n";
                for(auto& i : ctx.rx_post_raw)
                    h << "<input type=\"hidden\" name=\"" << i.first << "\" id=\"" << i.first << "\" value=\"" << i.second << "\">\n";
                h << "<input type=\"submit\" value=\"Forward\">\n"
                  << "</form>\n";
            }
            
            h << ctx.rx_post;
        }
        
        h.h2("Headers");
        h << ctx.rx_headers;
        
        if(!ctx.rx_body.empty()){
            h.h2("Body");
            h << "<pre>\n";
            html_escape_write(h, ctx.rx_body);
            h << "</pre>\n";
        }
    }

    void    p_dev_field(WebHtml& h)
    {
        Field fld = field(h);
        if(!fld)
            throw HttpStatus::BadArgument;
        Field::Info i   = info(fld);
        h.title() << "Field (" << label(fld) << ")";
        auto ta = h.table();
        h.kvrow("ID") << fld.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Class") << dev(i.class_);
        h.kvrow("Name") << i.name;
        h.kvrow("Plural") << i.plural;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Category") << dev(i.category);
    }
    
    void    p_dev_field_tags(WebHtml& h)
    {
        Field fld = field(h);
        if(!fld)
            throw HttpStatus::BadArgument;
        h.title() << "Field (" << label(fld) << "): Tags";
        dev_table(h, tags(fld, Sorted::YES));
    }

    void    p_dev_fields(WebHtml& h)
    {
        h.title() << "All Fields";
        dev_table(h, all_fields(Sorted::YES));
    }

    void    p_dev_folder(WebHtml&h)
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
    
    void    p_dev_folder_directories(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        h.title() << "Folder (" << key(x) << "): Directories";
        dev_table(h, directories(x));
    }

    void    p_dev_folder_documents(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Folder (" << key(x) << "): Documents";
        dev_table(h, child_documents(x));
    }

    void    p_dev_folder_fragments(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Folder (" << key(x) << "): Fragments";
        dev_table(h, child_fragments(x));
    }

    void    p_dev_folder_subs(WebHtml&h)
    {
        Folder  x   = folder(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Folder (" << key(x) << "): Sub-Folders";
        dev_table(h, child_folders(x));
    }

    void    p_dev_folders(WebHtml& h)
    {
        h.title("Listing of Folders");
        dev_table(h, all_folders(Sorted::YES));
    }
    
 
    void    p_dev_fragment(WebHtml&h)
    {
        Fragment    x = fragment(h, nullptr);
        if(!x)
            throw HttpStatus::BadArgument;

        auto i = info(x);
        h.title() << "Fragment (" << i.path.string() << ")";

        auto _t = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Document") << dev(i.document);
        h.kvrow("Directory") << dev(i.directory);
        h.kvrow("Folder") << dev(i.folder);
        h.kvrow("Hidden") << i.hidden;
        h.kvrow("Modified") << i.modified << " ns.";
        h.kvrow("Name") << i.name;
        h.kvrow("Path") << i.path;
        h.kvrow("Removed") << i.removed;
        h.kvrow("Rescan") << i.rescan;
        h.kvrow("RootDir") << dev(i.root_dir);
        h.kvrow("Size") << i.size;
    }
    
    void    p_dev_fragment_content(WebHtml& h)
    {
        Fragment    x = fragment(h, nullptr);
        if(!x)
            throw HttpStatus::BadArgument;
            
        std::string     sfx = suffix(x).ext;

        h.title() << "Fragment (" << path(x) << "): Content";
        ContentType     ct = mimeTypeForExt( sfx);
        if(isImage(ct)){
            h << "<img src=\"/dev/fragment/image?" << h.context().url.query << "\" alt=\"Raw Image\">";
        } else if(isTextual(ct) || !ct){
            if(!is_similar(sfx, "user")){ // blank out user files (avoid passwords)
                h << "<pre>";
                std::string s = frag_string(x);
                html_escape_write(h, s);
                h << "</pre>";
            }
        } else if(isAudio(ct)){
            h.p() << "Audio File... TODO";
        } else if(isMovie(ct)){
            h.p() << "Movie File... TODO";
        } else {
            h.p() << "Unimplemented type: " << mimeType(ct);
        }
    }

    void    dev_fragment_image(WebContext& ctx)
    {
        Fragment x  = fragment(ctx, nullptr);
        if(!x)  
            throw HttpStatus::BadArgument;
    
        auto ext = suffix(x).ext;
        ctx.tx_content_type = mimeTypeForExt( ext );
        ctx.tx_content      = std::make_shared<ByteArray>(frag_bytes(x));
    }

    void    p_dev_fragments(WebHtml& h)
    {
        h.title("Listing of Fragments");
        dev_table(h, all_fragments(Sorted::YES));
    }
    
    void    p_dev_globals(WebHtml& h)
    {
        h.title("Listing of Global Variables");
        auto _t = h.table();
        for(std::string_view k : global::variable::names()){
            h << "<tr><th align=\"left\">" << k << "</th><td>";
            auto v = global::variable::get(k);
            if(!v){
                h << "<b>[error]</b></td><td> " << v.error().message();
            } else {
                h << "<i>[" << v->type().name() << "]</i></td><td>";
                html_escape_write(h, v->printable());
            }
            h << "</td></tr>\n";
        }
    }
 
    void p_dev_graphviz(WebHtml&h)
    {
        Graphviz    gv  = graphviz(h);
        if(!gv)
            throw HttpStatus::BadArgument;
        
        auto t = h.table();
        h.kvrow("Name") << cdb::name(gv);
        h.kvrow("Dot Size") << cdb::dot_size(gv);
        h.kvrow("Html Size") << cdb::html_size(gv);
        h.kvrow("SVG Size") << cdb::svg_size(gv);
    }
    
    void    p_dev_graphviz_html(WebHtml&h)
    {
        Graphviz    gv  = graphviz(h);
        if(!gv)
            throw HttpStatus::BadArgument;
        h.title("HTML");
        std::string txt = cdb::html_text(gv);
        h << "<pre>\n";
        html_escape_write(h, txt);
        h << "</pre>\n";
    }

    void    p_dev_graphviz_dot(WebHtml&h)
    {
        Graphviz    gv  = graphviz(h);
        if(!gv)
            throw HttpStatus::BadArgument;
        h.title("DOT");
        std::string txt = cdb::dot_text(gv);
        h << "<pre>\n";
        html_escape_write(h, txt);
        h << "</pre>\n";
    }

    void    p_dev_graphviz_svg(WebHtml&h)
    {
        Graphviz    gv  = graphviz(h);
        if(!gv)
            throw HttpStatus::BadArgument;
        h.title("SVG");
        h << gv;
    }
    
    void p_dev_graphvizs(WebHtml& h)
    {
        h.title("All Graphvizs");
        dev_table(h, all_graphvizs());
    }

    void    p_dev_hello(WebHtml& out)
    {
        out.title("HELLO WORLD!");
        
        auto _t     = out.table();
        
        out.kvrow("Method") << out.context().method.key() << "\n";
        out.kvrow("URL") << out.context().url << "\n";
        out.kvrow("Client HTTP") << out.context().version << "</td></tr>\n";
        for(auto& hv : out.context().rx_headers)
            out.kvrow(hv.first) << hv.second;
    }
    
    
    void p_dev_image(WebHtml& h)
    {
        Image   img = image(h);
        if(!img)
            throw HttpStatus::BadArgument;
        
        Fragment    frag    = fragment(img);
        Document    doc     = document(img);
        const RootDir* rt      = root_dir(frag);
        if(!rt)
            throw HttpStatus::BadArgument;
        
        h.title() << "Image [" << rt->key << "](" << label(doc) << ")";
        h << img;
    }

    void p_dev_images(WebHtml& h)
    {
        h.title("All Images");
        dev_table(h, all_images());
    }

    void p_dev_leaf(WebHtml&h)
    {
        Leaf    x   = leaf(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        auto i = info(x);
            
        h.title() << "Leaf (" << i.key << ")";
        auto t = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Title") << i.title;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Document") << dev(i.doc);
        h.kvrow("Atom") << dev(i.atom);
   }
    
    //  we'll do more (later)

    void    p_dev_leafs(WebHtml&h)
    {
        h.title("All Leafs");
        dev_table(h, all_leafs(Sorted::YES));
    }

    void p_dev_meta_object(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << ")";
        auto ta = h.table();
        
        h.kvrow("ID") << obj -> id();
        h.kvrow("Name") << obj -> name();
        h.kvrow("Base") << dev(obj->base());
        h.kvrow("Description") << obj->description();
        h.kvrow("Size") << obj->size();
        h.kvrow("Abstract?") << obj->is_abstract();
    }
    
    void p_dev_meta_object_all_base(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Bases";
        dev_table(h, obj->bases(true).all);
    }

    void p_dev_meta_object_all_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Derived";
        dev_table(h, obj->deriveds(true).all);
    }

    void p_dev_meta_object_all_properties(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Properties";
        dev_table(h, obj->properties(true).all);
    }

    void p_dev_meta_object_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Derived";
        dev_table(h, obj->deriveds(false).all);
    }

    void p_dev_meta_object_properties(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Properties";
        dev_table(h, obj->properties(false).all);
    }
    
    void    p_dev_meta_objects(WebHtml&h)
    {
        h.title() << "All Meta Objects";
        dev_table(h, ObjectInfo::all());
    }

    void    p_dev_meta_type(WebHtml& h)
    {
        const TypeInfo* type    = type_info(h);
        if(!type)
            throw HttpStatus::BadArgument;
        
        h.title() << "Type Info (" << type->name() << ")";
        auto ta = h.table();
        h.kvrow("ID") << type -> id();
        h.kvrow("Name") << type->name();
        h.kvrow("Aliases") << join(type->aliases(), ", ");
        h.kvrow("Size") << type->size();
        
        
        h.kvrow("Small?") << type->is_small();
        h.kvrow("Parsable?") << type->can_parse();
        h.kvrow("Writable?") << type->can_write();
        h.kvrow("Printable?") << type->can_print();
        
        Any     def(type);
        h.kvrow("Default") << "'" << def.printable() << "'";

        h.kvrow("Properties") << type->property_count();
        h.kvrow("Methods") << type->method_count();
    }
    
    void    p_dev_meta_type_methods(WebHtml& h)
    {
        const TypeInfo* type    = type_info(h);
        if(!type)
            throw HttpStatus::BadArgument;
        
        h.title() << "Type Info (" << type->name() << "): Methods";
        dev_table(h, type->methods());
    }
    
    void    p_dev_meta_type_properties(WebHtml& h)
    {
        const TypeInfo* type    = type_info(h);
        if(!type)
            throw HttpStatus::BadArgument;
        
        h.title() << "Type Info (" << type->name() << "): Properties";
        dev_table(h, type->properties());
    }

    void p_dev_meta_types(WebHtml&h)
    {
        h.title() << "All Type Infos";
        dev_table(h, TypeInfo::all());
    }

    void p_dev_mimetypes(WebHtml&h)
    {
        h.title("Mime Types");
        auto ta = h.table();
        h << "<tr><th>ID</th><th>KEY</th><th>MIME-TYPE</th></tr>\n";
        for(ContentType ct : ContentType::all_values())
            h << "<tr><td>" << ct.value() << "</td><td>" << ct.key() << "</td><td>" << mimeType(ct) << "</td></tr>\n";
    }

    void    p_dev_root(WebHtml& h)
    {
        const RootDir* rt   = root_dir(h);
        if(!rt)
            rt          = wksp::root_dir(0);
        
        h.title() << "RootDir (" << rt->name << ")";
        
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

        h.title() << "RootDir (" << rt->name << "): All Directories";
        dev_table(h, all_directories(rt, Sorted::YES));
    }
    
    void    p_dev_root_all_fragments(WebHtml& h)
    {
        const RootDir* rt  = root_dir(h);
        if(!rt)
            rt          = wksp::root_dir(0);
            
        h.title() << "RootDir (" << rt->name << "): All Fragments";
        dev_table(h, all_fragments(rt, Sorted::YES));
    }

    void    p_dev_root_directories(WebHtml& h)
    {
        const RootDir* rt   = root_dir(h);
        if(!rt)
            rt          = wksp::root_dir(0);
            
        h.title() << "RootDir (" << rt->name << "): Directories";
        
        dev_table(h, directories(rt, Sorted::YES));
    }

    void    p_dev_root_fragments(WebHtml& h)
    {
        const RootDir* rt   = root_dir(h);
        if(!rt)
            rt          = wksp::root_dir(0);

        h.title() << "RootDir (" << rt->name << "): Fragments";

        dev_table(h, fragments(rt, Sorted::YES));
    }
    
    void    p_dev_roots(WebHtml& h)
    {
        h.title() << "All Roots";
        dev_table(h, wksp::root_dirs());
    }
    
    void p_dev_server(WebHtml&h)
    {
        h.title("Server Information");
        auto ta  = h.table();
        
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Auxillary Ports") << join(wksp::aux_ports(), ", ");
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Dot") << wksp::dot();
        h.kvrow("GIT") << wksp::git();
        h.kvrow("Home") << wksp::home();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Local User") << wksp::local_user();
        h.kvrow("Log Directory") << wksp::log_dir();
        h.kvrow("Markdown") << wksp::markdown();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Perl") << wksp::perl();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Quill Key") << wksp::quill_key();
        h.kvrow("Quill Path") << wksp::quill_file();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        
        {
            auto r = h.kvrow("Roots");
            r << "<OL>";
            for(const RootDir* rt : wksp::root_dirs())
                r << "<LI>[" << rt -> key << "]: " << rt->path;
            r << "</OL>";
        }
        
        h.kvrow("Smartypants") << wksp::smartypants();
        h.kvrow("Start") << wksp::start();
        
        h.kvrow("Temp Path") << wksp::temp_dir();
        
        {
            auto r = h.kvrow("Templates");
            r << "<OL>";
            for(const std::string& t : wksp::templates())
                r << "<LI>" << t;
            r << "</OL>";
        }
    }

    void p_dev_sql_table(WebHtml& h)
    {
        int i, cc, count;
        std::string table = h.context().find_query("table");
        if(table.empty())
            throw HttpStatus::BadArgument;
        
        auto& db    = wksp::db();
        if(!db.has_table(table))
            throw HttpStatus::NotFound;

        std::string cmd = "SELECT * FROM " + table;
        SqlQuery    sql(db, cmd);
        if(!sql.valid())
            throw HttpStatus::InternalError;

        h.title() << "Sql Table: " << table;
        
//        bool    first   = false;
        
        auto _table = h.table();

        h << "<tr>";
        cc  = sql.column_count();
        for(i=1;i<=cc;++i)
            h << "<th>" << sql.column_name(i) << "</th>";
        h << "</tr>\n";
        
        count   = 0;
        SQResult    r;
        while((r = sql.step()) == SQResult::Row){
            ++ count;
            h << "<tr>";
            for(i=1;i<=cc;++i){
                h << "<td>";
                html_escape_write(h, sql.v_text(i));
                h << "</td>";
            }
            h << "</tr>\n";
        }
        
        h << "</table>\n";
        h.paragraph() << "This table has " << count << " row(s).";
    }
    
    void p_dev_sql_tables(WebHtml& out)
    {
        auto list = out.numbers();
        for(auto& s : wksp::db().tables())
            out.li() << "<a href=\"/dev/sql/table?table=" << web_encode(s) << "\">" << s << "</a>\n";
    }
    
    void    dev_title(WebHtml& h, Style x, std::string_view extra=std::string_view())
    {
        auto t = h.title();
        auto i = nki(x);
        h << "Style \"" << i.name << "\" (" << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }
    

    void    p_dev_style(WebHtml& h)
    {
        Style   x = style(h);
        if(!x)
            throw HttpStatus::BadArgument;
        auto i = cdb::info(x);
        dev_title(h, x);
        auto ta = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Key") << i.key;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Document") << dev(i.doc);
    }

    void    p_dev_styles(WebHtml& out)
    {
        out.title("All Styles");
        dev_table(out, all_styles());
    }

    void    dev_title(WebHtml& h, Tag x, std::string_view extra=std::string_view())
    {
        auto t = h.title();
        auto i = nki(x);
        h << "Tag \"" << i.name << "\" (" << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }
    

    void    p_dev_tag(WebHtml& h)
    {
        Tag x   = tag(h);
        if(!x)
            throw HttpStatus::BadArgument;
        auto i = info(x);
        
        dev_title(h, x);
        auto ta = h.table();
        h.kvrow("ID") << x.id;
        h.kvrow("Name") << i.name;
        h.kvrow("Key") << i.key;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Document") << dev(i.doc);
        h.kvrow("Leaf") << dev(leaf(x));
        h.kvrow("Image");
    }
    
    void    p_dev_tag_atoms(WebHtml& h)
    {
        Tag x   = tag(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Atoms");
        dev_table(h, all_atoms(x));
    }

    void    p_dev_tag_classes(WebHtml& h)
    {
        Tag x   = tag(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Classes");
        dev_table(h, classes_with_tag(x));
    }

    void    p_dev_tags(WebHtml& out)
    {
        out.title("All Tags");
        dev_table(out, all_tags());
    }

    void    p_dev_users(WebHtml& h)
    {
        h.title() << "All Users";
        dev_table(h, all_users(Sorted::YES));
    }

    void    p_dev_webdirs(WebHtml&h)
    {
        h.title("All WebDirs");
        dev_table(h, web::directory_map());
    }

    void p_dev_webexts(WebHtml& h)
    {
        h.title("All WebExts");
        dev_table(h, web::extension_map());
    }
    
    void p_dev_webglobs(WebHtml& h)
    {
        h.title("All WebGlobs");
        dev_table(h, web::glob_map());
    }

    void p_dev_webpages(WebHtml& h)
    {
        h.title("All WebPages");
        
        h << "<div class=\"explain\">"
          << "List of all registered (specific) web-pages.  Directories & GLOBs not included."
          << "</div>\n";
        
        dev_table(h, web::page_map(), true);
    }

    void p_dev_webvars(WebHtml& h)
    {
        h.title("All WebVariables");
        dev_table(h, web::variable_map());
    }


    void  p_dev_wksp(WebHtml& h)
    {
        h.title() << "Workspace '" << wksp::name() << "'";
        
        auto t = h.table();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Color (Background)") << gBkColor.get();
        h.kvrow("Color (Text)") << gTextColor.get();
        //h.kvrow("Config") << dev(config_folder());
        h.kvrow("Copyright") << wksp::copyright().text;
        h.kvrow("Key") << wksp::quill_key();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        h.kvrow("Start") << wksp::start();
        h.kvrow("Templates") << join(wksp::templates(), ", ");
    }

    void    p_dev_wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::root_dirs());
    }

    void    reg_dev()
    {
        reg_webgroup({
            reg_webpage<p_dev_atom>("/dev/atom").argument("id", "Atom ID").label("Info"),
            reg_webpage<p_dev_atom_attributes>("/dev/atom/attributes").argument("id", "Atom ID").label("Attributes"),
            reg_webpage<p_dev_atom_children>("/dev/atom/children").argument("id", "Atom ID").label("Children"),
            reg_webpage<p_dev_atom_classes>("/dev/atom/classes").argument("id", "Atom ID").label("Classes"),
            //reg_webpage<p_dev_atom_documents>("/dev/atom/documents").argument("id", "Atom ID").label("Docs"),
            reg_webpage<p_dev_atom_properties>("/dev/atom/properties").argument("id", "Atom ID").label("Properties"),
            reg_webpage<p_dev_atom_tags>("/dev/atom/tags").argument("id", "Atom ID").label("Tags")
        });
        reg_webpage<p_dev_atom_property>("/dev/atom_property").argument("id", "Atom Property ID");
        reg_webpage<p_dev_atoms>("/dev/atoms");
        reg_webpage<p_dev_attribute>("/dev/attribute");
        reg_webpage<p_dev_attributes>("/dev/attributes");

        reg_webpage<p_dev_categories>("/dev/categories"); 
        reg_webgroup({
            reg_webpage<p_dev_category>("/dev/category").argument("id", "Category ID").label("Info"),
            reg_webpage<p_dev_category_classes>("/dev/category/classes").argument("id", "Category ID").label("Classes"),
            reg_webpage<p_dev_category_fields>("/dev/category/fields").argument("id", "Category ID").label("Fields")
        });

        reg_webgroup({
            reg_webpage<p_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
            reg_webpage<p_dev_class_bases>("/dev/class/base").argument("id", "Class ID").label("Base"),
            //reg_webpage<p_dev_class_uses>("/dev/class/uses").argument("id", "Class ID").label("Uses"),
            //reg_webpage<p_dev_class_rev_uses>("/dev/class/used").argument("id", "Class ID").label("Used"),
            reg_webpage<p_dev_class_derived>("/dev/class/derived").argument("id", "Class ID").label("Derived"),
            reg_webpage<p_dev_class_sources>("/dev/class/sources").argument("id", "Class ID").label("Sources"),
            reg_webpage<p_dev_class_targets>("/dev/class/targets").argument("id", "Class ID").label("Targets"),
            reg_webpage<p_dev_class_inbound>("/dev/class/inbound").argument("id", "Class ID").label("Inbound"),
            reg_webpage<p_dev_class_outbound>("/dev/class/outbound").argument("id", "Class ID").label("Outbound"),
            reg_webpage<p_dev_class_reverses>("/dev/class/reverses").argument("id", "Class ID").label("Reverses"),
            reg_webpage<p_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags"),
            reg_webpage<p_dev_class_fields>("/dev/class/fields").argument("id", "Class ID").label("Fields"),
            //reg_webpage<p_dev_class_def_fields>("/dev/class/def_fields").argument("id", "Class ID").label("Def Fields"),
            reg_webpage<p_dev_class_atoms>("/dev/class/atoms").argument("id", "Class ID").label("Atoms")
        });
        reg_webpage<p_dev_classes>("/dev/classes");

        reg_webpage<p_dev_directories>("/dev/directories");
        reg_webgroup({
            reg_webpage<p_dev_directory>("/dev/directory").argument("id", "Directory ID").label("Info"),
            reg_webpage<p_dev_directory_fragments>("/dev/directory/fragments").argument("id", "Directory ID").label("Frags"),
            reg_webpage<p_dev_directory_subdirs>("/dev/directory/subdirs").argument("id", "Directory ID").label("Subdirs")
        });

        reg_webgroup({
            reg_webpage<p_dev_document>("/dev/document").argument("id", "Document ID").label("Info"),
            reg_webpage<p_dev_document_attributes>("/dev/document/attributes").argument("id", "Document ID").label("Attributes"),
            reg_webpage<p_dev_document_fragments>("/dev/document/fragments").argument("id", "Document ID").label("Fragments"),
            reg_webpage<p_dev_document_roots>("/dev/document/root_dirs").argument("id", "Document ID").label("Roots")
        });
        reg_webpage<p_dev_documents>("/dev/documents"); 

        reg_webpage<p_dev_echo>({hGet, hPost}, "/dev/echo");

        reg_webgroup({
            reg_webpage<p_dev_field>("/dev/field").argument("id", "Field ID").label("Info"),
            reg_webpage<p_dev_field_tags>("/dev/field/tags").argument("id", "Field ID").label("Fields")
        });
        reg_webpage<p_dev_fields>("/dev/fields");

       reg_webgroup({
            reg_webpage<p_dev_folder>("/dev/folder").label("Info"),
            reg_webpage<p_dev_folder_directories>("/dev/folder/dirs").label("Dirs"),
            reg_webpage<p_dev_folder_documents>("/dev/folder/docs").label("Docs"),
            reg_webpage<p_dev_folder_fragments>("/dev/folder/frags").label("Frags"),
            reg_webpage<p_dev_folder_subs>("/dev/folder/subs").label("Subs")
        });
        reg_webpage<p_dev_folders>("/dev/folders");
 
       reg_webgroup({
            reg_webpage<p_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID").label("Info"),
            reg_webpage<p_dev_fragment_content>("/dev/fragment/content").description("Content for fragment").argument("id", "Fragment ID").label("Content")
        });
        reg_webpage<dev_fragment_image>("/dev/fragment/image").argument("id", "ID for fragment"); 
        reg_webpage<p_dev_fragments>("/dev/fragments");
 
        reg_webpage<p_dev_globals>("/dev/globals").local();
 
        reg_webgroup({
            reg_webpage<p_dev_graphviz>("/dev/graphviz").description("Info for a graphviz").argument("id", "Graphviz ID").label("Info"),
            reg_webpage<p_dev_graphviz_dot>("/dev/graphviz/dot").description("DOT for a graphviz").argument("id", "Graphviz ID").label("DOT"),
            reg_webpage<p_dev_graphviz_html>("/dev/graphviz/html").description("HTML for a graphviz").argument("id", "Graphviz ID").label("HTML"),
            reg_webpage<p_dev_graphviz_svg>("/dev/graphviz/svg").description("SVG for a graphviz").argument("id", "Graphviz ID").label("SVG")
        });
        reg_webpage<p_dev_graphvizs>("/dev/graphvizs");
 
        reg_webpage<p_dev_hello>("/dev/hello");

        reg_webpage<p_dev_image>("/dev/image");
        reg_webpage<p_dev_images>("/dev/images"); 
 
        reg_webgroup({
            reg_webpage<p_dev_leaf>("/dev/leaf").argument("id", "Leaf ID").label("Info")
        });
        reg_webpage<p_dev_leafs>("/dev/leafs");

        reg_webgroup({
            reg_webpage<p_dev_meta_object>("/dev/meta/object").label("Info"),
            reg_webpage<p_dev_meta_object_derived>("/dev/meta/object/derived").label("Derived"),
            reg_webpage<p_dev_meta_object_properties>("/dev/meta/object/properties").label("Properties"),
            reg_webpage<p_dev_meta_object_all_base>("/dev/meta/object/all_base").label("All Base"),
            reg_webpage<p_dev_meta_object_all_derived>("/dev/meta/object/all_derived").label("All Derived"),
            reg_webpage<p_dev_meta_object_all_properties>("/dev/meta/object/all_properties").label("All Properties")
        });
        reg_webpage<p_dev_meta_objects>("/dev/meta/objects"); 
        reg_webgroup({
            reg_webpage<p_dev_meta_type>("/dev/meta/type").label("Info"),
            reg_webpage<p_dev_meta_type_methods>("/dev/meta/methods").label("Methods"),
            reg_webpage<p_dev_meta_type_properties>("/dev/meta/type/properties").label("Properties")
        });
        reg_webpage<p_dev_meta_types>("/dev/meta/types");

        reg_webpage<p_dev_mimetypes>("/dev/mimetypes");

        reg_webgroup({
            reg_webpage<p_dev_root>("/dev/root").argument("id", "RootDir ID").label("Info"),
            reg_webpage<p_dev_root_directories>("/dev/root/dirs").argument("id", "RootDir ID").label("Dirs"),
            reg_webpage<p_dev_root_fragments>("/dev/root/frags").argument("id", "RootDir ID").label("Frags"),
            reg_webpage<p_dev_root_all_directories>("/dev/root/all_dirs").argument("id", "RootDir ID").label("AllDirs"),
            reg_webpage<p_dev_root_all_fragments>("/dev/root/all_frags").argument("id", "RootDir ID").label("AllFrags")
        });
        reg_webpage<p_dev_roots>("/dev/roots");

        reg_webpage<p_dev_server>("/dev/server").local();

        reg_webpage<p_dev_sql_table>("/dev/sql/table").local().argument("table", "SQL Table Name");
        reg_webpage<p_dev_sql_tables>("/dev/sql/tables", "SQL Tables").local();
        
        reg_webpage<p_dev_style>("/dev/style").local().argument("id", "Style id (number)");
        reg_webpage<p_dev_styles>("/dev/styles");

        reg_webgroup({
            reg_webpage<p_dev_tag>("/dev/tag")
                .argument("id", "Tag id (number)")
                .argument("key", "Tag key (string)")
                .argument("tag", "Tag key/identifier").label("Info"),
            reg_webpage<p_dev_tag_atoms>("/dev/tag/atoms").argument("id", "Tag id (number)").label("Atoms"),
            reg_webpage<p_dev_tag_classes>("/dev/tag/classes").argument("id", "Tag id (number)").label("Classes")
        });
        reg_webpage<p_dev_tags>("/dev/tags");

        reg_webpage<p_dev_users>("/dev/users");

        reg_webpage<p_dev_webdirs>("/dev/web/directories");
        reg_webpage<p_dev_webexts>("/dev/web/extensions"); 
        reg_webpage<p_dev_webglobs>("/dev/web/globs");
        reg_webpage<p_dev_webpages>("/dev/web/pages");
        reg_webpage<p_dev_webvars>("/dev/web/variables");

        reg_webgroup({
            reg_webpage<p_dev_wksp>("/dev/wksp").label("Info"),
            reg_webpage<p_dev_wksp_roots>("/dev/wksp/roots").label("Roots")
        });
    }
}
