////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/db.hpp>
#include <db/filesys/directory-arg.hpp>
#include <yq/app/DelayInit.hpp>
#include <yq/collection/c_utils.hpp>
#include <yq/io/dir_utils.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/meta/ObjectInfo.hpp>
#include <yq/meta/TypeInfo.hpp>
#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/Markdown.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/JsonAdapter.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/Template.hpp>

using namespace yq;
using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;

#include "common.hpp"

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

    void    dev_title(WebHtml& h, Tag x, std::string_view extra=std::string_view())
    {
        auto t = h.title();
        auto i = nki(x);
        h << "Tag \"" << i.name << "\" (" << i.key << ")";
        if(!extra.empty())
            h << ": " << extra;
    }



    void    page__index(WebHtml& out)
    {
        auto idx    = gIndex.get();
        if(!idx)
            return;
        idx -> execute(out);
    }

    void    page_admin_users(WebHtml&h)
    {
        // might have some characteristics?
        h.title() << "Users";
        dev_table(h, cdb::all_users(Sorted::YES));  // STUB
    }

    json    page_api_workspace(WebContext&ctx)
    {
        json    ret{
            { "author", wksp::author() },
            { "abbreviation", wksp::abbreviation() },
            { "bkcolor", gBkColor },
            { "copyright", wksp::copyright().text },
            { "c_stance", wksp::copyright().stance.key() },
            { "c_from", wksp::copyright().from },
            { "c_to", wksp::copyright().to },
            { "color", gTextColor },
            { "name", wksp::name() }
        };
        if(ctx.is_local()){
            ret["quill"] = wksp::quill_file().string();
            ret["cache"] = wksp::cache().string();
        }
        return ret;
    }

    void page_atom(WebHtml& h)
    {
        //   this will redirect the atom appropriately
        h << "TODO";
    }
    
    void page_atoms(WebHtml& h)
    {
        //   this will be a search-spot for the appropriate atoms
        h << "TODO";
    }
    
    void    page_background(WebContext& ctx)
    {
        Ref<TypedBytes>     data = gBackground;
        if(!data){
            ctx.status  = HttpStatus::ServiceUnavailable;
            return;
        }
        
        ctx.tx_content_type = data -> type;
        ctx.tx_content      = data -> data;
        ctx.tx_header("Date", data->modified);
        ctx.status          = HttpStatus::Success;
    }

    void page_class(WebHtml& h)
    {
        Class x = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        h.title() << "Info for class " << cdb::label(x);
        h << "TODO!";
    }
    
    void page_class_atoms(WebHtml&h)
    {
        Class x = class_(h);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << cdb::plural(x);
        h << "TODO!";
    }

    void    page_css(WebContext& ctx)
    {
        ctx.tx_content_type = ContentType::css;
        ctx.tx_content = gCss;
    }

    void page_dev_atom(WebHtml& h)
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
        //h.kvrow("Leaf") << dev(i.leaf);
    }
    
    void page_dev_atom_classes(WebHtml&h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Classes";
        dev_table(h, classes(a, Sorted::YES));
    }
    
    void page_dev_atom_documents(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Documents";
        dev_table(h, documents(a, Sorted::YES));
    }
    
    void page_dev_atom_tags(WebHtml& h)
    {
        Atom a = atom(h);
        if(!a)
            throw HttpStatus::BadArgument;
        h.title() << "Atom (" << label(a) << "): Tags";
        dev_table(h, tags(a, Sorted::YES));
    }

    void page_dev_atoms(WebHtml&h)
    {
        h.title() << "All Atoms";
        dev_table(h, all_atoms(Sorted::YES));
    }

    void    page_dev_categories(WebHtml&h)
    {
        h.title() << "All Categories!";
        dev_table(h, all_categories(Sorted::YES));
    }

    void    page_dev_category(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        Category::Info  i   = info(cat);
        
        h.title() << "Category (" << cdb::label(cat) << ")";
        auto ta = h.table();
        
        h.kvrow("ID") << cat.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Brief") << i.brief;
    }
    
    void    page_dev_category_classes(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category (" << cdb::label(cat) << "): In Classes";
        dev_table(h, classes(cat));
    }

    void    page_dev_category_fields(WebHtml& h)
    {
        Category    cat = category(h);
        if(!cat)
            throw HttpStatus::BadArgument;
        
        h.title() << "Category (" << cdb::label(cat) << "): In Fields";
        dev_table(h, fields(cat));
    }

    void    page_dev_class(WebHtml& h)
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
        h.kvrow("Aliases") << join(def_aliases(c), ", ");
        h.kvrow("Prefixes") << join(def_prefixes(c), ", ");
        h.kvrow("Suffixes") << join(def_suffixes(c), ", ");
        h.kvrow("Binding") << i.binding;
    }
    
    void    page_dev_class_tags(WebHtml& h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;
            
        h.title() << "Class (" << label(c) << "): Tags";
        dev_table(h, tags(c, Sorted::YES));
        
    }
    
    void    page_dev_class_def_fields(WebHtml&h)
    {
        Class   c   = class_(h);
        if(!c)
            throw HttpStatus::BadArgument;

        h.title() << "Class (" << label(c) << "): Def Fields";
        dev_table(h, def_fields(c, Sorted::YES));
    }

    void    page_dev_classes(WebHtml&h)
    {
        h.title() << "All Classes";
        dev_table(h, all_classes(Sorted::YES));
    }

    void    page_dev_directories(WebHtml& h)
    {
        h.title("All Directories");
        dev_table(h, all_directories(Sorted::YES));
    }
    
    void    page_dev_directory(WebHtml& h)
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
    
    void    page_dev_directory_fragments(WebHtml& h)
    {
        Directory   x   = directory(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Fragments");
        dev_table(h, child_fragments(x, Sorted::YES));
    }

    void    page_dev_directory_subdirs(WebHtml& h)
    {
        Directory   x   = directory(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Subdirectories");
        dev_table(h, child_directories(x, Sorted::YES));
    }

    void    page_dev_document(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
            
        auto i = cdb::info(doc);

        h.title() << "Document (" << i.key << ")";

        auto t = h.table();
        h.kvrow("ID") << doc.id;
        h.kvrow("Folder") << dev(i.folder);
        h.kvrow("Fragments") << cdb::fragments_count(doc);
        h.kvrow("Hidden") << i.hidden;
        
        if(is_similar(i.suffix, "class")){
            h.kvrow("Is class") << dev(cdb::class_(doc));
        } else if(is_similar(i.suffix, "field")){
            h.kvrow("Is field") << dev(cdb::field(doc));
        } else if(is_similar(i.suffix, "user")){
            h.kvrow("Is user") << dev(cdb::user(doc));
        } else if(is_similar(i.suffix, "tag")){
            h.kvrow("Is tag") << dev(cdb::tag(doc));
        } else if(is_similar(i.suffix, "y")){
            h.kvrow("Is leaf") << dev(cdb::leaf(doc));
        }
        
        h.kvrow("Key") << i.key;
        h.kvrow("Name") << i.name;
        h.kvrow("Removed") << i.removed;
        h.kvrow("SKey") << i.skey;
        h.kvrow("SKey B") << i.skeyb;
        h.kvrow("SKey C") << i.skeyc;
        h.kvrow("Suffix") << i.suffix;
    }
    
    void    page_dev_document_attributes(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Attributes";
        dev_table(h, cdb::attributes(doc));
    }
    
    void    page_dev_document_fragments(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Fragments";
        dev_table(h, cdb::fragments(doc));
    }
    
    void    page_dev_document_roots(WebHtml&h)
    {
        Document    doc = document(h);
        if(!doc)
            throw HttpStatus::BadArgument;
        h.title() << "Document (" << cdb::key(doc) << "): Roots";
        dev_table(h, cdb::roots(doc));
    }
    
    void    page_dev_documents(WebHtml& h)
    {
        h.title("Listing of Documents");
        dev_table(h, all_documents(Sorted::YES));
    }

    void    page_dev_echo(WebHtml& h)
    {
        auto& ctx   = h.context();

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
        {
            auto ta = h.table();
            for(auto& i : web_decode(ctx.decode_query()))
                h.kvrow(i.first) << i.second;
        }

        if(isPost){
            h.h2("Post Parameters");

            StringViewMultiMap   postParams = ctx.decode_post();
            std::string_view     url = postParams.first("url");
            if(!url.empty()){
                h << "<form action=\"" << url << "\" method=\"post\">\n";
                for(auto& i : postParams)
                    h << "<input type=\"hidden\" name=\"" << i.first << "\" id=\"" << i.first << "\" value=\"" << i.second << "\">\n";
                h << "<input type=\"submit\" value=\"Forward\">\n"
                  << "</form\n";
            }
            
            auto ta = h.table();
            for(auto& i : web_decode(postParams))
                h.kvrow(i.first) << i.second;
        }
        
        h.h2("Headers");
        {
            auto ta = h.table();
            for(auto& i : ctx.rx_headers)
                h.kvrow(i.first) << i.second;
        }
        
        if(!ctx.rx_body.empty()){
            h.h2("Body");
            h << ctx.rx_body;
        }
    }

    void    page_dev_field(WebHtml& h)
    {
        Field fld = field(h);
        if(!fld)
            throw HttpStatus::BadArgument;
        Field::Info i   = info(fld);
        h.title() << "Field (" << cdb::label(fld) << ")";
        auto ta = h.table();
        h.kvrow("ID") << fld.id;
        h.kvrow("Key") << i.key;
        h.kvrow("Class") << dev(i.class_);
        h.kvrow("Name") << i.name;
        h.kvrow("Plural") << i.plural;
        h.kvrow("Brief") << i.brief;
        h.kvrow("Category") << dev(i.category);
    }
    
    void    page_dev_field_tags(WebHtml& h)
    {
        Field fld = field(h);
        if(!fld)
            throw HttpStatus::BadArgument;
        h.title() << "Field (" << cdb::label(fld) << "): Tags";
        dev_table(h, tags(fld, Sorted::YES));
    }

    void    page_dev_fields(WebHtml& h)
    {
        h.title() << "All Fields";
        dev_table(h, all_fields(Sorted::YES));
    }


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

    void    page_dev_fragment(WebHtml&h)
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
        h.kvrow("Root") << dev(i.root);
        h.kvrow("Size") << i.size;
    }
    
    void    page_dev_fragment_content(WebHtml& h)
    {
        Fragment    x = fragment(h, nullptr);
        if(!x)
            throw HttpStatus::BadArgument;

        h.title() << "Fragment (" << path(x) << "): Content";
        ContentType     ct = mimeTypeForExt( suffix(x));
        if(isImage(ct)){
            h << "<img src=\"/dev/fragment/image?" << h.context().url.query << "\" alt=\"Raw Image\">";
        } else if(isTextual(ct) || !ct){
            if(!is_similar(suffix(document(x)), "usr")){ // blank out user files (avoid passwords)
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
    
        ctx.tx_content_type = mimeTypeForExt( suffix(x));
        ctx.tx_content      = std::make_shared<ByteArray>(frag_bytes(x));
    }

    void    page_dev_fragments(WebHtml& h)
    {
        h.title("Listing of Fragments");
        dev_table(h, all_fragments(Sorted::YES));
    }

    void    page_dev_hello(WebHtml& out)
    {
        out.title("HELLO WORLD!");
        
        auto _t     = out.table();
        
        out.kvrow("Method") << out.context().method.key() << "\n";
        out.kvrow("URL") << out.context().url << "\n";
        out.kvrow("Client HTTP") << out.context().version << "</td></tr>\n";
        for(auto& hv : out.context().rx_headers)
            out.kvrow(hv.first) << hv.second;
    }
    
    void page_dev_image(WebHtml& h)
    {
        Image   img = image(h);
        if(!img)
            throw HttpStatus::BadArgument;
        
        Fragment    frag    = fragment(img);
        Document    doc     = document(img);
        const Root* rt      = root(frag);
        if(!rt)
            throw HttpStatus::BadArgument;
        
        h.title() << "Image [" << rt->key << "](" << label(doc) << ")";
        h << img;
    }

    void page_dev_images(WebHtml& h)
    {
        h.title("All Images");
        dev_table(h, all_images());
    }

    void page_dev_leaf(WebHtml&h)
    {
        Leaf    x   = leaf(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        auto i = cdb::info(x);
            
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

    void    page_dev_leafs(WebHtml&h)
    {
        h.title("All Leafs");
        dev_table(h, all_leafs(Sorted::YES));
    }


    void page_dev_meta_object(WebHtml&h)
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
    
    void page_dev_meta_object_all_base(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Bases";
        dev_table(h, obj->bases(true).all);
    }

    void page_dev_meta_object_all_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Derived";
        dev_table(h, obj->deriveds(true).all);
    }

    void page_dev_meta_object_all_properties(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): All Properties";
        dev_table(h, obj->properties(true).all);
    }

    void page_dev_meta_object_derived(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Derived";
        dev_table(h, obj->deriveds(false).all);
    }

    void page_dev_meta_object_properties(WebHtml&h)
    {
        const ObjectInfo*   obj = object_info(h);
        if(!obj)
            throw HttpStatus::BadArgument;
            
        h.title() << "Object Info (" << obj -> name() << "): Properties";
        dev_table(h, obj->properties(false).all);
    }
    
    void    page_dev_meta_objects(WebHtml&h)
    {
        h.title() << "All Meta Objects";
        dev_table(h, ObjectInfo::all());
    }

    void    page_dev_meta_type(WebHtml& h)
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
    
    void    page_dev_meta_type_properties(WebHtml& h)
    {
        const TypeInfo* type    = type_info(h);
        if(!type)
            throw HttpStatus::BadArgument;
        
        h.title() << "Type Info (" << type->name() << "): Properties";
        dev_table(h, type->properties());
    }
    
    void page_dev_meta_types(WebHtml&h)
    {
        h.title() << "All Type Infos";
        dev_table(h, TypeInfo::all());
    }

    void page_dev_mimetypes(WebHtml&h)
    {
        h.title("Mime Types");
        auto ta = h.table();
        h << "<tr><th>ID</th><th>KEY</th><th>MIME-TYPE</th></tr>\n";
        for(ContentType ct : ContentType::all_values())
            h << "<tr><td>" << ct.value() << "</td><td>" << ct.key() << "</td><td>" << mimeType(ct) << "</td></tr>\n";
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
    
    void    page_dev_roots(WebHtml& h)
    {
        h.title() << "All Roots";
        dev_table(h, wksp::roots());
    }

    void page_dev_server(WebHtml&h)
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
            for(const Root* rt : wksp::roots())
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

    void page_dev_sql_table(WebHtml& h)
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
        SqlQuery::Result    r;
        while((r = sql.step()) == SqlQuery::Row){
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
    
    void page_dev_sql_tables(WebHtml& out)
    {
        auto list = out.numbers();
        for(auto& s : wksp::db().tables())
            out.li() << "<a href=\"/dev/sql/table?table=" << web_encode(s) << "\">" << s << "</a>\n";
    }
    
    void    page_dev_tag(WebHtml& h)
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
        h.kvrow("Leaf"); //  << dev(leaf(x_tag));
        h.kvrow("Image");
    }
    
    void    page_dev_tag_classes(WebHtml& h)
    {
        Tag x   = tag(h);
        if(!x)
            throw HttpStatus::BadArgument;
        
        dev_title(h, x, "Classes");
        dev_table(h, classes(x));
    }

    void    page_dev_tags(WebHtml& out)
    {
        out.title("All Tags");
        dev_table(out, all_tags());
    }

    void    dev_page_users(WebHtml& h)
    {
        h.title() << "All Users";
        dev_table(h, cdb::all_users(Sorted::YES));
    }

    void    page_dev_webdirs(WebHtml&h)
    {
        h.title("All WebDirs");
        dev_table(h, web::directory_map());
    }

    void page_dev_webexts(WebHtml& h)
    {
        h.title("All WebExts");
        dev_table(h, web::extension_map());
    }
    
    void page_dev_webglobs(WebHtml& h)
    {
        h.title("All WebGlobs");
        dev_table(h, web::glob_map());
    }

    void page_dev_webpages(WebHtml& h)
    {
        h.title("All WebPages");
        dev_table(h, web::page_map(), true);
    }

    void page_dev_webvars(WebHtml& h)
    {
        h.title("All WebVariables");
        dev_table(h, web::variable_map());
    }

    void  page_dev_wksp(WebHtml& h)
    {
        h.title() << "Workspace '" << wksp::name() << "'";
        
        auto t = h.table();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Color (Background)") << gBkColor.get();
        h.kvrow("Color (Text)") << gTextColor.get();
        //h.kvrow("Config") << dev(cdb::config_folder());
        h.kvrow("Copyright") << wksp::copyright().text;
        h.kvrow("Key") << wksp::quill_key();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        h.kvrow("Start") << wksp::start();
        h.kvrow("Templates") << join(wksp::templates(), ", ");
    }

    void    page_dev_wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::roots());
    }

    void    page_extension_markdown( WebHtml& out )
    {
        WebContext&ctx  = out.context();
        std::string     bytes   = file_string(ctx.resolved_file);
        if(bytes.empty())
            throw hNotFound;

        std::string_view    t   = web_title(bytes);
        std::string_view    d;
        
        if(!t.empty()){
            const char* z0  = t.data()+t.size();
            const char* z1  = bytes.data() + bytes.size();
            d               = std::string_view(z0, z1);
            ctx.var_title   = trimmed(t);
        } else {
            d               = bytes;
            ctx.var_title   = ctx.resolved_file.filename().string();
        }

        stream::Text    render(ctx.var_body);
        Markdown::Config    config;
        Markdown            parser(render, config);
        parser.process(d);
    }

    void    page_image(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;

        ByteArray   by   = bytes(img, SizeDesc::Original);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }

    void    page_leaf(WebHtml& h)
    {
        //  BIG FAT TODO!
    }

    void    page_thumbnail(WebContext& ctx)
    {
        Image   img = image(ctx);
        if(!img)
            throw HttpStatus::BadArgument;
        
        bool    detected    = false;
        SizeDesc    sz  = size_desc(ctx, "size", &detected);
        if(!detected)
            sz      = ctx.session.icon_size;
        
        ByteArray   by   = bytes(img, sz);
        ctx.tx_content_type = mime_type(img);
        ctx.tx_content      = std::make_shared<ByteArray>(by);
    }

    void    page_user(WebHtml& h)
    {
        User    u   = arg::user(h);
        if(!u)
            throw HttpStatus::BadArgument;
        
        h.title() << "User: " << cdb::name(u);
        h.p() << "Fill in the blank... TODO!";
    }

    void page_wksp(WebHtml& h)
    {
        //  NOTE ... REPLACE THIS WITH A STATIC-PAGE TEMPLATE
        
        h.title(wksp::name());
        if(h.context().session.can_edit)
            h << "<font color=\"red\">Warning: Changes here (may) require a restart of the server.</font>\n";
        
        auto t = h.table();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Copyright") << wksp::copyright().text;
    }
    
    void    page_wksp_classes(WebHtml& h)
    {
        h << "TODO";
    }

    void    reg_me()
    {
        reg_webpage<page__index>("/");
        reg_webpage<page_admin_users>("/admin/users"); 
        reg_webpage<page_api_workspace>("/api/workspace"sv); 
        reg_webpage<page_atom>("/atom").argument("ID", "Atom ID");
        reg_webpage<page_atoms>("/atoms");
        reg_webpage<page_background>("/background");
        reg_webgroup({
            reg_webpage<page_class>("/class").argument("ID", "Class ID").label("Overview"),
            reg_webpage<page_class_atoms>("/class/atoms").argument("ID", "Class ID").label("Atoms")
        });
        reg_webpage<page_css>("/css");
        reg_webpage("/dev/**", wksp::shared_all("www/dev"sv));
        reg_webgroup({
            reg_webpage<page_dev_atom>("/dev/atom").argument("id", "Atom ID").label("Info"),
            reg_webpage<page_dev_atom_classes>("/dev/atom/classes").argument("id", "Atom ID").label("Classes"),
            reg_webpage<page_dev_atom_documents>("/dev/atom/documents").argument("id", "Atom ID").label("Docs"),
            reg_webpage<page_dev_atom_tags>("/dev/atom/tags").argument("id", "Atom ID").label("Tags")
        });
        reg_webpage<page_dev_atoms>("/dev/atoms");
        reg_webpage<page_dev_categories>("/dev/categories"); 
        reg_webgroup({
            reg_webpage<page_dev_category>("/dev/category").argument("id", "Category ID").label("Info"),
            reg_webpage<page_dev_category_classes>("/dev/category/classes").argument("id", "Category ID").label("Classes"),
            reg_webpage<page_dev_category_fields>("/dev/category/fields").argument("id", "Category ID").label("Fields")
        });
        reg_webgroup({
            reg_webpage<page_dev_class>("/dev/class").argument("id", "Class ID").label("Info"),
            reg_webpage<page_dev_class_tags>("/dev/class/tags").argument("id", "Class ID").label("Tags"),
            reg_webpage<page_dev_class_def_fields>("/dev/class/def_fields").argument("id", "Class ID").label("Def Fields")
        });
        reg_webpage<page_dev_classes>("/dev/classes");
        reg_webpage<page_dev_directories>("/dev/directories");
        reg_webgroup({
            reg_webpage<page_dev_directory>("/dev/directory").argument("id", "Directory ID").label("Info"),
            reg_webpage<page_dev_directory_fragments>("/dev/directory/fragments").argument("id", "Directory ID").label("Frags"),
            reg_webpage<page_dev_directory_subdirs>("/dev/directory/subdirs").argument("id", "Directory ID").label("Subdirs")
        });
        reg_webgroup({
            reg_webpage<page_dev_document>("/dev/document").argument("id", "Document ID").label("Info"),
            reg_webpage<page_dev_document_attributes>("/dev/document/attributes").argument("id", "Document ID").label("Attributes"),
            reg_webpage<page_dev_document_fragments>("/dev/document/fragments").argument("id", "Document ID").label("Fragments"),
            reg_webpage<page_dev_document_roots>("/dev/document/roots").argument("id", "Document ID").label("Roots")
        });
        reg_webpage<page_dev_documents>("/dev/documents"); 
        reg_webpage<page_dev_echo>({hGet, hPost}, "/dev/echo");
        reg_webgroup({
            reg_webpage<page_dev_field>("/dev/field").argument("id", "Field ID").label("Info"),
            reg_webpage<page_dev_field_tags>("/dev/field/tags").argument("id", "Field ID").label("Fields")
        });
        reg_webpage<page_dev_fields>("/dev/fields");
        reg_webgroup({
            reg_webpage<page_dev_folder>("/dev/folder").label("Info"),
            reg_webpage<page_dev_folder_directories>("/dev/folder/dirs").label("Dirs"),
            reg_webpage<page_dev_folder_documents>("/dev/folder/docs").label("Docs"),
            reg_webpage<page_dev_folder_fragments>("/dev/folder/frags").label("Frags"),
            reg_webpage<page_dev_folder_subs>("/dev/folder/subs").label("Subs")
        });
        reg_webpage<page_dev_folders>("/dev/folders");
        reg_webgroup({
            reg_webpage<page_dev_fragment>("/dev/fragment").description("Developer info for a fragment").argument("id", "Fragment ID").label("Info"),
            reg_webpage<page_dev_fragment_content>("/dev/fragment/content").description("Content for fragment").argument("id", "Fragment ID").label("Content")
        });
        reg_webpage<dev_fragment_image>("/dev/fragment/image").argument("id", "ID for fragment"); 
        reg_webpage<page_dev_fragments>("/dev/fragments");
        reg_webpage<page_dev_hello>("/dev/hello");
        reg_webpage<page_dev_image>("/dev/image");
        reg_webpage<page_dev_images>("/dev/images"); 
        reg_webgroup({
            reg_webpage<page_dev_leaf>("/dev/leaf").argument("id", "Leaf ID").label("Info")
        });
        reg_webpage<page_dev_leafs>("/dev/leafs");
        reg_webgroup({
            reg_webpage<page_dev_meta_object>("/dev/meta/object").label("Info"),
            reg_webpage<page_dev_meta_object_derived>("/dev/meta/object/derived").label("Derived"),
            reg_webpage<page_dev_meta_object_properties>("/dev/meta/object/properties").label("Properties"),
            reg_webpage<page_dev_meta_object_all_base>("/dev/meta/object/all_base").label("All Base"),
            reg_webpage<page_dev_meta_object_all_derived>("/dev/meta/object/all_derived").label("All Derived"),
            reg_webpage<page_dev_meta_object_all_properties>("/dev/meta/object/all_properties").label("All Properties")
        });
        reg_webpage<page_dev_meta_objects>("/dev/meta/objects"); 
        reg_webgroup({
            reg_webpage<page_dev_meta_type>("/dev/meta/type").label("Info"),
            reg_webpage<page_dev_meta_type_properties>("/dev/meta/type/properties").label("Properties")
        });
        reg_webpage<page_dev_meta_types>("/dev/meta/types");
        reg_webpage<page_dev_mimetypes>("/dev/mimetypes");
        reg_webgroup({
            reg_webpage<page_dev_root>("/dev/root").argument("id", "Root ID").label("Info"),
            reg_webpage<page_dev_root_directories>("/dev/root/dirs").argument("id", "Root ID").label("Dirs"),
            reg_webpage<page_dev_root_fragments>("/dev/root/frags").argument("id", "Root ID").label("Frags"),
            reg_webpage<page_dev_root_all_directories>("/dev/root/all_dirs").argument("id", "Root ID").label("AllDirs"),
            reg_webpage<page_dev_root_all_fragments>("/dev/root/all_frags").argument("id", "Root ID").label("AllFrags")
        });
        reg_webpage<page_dev_roots>("/dev/roots");
        reg_webpage<page_dev_server>("/dev/server").local();
        reg_webpage<page_dev_sql_table>("/dev/sql/table").local().argument("table", "SQL Table Name");
        reg_webpage<page_dev_sql_tables>("/dev/sql/tables", "SQL Tables").local();
        reg_webgroup({
            reg_webpage<page_dev_tag>("/dev/tag")
                .argument("id", "Tag id (number)")
                .argument("key", "Tag key (string)")
                .argument("tag", "Tag key/identifier").label("Info"),
            reg_webpage<page_dev_tag_classes>("/dev/tag/classes").argument("id", "Tag id (number)").label("Classes")
        });
        reg_webpage<page_dev_tags>("/dev/tags");
        reg_webpage<dev_page_users>("/dev/users");
        reg_webpage<page_dev_webdirs>("/dev/web/directories");
        reg_webpage<page_dev_webexts>("/dev/web/extensions"); 
        reg_webpage<page_dev_webglobs>("/dev/web/globs");
        reg_webpage<page_dev_webpages>("/dev/web/pages");
        reg_webpage<page_dev_webvars>("/dev/web/variables");
        reg_webgroup({
            reg_webpage<page_dev_wksp>("/dev/wksp").label("Info"),
            reg_webpage<page_dev_wksp_roots>("/dev/wksp/roots").label("Roots")
        });
        reg_webpage<page_extension_markdown>("*.md");
        reg_webpage("/favicon.ico", wksp::shared("www/img/yquill.svg"sv)).todo();
        reg_webpage("/help/**", wksp::shared_all("www/help"sv));
        reg_webpage<page_image>("/image").argument("id", "ID for the image");
        reg_webpage("/img/**", wksp::shared_all("www/img"sv));
        reg_webpage("/js/**", wksp::shared_all("www/js"sv));
        reg_webpage("/logo", wksp::shared("www/img/yquill.svg"sv)).todo();
        reg_webpage<page_thumbnail>("/thumbnail").argument("id", "ID for the image");
        reg_webpage<page_user>("/user");
        reg_webgroup({
            reg_webpage<page_wksp>("/wksp").label("Info")
        });
    }
    
    
    YQ_INVOKE(reg_me();)
}



