////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Atom.hpp"
#include "AtomArg.hpp"
#include "AtomCDB.hpp"
#include "AtomHtml.hpp"

#include "Class.hpp"
#include "ClassArg.hpp"
#include "ClassCDB.hpp"
#include "ClassData.hpp"
#include "ClassFile.hpp"
#include "ClassHtml.hpp"

#include "Field.hpp"
#include "FieldArg.hpp"
#include "FieldCDB.hpp"
#include "FieldData.hpp"
#include "FieldFile.hpp"
#include "FieldHtml.hpp"

#include "Value.hpp"
#include "ValueCDB.hpp"
#include "ValueData.hpp"
#include "ValueFile.hpp"
#include "ValueHtml.hpp"

#include <yq/bit/KeyValue.hpp>
#include <yq/collection/c_utils.hpp>
#include <yq/db/CacheFwd.hpp>
#include <yq/db/SQ.hpp>
#include <yq/db/NKI.hpp>
#include <yq/file/DocumentCDB.hpp>
#include <yq/file/DocumentHtml.hpp>
#include <yq/file/FolderCDB.hpp>
#include <yq/file/FragmentCDB.hpp>
#include <yq/file/Root.hpp>
#include <yq/image/ImageCDB.hpp>
#include <yq/io/file_utils.hpp>
#include <yq/io/Strings.hpp>
#include <yq/io/XmlUtils.hpp>
#include <yq/meta/TypeInfo.hpp>
#include <yq/org/CategoryCDB.hpp>
#include <yq/org/TagCDB.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/wksp/Workspace.hpp>

#include <yq/db/IDLock.ipp>


namespace yq {
    template class IDLock<Atom>;
    template class IDLock<Class>;
    template class IDLock<Field>;

////////////////////////////////////////////////////////////////////////////////
//  (ATOM)
////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Atom atom(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Atom{};
                
            Atom t   = cdb::atom( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_atom(i))
                return Atom{i};
            return Atom{};
        }
        
        Atom atom(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return atom_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return atom_key(k);
            
            k       = ctx.find_query("atom");
            if(!k.empty())
                return atom(k);
            return Atom{};
        }
        
        
        Atom atom(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom(arg_string);
        }
        
        Atom atom(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom(arg_string);
        }

        Atom atom_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_atom(i))
                return Atom{i};
            return Atom{};
        }

        Atom atom_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_id(arg_string);
        }
        
        Atom atom_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_id(arg_string);
        }
        
        Atom atom_key(std::string_view arg_string)
        {
            return cdb::atom(trimmed(arg_string));
        }
        
        Atom atom_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_key(arg_string);
        }
        
        Atom atom_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_key(arg_string);
        }
    }
    
////////////////////////////////////////////////////////////////////////////////

    namespace cdb {
        namespace {
            std::string     full_key(Document d, std::string_view  ck)
            {
                std::string     kk  = key(d);
                std::string     dk = std::string(strip_extension(kk));
                if(dk.empty())    
                    return std::string();

                if(!ck.empty()){
                    dk += '#';
                    dk += ck;
                }
                return dk;
            }
        }


        std::string             abbreviation(Atom a)
        {
            static thread_local SQ s("SELECT abbr FROM Atoms WHERE id=?");
            return s.str(a.id);
        }

        namespace {
            std::vector<Atom>        all_atoms_sorted()
            {
                static thread_local SQ s("SELECT id FROM Atoms ORDER BY k");
                return s.vec<Atom>();
            }

            std::vector<Atom>        all_atoms_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Atoms");
                return s.vec<Atom>();
            }
        }
        

        std::vector<Atom>        all_atoms(Sorted sorted)
        {
            return sorted ? all_atoms_sorted() : all_atoms_unsorted();
        }
        
        namespace {
            std::vector<Atom>    all_atoms_sorted(Class c)
            {
                        // I think this SQL is right.....
                static thread_local SQ s("SELECT atom FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Atom>(c.id);
            }

            std::vector<Atom>    all_atoms_unsorted(Class c)
            {
                static thread_local SQ s("SELECT atom FROM AClasses WHERE class=?");
                return s.vec<Atom>(c.id);
            }
        }
        
        std::vector<Atom>        all_atoms(Class c,Sorted sorted)
        {
            return sorted ? all_atoms_sorted(c) : all_atoms_unsorted(c);
        }
        
        namespace {
            std::vector<Atom>    all_atoms_sorted(Tag t)
            {
                static thread_local SQ s("SELECT atom FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE tag=? ORDER BY Tags.k");
                return s.vec<Atom>(t.id);
            }

            std::vector<Atom>    all_atoms_unsorted(Tag t)
            {
                static thread_local SQ s("SELECT atom FROM ATags WHERE tag=?");
                return s.vec<Atom>(t.id);
            }
        }
        
        std::vector<Atom>        all_atoms(Tag t,Sorted sorted)
        {
            return sorted ? all_atoms_sorted(t) : all_atoms_unsorted(t);
        }
        
        size_t              all_atoms_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms");
            return s.size();
        }
        
        size_t              all_atoms_count(Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE class=?");
            return s.size(c.id);
        }
        
        size_t              all_atoms_count(Tag t)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE tag=?");
            return s.size(t.id);
        }
        
        
        Atom                atom(uint64_t i)
        {
            return exists_atom(i) ? Atom{i} : Atom();
        }
        
        Atom                atom(std::string_view  k)
        {
            static thread_local SQ s("SELECT id FROM Atoms WHERE k=? LIMIT 1");
            return s.as<Atom>(k);
        }

        Atom                atom(Document doc, std::string_view  ck)
        {
            std::string ks  = full_key(doc, ck);
            return atom(ks);
        }
        
        
        namespace {
            std::vector<Atom>        atoms_sorted(Atom a)
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE parent=? ORDER BY k");
                return s.vec<Atom>(a.id);
            }
            
            std::vector<Atom>        atoms_unsorted(Atom a)
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE parent=?");
                return s.vec<Atom>(a.id);
            }
            
        }

        std::vector<Atom>            atoms(Atom a, Sorted sorted)
        {
            return sorted ? atoms_sorted(a) : atoms_unsorted(a);
        }

        namespace {
            std::vector<Atom>    atoms_sorted(Document doc)
            {
                static thread_local SQ s("SELECT atom FROM ADocuments INNER JOIN Atoms ON ADocuments.atom=Atoms.id WHERE doc=? ORDER BY Atoms.k");
                return s.vec<Atom>(doc.id);
            }

            std::vector<Atom>    atoms_unsorted(Document doc)
            {
                static thread_local SQ s("SELECT atom FROM ADocuments WHERE doc=?");
                return s.vec<Atom>(doc.id);
            }
        }
        
        std::vector<Atom>        atoms(Document doc, Sorted sorted)
        {
            return sorted ? atoms_sorted(doc) : atoms_unsorted(doc);
        }
        
        namespace {
            std::vector<Atom>     atoms_by_name_sorted(std::string_view  n)
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE name=? ORDER BY k");
                return s.vec<Atom>(n);
            }
            
            std::vector<Atom>     atoms_by_name_unsorted(std::string_view  n)
            {
                static thread_local SQ s("SELECT id FROM Atoms WHERE name=?");
                return s.vec<Atom>(n);
            }
        }
        
        
        std::vector<Atom>             atoms_by_name(std::string_view  n, Sorted sorted)
        {
            return sorted ? atoms_by_name_sorted(n) : atoms_by_name_unsorted(n);
            std::vector<Atom>     ret;
        }

        size_t              atoms_count(Document doc)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Atoms WHERE doc=?");
            return s.size(doc);
        }
        
        std::string             brief(Atom a)
        {
            static thread_local SQ s("SELECT brief FROM Atoms WHERE id=?");
            return s.str(a.id);
        }
        
        namespace {
            std::vector<Class>    classes_sorted(Atom a)
            {
                static thread_local SQ s("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? ORDER BY Classes.k");
                return s.vec<Class>(a.id);
            }
            
            std::vector<Class>    classes_unsorted(Atom a)
            {
                static thread_local SQ s("SELECT class FROM AClasses WHERE atom=?");
                return s.vec<Class>(a.id);
            }
        }
        
        
        std::vector<Class>       classes(Atom a, Sorted sorted)
        {
            return sorted ? classes_sorted(a) : classes_unsorted(a);
        }
        
        namespace {
            std::vector<Class>    classes_sorted(Atom a, Document d)
            {
                static thread_local SQ s("SELECT class FROM AClasses INNER JOIN Classes ON AClasses.class=Classes.id WHERE atom=? AND doc=? ORDER BY Classes.k");
                return s.vec<Class>(a.id, d.id);
            }

            std::vector<Class>    classes_unsorted(Atom a, Document d)
            {
                static thread_local SQ s("SELECT class FROM AClasses WHERE atom=? AND doc=?");
                return s.vec<Class>(a.id, d.id);
            }
        }
        
        std::vector<Class>       classes(Atom a, Document d, Sorted sorted)
        {
            return sorted ? classes_sorted(a,d) : classes_unsorted(a,d);
        }
        
        size_t              classes_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(DISTINCT class) FROM AClasses WHERE atom=?");
            return s.size(a.id);
        }    
        
        size_t              classes_count(Atom a, Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM AClasses WHERE atom=? AND doc=?");
            return s.size(a.id, d.id);
        }    


        Atom                db_atom(Document d, bool* wasCreated)
        {
            return db_atom(d, std::string(), wasCreated);
        }
        
        Atom                db_atom(Document d, std::string_view ck, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;


            std::string     dk = full_key(d, ck);
            if(dk.empty()){
                yError() << "Cannot create to an empty key!";
                return Atom();
            }

            static thread_local SQ    i("INSERT OR FAIL INTO Atoms (k,doc,sk) VALUES (?,?,?)");
            static thread_local SQ    s("SELECT id FROM Atoms WHERE k=?");
            auto i_af   = i.af();
            auto s_af   = s.af();
            i.bind(1, dk);
            i.bind(2, d.id);
            i.bind(3, ck);
            
            if(i.exec()){
                if(wasCreated)
                    *wasCreated = true;
                return Atom{(uint64_t) i.last_id()};
            } else {
                s.bind(1, dk);
                if(s.step() == SqlQuery::Row)
                    return Atom{s.v_uint64(1)};
                yError() << "Unable to get the atom from the database: " << dk;
                return Atom();
            }
        }
        
        //Document            document(Atom a)
        //{
            //static thread_local SQ s("SELECT doc FROM Atoms WHERE id=?");
            //auto s_af = s.af();
            //s.bind(0, a.id);
            //if(s.exec() && s.next())
                //return Document{s.valueU64(0)};
            //return Document();
        //}
        
        namespace {
            std::vector<Document>  documents_sorted(Atom a)
            {
                static thread_local SQ  s("SELECT doc FROM ADocuments INNER JOIN Documents ON ADocuments.doc=Documents.id WHERE id=? ORDER BY Documents.k");
                return s.vec<Document>(a.id);
            }
            
            std::vector<Document>  documents_unsorted(Atom a)
            {
                static thread_local SQ  s("SELECT doc FROM ADocuments WHERE id=?");
                return s.vec<Document>(a.id);
            }
            
        }
        
        std::vector<Document>        documents(Atom a, Sorted sorted)
        {
            return sorted ? documents_sorted(a) : documents_unsorted(a);
        }
        
        size_t                  documents_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ADocuments WHERE id=?");
            return s.size(a.id);
        }

       bool                exists(Atom a)
        {
            return exists_atom(a.id);
        }
        
       
        bool                exists_atom(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Atoms WHERE id=?");
            return s.present(i);
        }

        
       
        Image               icon(Atom a) 
        {
            static thread_local SQ s("SELECT icon FROM Atoms WHERE id=?");
            return s.as<Image>(a.id);
        }
        
        std::vector<Atom>   inbound(Atom a)
        {
            static thread_local SQ s("SELECT atom FROM AEdges WHERE tgtid=?");
            return s.vec<Atom>(a.id);
        }

        Atom::Info          info(Atom a)
        {
            Atom::Info    ret;
            
            static thread_local SQ s("SELECT k, abbr, brief, name, leaf FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
                ret.key         = s.v_string(1);
                ret.abbr        = s.v_string(2);
                ret.brief       = s.v_string(3);
                ret.name        = s.v_string(4);
                ret.brief       = Leaf{s.v_uint64(5)};
            }
            return ret;
        }
        
        
        bool                is(Atom a, Class c)
        {
            static thread_local SQ    s("SELECT 1 FROM AClasses WHERE atom=? AND class=?");
            return s.present(a.id, c.id);
        }
        
        //bool                is_all(Atom a, std::initializer_list<Class>cs)
        //{
            //for(Class c : cs)
                //if(!is(a, c))
                    //return false;
            //return true;
        //}
        
        //bool                is_any(Atom a, std::initializer_list<Class>cs)
        //{
            //for(Class c : cs)
                //if(is(a, c))
                    //return true;
            //return false;
        //}
        
     
        std::string             key(Atom a)
        {
            static thread_local SQ s("SELECT k FROM Atoms WHERE id=?");
            return s.str(a.id);
        }
        
        std::string             label(Atom a)
        {
            static thread_local SQ    s("SELECT ifnull(name,k) FROM Atoms WHERE id=?");
            return s.str(a.id);
        }
        
        
        Leaf                    leaf(Atom a)
        {
            static thread_local SQ s("SELECT leaf FROM Atoms WHERE id=?");
            return s.as<Leaf>(a.id);
        }

        std::string             name(Atom a)
        {
            static thread_local SQ s("SELECT name FROM Atoms WHERE id=?");
            return s.str(a.id);
        }

        
        NKI                 nki(Atom a, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Atoms WHERE id=?");
            auto s_af = s.af();
            s.bind(1, a.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image{s.v_uint64(2)};
                ret.key     = s.v_string(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI();
        }
        

        std::vector<Atom>   outbound(Atom a)
        {
            static thread_local SQ s("SELECT target FROM AEdges WHERE atom=? AND tgtid!=0");
            return s.vec<Atom>(a.id);
        }

        Atom                parent(Atom a)
        {
            static thread_local SQ s("SELECT parent FROM Atoms WHERE id=?");
            return s.as<Atom>(a.id);
        }
        

        std::string             skey(Atom a) 
        {
            static thread_local SQ s("SELECT sk FROM Atoms WHERE id=?");
            return s.str(a.id);
        }

        namespace {
            std::vector<Tag>          tags_sorted(Atom a)
            {
                static thread_local SQ s("SELECT DISTINCT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? ORDER BY Tags.k");
                return s.vec<Tag>(a.id);
            }

            std::vector<Tag>          tags_unsorted(Atom a)
            {
                static thread_local SQ s("SELECT DISTINCT tag FROM ATags WHERE atom=?");
                return s.vec<Tag>(a.id);
            }
        }
        
        std::vector<Tag>              tags(Atom a, Sorted sorted)
        {
            return sorted ? tags_sorted(a) : tags_unsorted(a);
        }
        
        namespace {
            std::vector<Tag>          tags_sorted(Atom a, Document d)
            {
                static thread_local SQ s("SELECT tag FROM ATags INNER JOIN Tags ON ATags.tag=Tags.id WHERE atom=? AND doc=? ORDER BY Tags.k");
                return s.vec<Tag>(a.id,d.id);
            }

            std::vector<Tag>          tags_unsorted(Atom a, Document d)
            {
                static thread_local SQ s("SELECT tag FROM ATags WHERE atom=? AND doc=?");
                return s.vec<Tag>(a.id,d.id);
            }
        }
        

        std::vector<Tag>              tags(Atom a, Document d, Sorted sorted)
        {
            return sorted ? tags_sorted(a,d) : tags_unsorted(a,d);
        }

        bool                tagged(Atom a, Tag t)
        {
            static thread_local SQ s("SELECT 1 FROM ATags WHERE atom=? AND tag=? LIMIT 1");
            return s.present(a.id, t.id);
        }
        
        size_t              tags_count(Atom a)
        {
            static thread_local SQ s("SELECT COUNT(DISTINCT tag) FROM ATags WHERE atom=?");
            return s.size(a.id);
        }

        size_t              tags_count(Atom a, Document d)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM ATags WHERE atom=? AND doc=?");
            return s.size(a.id, d.id);
        }

        std::string             title(Atom a)
        {
            return label(a);
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Atom v)
        {
            //  more TODO...
            h << cdb::label(v);
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Atom> v)
        {
            if(v.data)
                h << "<a href=\"/dev/atom?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const std::vector<Atom>& atoms)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Brief</th></tr>\n";
            for(Atom a : atoms){
                auto i = cdb::info(a);
                h << "<tr><td>" << dev_id(a) << "</td><td>" << i.key << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  (ATOM) CLASS(IFICATIONS)
////////////////////////////////////////////////////////////////////////////////


    Class::Data&      Class::Data::merge(const Class::Data&b, bool fOverride)
    {
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(folder, b.folder, fOverride);
        set_if_empty(category, b.category, fOverride);
        use += b.use;
        reverse += b.reverse;
        sources += b.sources;
        targets += b.targets;
        set_if_empty(binding, b.binding, fOverride);
        suffixes += b.suffixes;
        prefixes += b.prefixes;
        aliases += b.aliases;
        tags += b.tags;
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void    Class::File::reset() 
    {
        *(Data*) this   = Data{};
    }

    
    #if CLASS_XML_RESAVE
    bool    Class::File::read(const XmlDocument&doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQClass);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szClass);
        if(!xn){
            yError() << "No class! In: " << fname << "'";
            return false;
        }
        
        name            = read_child(xn, szName, x_string);
        plural          = read_child(xn, szPlural, x_string);
        brief           = read_child(xn, szBrief, x_string);
        notes           = read_child(xn, szNotes, x_string);
        folder          = read_child(xn, szFolder, x_string);
        use             = read_child_string_set(xn, szIs);
        reverse         = read_child_string_set(xn, szReverse);
        sources         = read_child_string_set(xn, szSource);
        targets         = read_child_string_set(xn, szTarget);
        aliases         = read_child_string_set(xn, szAlias);
        binding         = read_child(xn, szBinding, x_string);
        prefixes        = read_child_string_set(xn, szPrefix);
        suffixes        = read_child_string_set(xn, szSuffix);
        tags            = read_child_string_set(xn, szTag);
        category        = read_child(xn, szCategory, x_string);
        return true;
    }
    #endif
    
    bool    Class::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name            = attrs.value(kv::key({"%", "name"}));
        plural          = attrs.value("plural");
        brief           = attrs.value("brief");
        notes           = attrs.value("notes");
        folder          = attrs.value("folder");
        use            += attrs.values_set("is");
        reverse        += attrs.values_set("reverse");
        sources        += attrs.values_set("source");
        targets        += attrs.values_set("target");
        aliases        += attrs.values_set("alias");
        binding         = attrs.value("binding");
        prefixes       += attrs.values_set("prefix");
        suffixes       += attrs.values_set("suffix");
        tags           += attrs.values_set("tag");
        category        = attrs.value("category");
        return true;
    }
    
    bool    Class::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!plural.empty())
            attrs.set("plural", plural);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(!use.empty())
            attrs.set("is", join(use, ","));
        if(!reverse.empty())
            attrs.set("reverse", join(reverse, ","));
        if(!targets.empty())
            attrs.set("target", join(targets, ","));
        if(!sources.empty())
            attrs.set("source", join(sources, ","));
        if(!prefixes.empty())
            attrs.set("prefix", join(prefixes, ","));
        if(!suffixes.empty())
            attrs.set("suffix", join(suffixes, ","));
        if(!aliases.empty())
            attrs.set("alias", join(aliases, ",") );
        if(!binding.empty())
            attrs.set("binding", binding);
        if(!tags.empty())
            attrs.set("tag", join(tags, ","));
        if(!category.empty())
            attrs.set("category", category);
        return true;
    }


////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Class class_(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Class{};
                
            Class t   = cdb::class_( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_class(i))
                return Class{i};
            return Class{};
        }
        
        Class class_(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return class_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return class_key(k);
            
            k       = ctx.find_query("class");
            if(!k.empty())
                return class_(k);
            return Class{};
        }
        
        
        Class class_(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_(arg_string);
        }
        
        Class class_(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_(arg_string);
        }

        Class class_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_class(i))
                return Class{i};
            return Class{};
        }

        Class class_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_id(arg_string);
        }
        
        Class class_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_id(arg_string);
        }
        
        Class class_key(std::string_view arg_string)
        {
            return cdb::class_(trimmed(arg_string));
        }
        
        Class class_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return class_key(arg_string);
        }
        
        Class class_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return class_key(arg_string);
        }
    }

////////////////////////////////////////////////////////////////////////////////


    bool Class::less_key(Class a, Class b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Class::less_label(Class a, Class b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Class::less_name(Class a, Class b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }


    namespace cdb {

        namespace {
            inline std::string class_filename(std::string_view k)
            {
                return make_filename(k, Class::szExtension);
            }
        }

        namespace {
            std::vector<Class>    all_classes_sorted()
            {
                static thread_local SQ    s("SELECT id FROM Classes ORDER BY K");
                return s.vec<Class>();
            }
            
            std::vector<Class>    all_classes_unsorted()
            {
                static thread_local SQ    s("SELECT id FROM Classes");
                return s.vec<Class>();
            }
        }

        std::vector<Class>  all_classes(Sorted sorted)
        {
            return sorted ? all_classes_sorted() : all_classes_unsorted();
        }

        size_t  all_classes_count()
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM Classes");
            return s.size();
        }

        string_set_t        alternative_keys(Class c)
        {
            static thread_local SQ   s("SELECT k FROM CLookup WHERE class=? AND priority=0");
            return s.sset(c.id);
        }
        
        std::string  binding(Class c)
        {
            static thread_local SQ  s("SELECT binding FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        std::string  brief(Class c)
        {
            static thread_local SQ    s("SELECT brief FROM Classes WHERE id=?");
            return s.str(c.id);
        }
        
        Category  category(Class c)
        {
            static thread_local SQ s("SELECT category FROM Classes WHERE id=?");
            return s.as<Category>(c.id);
        }

        Class  class_(uint64_t i)
        {
            return exists_class(i) ? Class{i} : Class();
        }

        Class  class_(Document doc, bool calc)
        {
            if(!doc)
                return Class();
            if(exists_class(doc.id))
                return Class{doc.id};
            if(calc && (folder(doc) == classes_folder())){
                std::string k   = base_key(doc);
                return class_(k);
            }
            
            return Class();
        }
        
        Class  class_(std::string_view  k)
        {
            static thread_local SQ s("SELECT class, priority FROM CLookup WHERE k=? ORDER BY priority DESC LIMIT 1");
            return s.as<Class>(k);
        }

        Class::SharedFile  class_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return Class::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return Class::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<Class::File>();
                return Class::SharedFile();
            }
                
            Class::SharedFile  td = std::make_shared<Class::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return Class::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }
        

        std::vector<Class>       classes(const string_set_t& sset, bool noisy)
        {
            std::vector<Class>   ret;
            for(const std::string& s : sset){
                if(s.empty())
                    continue;
                    
                Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.push_back(cls);
            }
            return ret;
        }
        
        namespace {
            std::vector<Class>          classes_unsorted(Tag x)
            {
                static thread_local SQ s("SELECT class FROM CTags WHERE tag=?");
                return s.vec<Class>(x.id);
            }

            std::vector<Class>          classes_sorted(Tag x)
            {
                static thread_local SQ s("SELECT class FROM CTags INNER JOIN Classes ON CTags.class=Classes.id WHERE tag=? ORDER BY Classes.k");
                return s.vec<Class>(x.id);
            }
        }
        
        std::vector<Class>          classes(Tag x, Sorted sorted)
        {
            return sorted ? classes_sorted(x) : classes_unsorted(x);
        }

        std::set<Class>     classes_set(const string_set_t&sset, bool noisy)
        {
            std::set<Class>   ret;
            for(const std::string& s : sset){
                if(s.empty())
                    continue;
                    
                Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.insert(cls);
            }
            return ret;
        }
        
        std::set<Class>     classes_set(const string_view_set_t&sset, bool noisy)
        {
            std::set<Class>   ret;
            for(std::string_view s : sset){
                if(s.empty())
                    continue;
                    
                Class cls = class_(s);
                if(!cls){
                    if(noisy)
                        yWarning() << "Unable to find class: " << s;
                    continue;
                }
                
                ret.insert(cls);
            }
            return ret;
        }
        

        Class               db_class(std::string_view k, bool *wasCreated)
        {
            std::string     tfn = class_filename(k);
            return db_class(db_document(classes_folder(), tfn), wasCreated);
        }
        
        Class               db_class(Document doc, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(folder(doc) != classes_folder())
                return Class{};
            std::string k   = base_key(doc);
            if(k.empty())
                return Class();
        
            static thread_local SQ    i("INSERT OR FAIL INTO Classes (id,k) VALUES (?,?)");
            static thread_local SQ    i2("INSERT OR REPLACE INTO CLookup (class,k,priority) VALUES (?,?,1)");
            auto i_lk   = i.af();

            i.bind(1, doc.id);
            i.bind(2, k);
            if(i.exec()){
                if(wasCreated)
                    *wasCreated = true;
                i2.bind(1, doc.id);
                i2.bind(2, k);
                i2.exec();
                return Class{doc.id};
            } else if(exists_class(doc.id)){
                return Class{doc.id};
            } else {
                yError() << "Unable to get the class from the database: " << k;
                return Class();
            }
        }

        std::vector<Class>            db_classes(const string_set_t& all)
        {
            std::vector<Class>   ret;
            for(std::string_view  s: all)
                ret.push_back(db_class(s));
            return ret;
        }

        string_set_t         def_aliases(Class c)
        {
            static thread_local SQ s("SELECT alias FROM CAlias WHERE class=?");
            return s.sset(c.id);
        }
        
        namespace {
            std::vector<Class>           def_derived_sorted(Class c)
            {   
                static thread_local SQ s("SELECT class FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? AND hops=0 ORDER BY Classes.k");
                return s.vec<Class>();
            }

            std::vector<Class>           def_derived_unsorted(Class c)
            {   
                static thread_local SQ s("SELECT class FROM CDepends WHERE base=? AND hops=0 ");
                return s.vec<Class>();
            }
        }
        
        std::vector<Class>                def_derived(Class c, Sorted sorted)
        {
            return sorted ? def_derived_sorted(c) : def_derived_unsorted(c);
        }

        namespace {
            std::vector<Field>           def_fields_sorted(Class c)
            {
                static thread_local SQ s("SELECT field FROM CFields INNER JOIN Fields ON CFields.field=Fields.id WHERE CFields.class=? AND hops=0 ORDER BY Fields.cK");
                return s.vec<Field>(c.id);
            }

            std::vector<Field>           def_fields_unsorted(Class c)
            {
                static thread_local SQ s("SELECT field FROM CFields WHERE class=? AND hops=0");
                return s.vec<Field>(c.id);
            }
        }
        
        std::vector<Field>           def_fields(Class c, Sorted sorted)
        {
            return sorted ? def_fields_sorted(c) : def_fields_unsorted(c);
        }

        string_set_t            def_prefixes(Class c)
        {
            static thread_local SQ  s("SELECT prefix FROM CPrefix WHERE class=?");
            return s.sset(c.id);
        }

        namespace {
            std::vector<Class>        def_reverse_sorted(Class c)
            {
                static thread_local SQ s("SELECT reverse FROM CReverses INNER JOIN Classes ON CReverseDef.reverse=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_reverse_unsorted(Class c)
            {
                static thread_local SQ s("SELECT reverse FROM CReverses WHERE class=? AND hops=0");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>           def_reverse(Class c, Sorted sorted)
        {   
            return sorted ? def_reverse_sorted(c) : def_reverse_unsorted(c);
        }
        
        namespace {
            std::vector<Class>        def_source_sorted(Class c)
            {
                static thread_local SQ s("SELECT source FROM CSources INNER JOIN Classes ON CSourceDef.source=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_source_unsorted(Class c)
            {
                static thread_local SQ s("SELECT source FROM CSources WHERE class=? AND hops=0");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>           def_source(Class c, Sorted sorted)
        {
            return sorted ? def_source_sorted(c) : def_source_unsorted(c);
        }
        
        string_set_t                def_suffixes(Class c)
        {
            static thread_local SQ  s("SELECT suffix FROM CSuffix WHERE class=?");
            return s.sset(c.id);
        }

        namespace {
            std::vector<Class>        def_target_sorted(Class c)
            {
                static thread_local SQ s("SELECT target FROM CTargets INNER JOIN Classes ON CTargetDef.target=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_target_unsorted(Class c)
            {
                static thread_local SQ s("SELECT target FROM CTargets WHERE class=? AND hops=0");
                return s.vec<Class>(c.id);
            }
        }
        
        
        std::vector<Class>           def_target(Class c, Sorted sorted)
        {
            return sorted ? def_target_sorted(c) : def_target_unsorted(c);
        }

        namespace {
            std::vector<Class>        def_use_sorted(Class c)
            {
                static thread_local SQ s("SELECT base FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? AND hops=0 ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>        def_use_unsorted(Class c)
            {
                static thread_local SQ s("SELECT base FROM CDepends WHERE class=? AND hops=0");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>           def_use(Class c, Sorted sorted)
        {
            return sorted ? def_use_sorted(c) : def_use_unsorted(c);
        }

        // disabled until graphs are working
        //Graph               dep_graph(Class c)
        //{   
            //static thread_local SQ    s("SELECT deps FROM Classes WHERE id=?");
            //return s.as<Graph>(c.id);
        //}
        
        namespace {
            std::vector<Class>        dependents_sorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CDepends INNER JOIN Classes ON CDepends.class=Classes.id WHERE base=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }
            
            std::vector<Class>        dependents_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CDepends WHERE base=?");
                return s.vec<Class>(c.id);
            }
        }
        

        std::vector<Class>       dependents(Class c, Sorted sorted)
        {
            return sorted ? dependents_sorted(c) : dependents_unsorted(c);
        }
        
        std::vector<Class::Rank>    dependents_ranked(Class c)
        {
            static thread_local SQ  s("SELECT class, hops FROM CDepends WHERE base=?");
            auto s_af = s.af();
            s.bind(1, c.id);
            std::vector<Class::Rank>    ret;
            while(s.step() == SqlQuery::Row)
                ret.push_back({ Class(s.v_uint64(1)), s.v_int64(2)});
            return ret;
        }

        std::vector<Class::Rank>    dependents_ranked(Class c, int64_t depth)
        {
            static thread_local SQ  s("SELECT class, hops FROM CDepends WHERE base=? AND hops<=depth");
            auto s_af = s.af();
            s.bind(1, c.id);
            s.bind(2, depth);
            std::vector<Class::Rank>    ret;
            while(s.step() == SqlQuery::Row)
                ret.push_back({ Class(s.v_uint64(1)), s.v_int64(2)});
            return ret;
        }

        Folder              detail_folder(Class c)
        {
            return db_folder( classes_folder(), key(c));
        }

        Document            document(Class c)
        {
            return exists(c) ? Document{c.id}  : Document{};
        }
        

        bool                edge(Class c)
        {
            static thread_local SQ    s("SELECT edge FROM Classes WHERE id=?");
            return s.boolean(c.id);
        }
        
        namespace {
            std::vector<Class>    edges_in_sorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.class=Classes.id WHERE target=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    edges_in_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CTargets WHERE target=?");
                return s.vec<Class>(c.id);
            }
        }
        

        std::vector<Class>      edges_in(Class c, Sorted sorted)
        {
            return sorted ? edges_in_sorted(c) : edges_in_unsorted(c);
        }
        
        namespace {
            std::vector<Class>    edges_out_sorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }
            
            std::vector<Class>    edges_out_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT class FROM CSources WHERE source=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>      edges_out(Class c, Sorted sorted)
        {
            return sorted ? edges_out_sorted(c) : edges_out_unsorted(c);
        }

        void                erase(Class x)
        {
            static thread_local SQ stmts[] = {
                SQ("DELETE FROM CAlias WHERE class=?"),
                SQ("DELETE FROM CPrefix WHERE class=?"),
                SQ("DELETE FROM CSuffix WHERE class=?"),
                SQ("DELETE FROM CTags WHERE class=?"),
                SQ("DELETE FROM CLookup WHERE class=? AND priority=1"),
                SQ("DELETE FROM Classes WHERE id=?")
            };
            
            if(!x)
                return ;
                
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        bool                exists(Class c)
        {
            return exists_class(c.id);
        }
        
        

        bool                exists_class(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Classes WHERE id=?");
            return s.present(i);
        }
        
        Field               field(Class c, std::string_view k)
        {
            static thread_local SQ s("SELECT id FROM Fields WHERE class=? AND k=?");
            return s.as<Field>(c.id, k);
        }
        
        namespace {
            std::vector<Field>    fields_sorted(Class c)
            {
                static thread_local SQ    s("SELECT field FROM CFields INNER JOIN Fields ON CFields.field=Fields.id WHERE class=? ORDER BY Fields.K");
                return s.vec<Field>(c.id);
            }

            std::vector<Field>    fields_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT field FROM CFields WHERE class=?");
                return s.vec<Field>(c.id);
            }
        }
        

        std::vector<Field>       fields(Class c, Sorted sorted)
        {
            return sorted ? fields_sorted(c) : fields_unsorted(c);
        }
        
        size_t          fields_count(Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CFields WHERE class=?");
            return s.size(c.id);
        }
        
        
        Image               icon(Class c)
        {
            static thread_local SQ    s("SELECT icon FROM Classes WHERE id=? LIMIT 1");
            return s.as<Image>(c.id);
        }
        
        namespace {
            std::vector<Class>        inbound_sorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CTargets INNER JOIN Classes ON CTargets.cleass=Classes.id WHERE target=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }
            
            std::vector<Class>        inbound_unsorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CTargets WHERE target=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>           inbound(Class c, Sorted sorted)
        {
            return sorted ? inbound_sorted(c) : inbound_unsorted(c);
        }
        
        Class::Info         info(Class c, bool autoKey)
        {
            Class::Info    ret;
            static thread_local SQ    s("SELECT k, edge, name, plural, brief, icon, deps, category, binding FROM Classes WHERE id=?");
            auto s_af = s.af();
            s.bind(1, c.id);
            if(s.step() == SqlQuery::Row){
                ret.key         = s.v_string(1);
                ret.doc         = Document(c.id);
                ret.edge        = s.v_bool(2);
                ret.name        = s.v_string(3);
                ret.plural      = s.v_string(4);
                ret.brief       = s.v_string(5);
                ret.icon        = Image(s.v_uint64(6));
                //ret.deps        = Graph(s.v_uint64(7));
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                ret.category    = Category(s.v_uint64(8));
                ret.binding     = s.v_string(9);
            }
            return ret;
        }

        bool                is(Class d, Class b)
        {
            static thread_local SQ    s("SELECT 1 FROM CDepends WHERE class=? AND base=?");
            return s.present(d.id, b.id);
        }
        
        
        bool                is_all(Class d, std::initializer_list<Class> b)
        {
            for(Class c : b)
                if(!is(d, c))
                    return false;
            return true;
        }

        bool                is_any(Class d, std::initializer_list<Class> b)
        {
            for(Class c : b)
                if(is(d, c))
                    return true;
            return false;
        }

        
        std::string             key(Class c)
        {
            static thread_local SQ    s("SELECT k FROM Classes WHERE id=? LIMIT 1");
            return s.str(c.id);
        }
        
        
        std::string             label(Class c)
        {
            static thread_local SQ    s("SELECT ifnull(name,k) FROM Classes WHERE id=?");
            return s.str(c.id);
        }


        Class                make_class(std::string_view k, const Root* rt)
        {
            if(!rt)
                rt      = wksp::root_first(DataRole::Config);
            if(!rt){
                yError() << "No root specified to create the class in!";
                return Class{};
            }
            
            std::string cfn = class_filename(k);
            Document        doc = db_document(classes_folder(), cfn);
            bool            was = false;
            Class           c   = db_class(doc, &was);
            if(!was)
                return c;
                
            if(fragments_count(doc))
                return c;
            Class::SharedFile    td  = writable(c, rt, DONT_LOCK);
            td -> name      = k;
            td -> plural    = td->name + 's';
            td -> save();
            return c;
        }
        

        Class::SharedData            merged(Class c, cdb_options_t opts)
        {
            Class::SharedData        ret = std::make_shared<Class::Data>();;
            for(auto& i : reads(c, opts)){
                if(opts & IS_UPDATE)
                    update(i.first);
                ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        


        std::string             name(Class c)
        {
            static thread_local SQ    s("SELECT name FROM Classes WHERE id=?");
            return s.str(c.id);
        }

        
        NKI                 nki(Class c, bool autoKey)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Classes WHERE id=?");
            auto s_af = s.af();
            s.bind(1, c.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image{s.v_uint64(2)};
                ret.key     = s.v_string(3);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI();
        }
        
        
        namespace {
            std::vector<Class>    outbound_sorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CSources INNER JOIN Classes ON CSources.class=Classes.id WHERE source=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    outbound_unsorted(Class c)
            {
                static thread_local SQ s("SELECT class FROM CSources WHERE source=?");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>           outbound(Class c, Sorted sorted)
        {
            return sorted ? outbound_sorted(c) : outbound_unsorted(c);
        }

        
        std::string             plural(Class c)
        {
            static thread_local SQ    s("SELECT plural FROM Classes WHERE id=?");
            return s.str(c.id);
        }


        Class::SharedFile        read(Class c, const Root*rt, cdb_options_t opts)
        {
            return class_doc(fragment(document(c), rt), opts);
        }

        std::vector<ClassFragDoc>    reads(Class c, cdb_options_t opts)
        {
            std::vector<ClassFragDoc>  ret;
            for(Fragment f : fragments(document(c), DataRole::Config)){
                Class::SharedFile    p   = class_doc(f, opts);
                if(p)
                    ret.push_back(ClassFragDoc(f, p));
            }
            return ret;
        }

        std::vector<ClassFragDoc>  reads(Class c, class Root*rt, cdb_options_t opts)
        {
            std::vector<ClassFragDoc>  ret;
            for(Fragment f : fragments(document(c), rt)){
                Class::SharedFile    p   = class_doc(f, opts);
                if(p)
                    ret.push_back(ClassFragDoc(f, p));
            }
            return ret;
        }
        
        namespace {
            std::vector<Class>    reverses_sorted(Class c)
            {
                static thread_local SQ    s("SELECT reverse FROM CReverses INNER JOIN Classes ON CReverses.reverse=Classes.id WHERE class=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    reverses_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT reverse FROM CReverses WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        
        
        std::vector<Class>       reverses(Class c, Sorted sorted)
        {
            return sorted ? reverses_sorted(c) : reverses_unsorted(c);
        }
        
        namespace {
            std::vector<Class>    sources_sorted(Class c)
            {
                static thread_local SQ    s("SELECT source FROM CSources INNER JOIN Classes ON CSources.source=Classes.id WHERE class=? ORDER BY Classes.K");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    sources_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT source FROM CSources WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }

        std::vector<Class>  sources(Class c, Sorted sorted)
        {
            return sorted ? sources_sorted(c) : sources_unsorted(c);
        }

        size_t  sources_count(Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CSources WHERE class=?");
            return s.size(c.id);
        }


        std::set<Tag>               tag_set(Class c)
        {
            static thread_local SQ s("SELECT tag FROM CTags WHERE class=?");
            return s.set<Tag>(c.id);
        }

        bool  tagged(Class c, Tag t)
        {
            static thread_local SQ s("SELECT 1 FROM CTags WHERE class=? AND tag=?");
            return s.present(c.id, t.id);
        }

        namespace {
            std::vector<Tag>  tags_sorted(Class c)
            {
                static thread_local SQ s("SELECT tag FROM CTags INNER JOIN Tags ON CTags.tag=Tags.id WHERE class=? ORDER BY Tags.K");
                return s.vec<Tag>(c.id);
            }

            std::vector<Tag>  tags_unsorted(Class c)
            {
                static thread_local SQ s("SELECT tag FROM CTags WHERE class=?");
                return s.vec<Tag>(c.id);
            }
        }

        std::vector<Tag>  tags(Class c, Sorted sorted)
        {
            return sorted ? tags_sorted(c) : tags_unsorted(c);
        }
        
        
        size_t  tags_count(Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM CTags WHERE class=?");
            return s.size(c.id);
        }


        namespace {
            std::vector<Class>    targets_sorted(Class c)
            {
                static thread_local SQ    s("SELECT target FROM CTargets INNER JOIN Classes ON CTargets.target=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    targets_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT target FROM CTargets WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        
        std::vector<Class>       targets(Class c, Sorted sorted)
        {
            return sorted ? targets_sorted(c) : targets_unsorted(c);
        }

        size_t          targets_count(Class c)
        {
            static thread_local SQ    s("SELECT COUNT(1) FROM CTargets WHERE class=?");
            return s.size(c.id);
        }
        
        Class::SharedData   update(Class x, cdb_options_t opts)
        {
            if(!x)
                return Class::SharedData{};

            if(opts & U_ICON)
                update_icon(x);
            
            Class::SharedData data = merged(x, opts);
            if(!data)
                return Class::SharedData();
            
            std::string     k   = key(x);

            static thread_local SQ iAlias("INSERT INTO CAlias (class, alias) VALUES (?,?)");
            static thread_local SQ iPrefix("INSERT INTO CPrefix (class, prefix) VALUES (?,?)");
            static thread_local SQ iSuffix("INSERT INTO CSuffix (class, suffix) VALUES (?,?)");
            static thread_local SQ iLookup("INSERT INTO CLookup (class, k, priority) VALUES (?,?,0)");
            static thread_local SQ iTag("INSERT INTO CTags (class, tag) VALUES (?,?)");
            
            static thread_local SQ dAlias("DELETE FROM CAlias WHERE class=? AND alias=?");
            static thread_local SQ dPrefix("DELETE FROM CPrefix WHERE class=? AND prefix=?");
            static thread_local SQ dSuffix("DELETE FROM CSuffix WHERE class=? AND suffix=?");
            static thread_local SQ dLookup("DELETE FROM CLookup WHERE class=? AND k=?");
            static thread_local SQ dTag("DELETE FROM CTags WHERE class=? AND tag=?");

            static thread_local SQ uInfo("UPDATE Classes SET name=?, plural=?, brief=?, category=?, binding=? WHERE id=?");

            if(opts & U_INFO){
                Category cat = category(data->category);
                uInfo.bind(1, data->name);
                uInfo.bind(2, data->plural);
                uInfo.bind(3, data->brief);
                uInfo.bind(4, cat.id);
                uInfo.bind(5, data->binding);
                uInfo.bind(6, x.id);
                uInfo.exec();
                
                string_set_t    old_aliases         = def_aliases(x);
                string_set_t    old_prefixes        = def_prefixes(x);
                string_set_t    old_suffixes        = def_suffixes(x);
                string_set_t    old_alternatives    = alternative_keys(x);
                
                string_set_t    alts;
                for(auto& p : data->prefixes){
                    alts << p + k;
                    for(auto& a : data->aliases){
                        alts << p + a;
                        for(auto & s : data->suffixes)
                            alts << p + a + s;
                   }
                }
                
                for(auto& a : data->aliases){
                    alts << a;
                    for(auto& s : data->suffixes)
                        alts << a + s;
                }
                
                for(auto& s : data->suffixes)
                    alts << k + s;
                
                auto    ch_alias        = add_remove(old_aliases, data->aliases);
                auto    ch_prefix       = add_remove(old_prefixes, data->prefixes);
                auto    ch_suffix       = add_remove(old_suffixes, data->suffixes);
                auto    ch_alternative  = add_remove(old_alternatives, alts);
                
                iAlias.batch(x.id, ch_alias.added);
                dAlias.batch(x.id, ch_alias.removed);
                
                iPrefix.batch(x.id, ch_prefix.added);
                dPrefix.batch(x.id, ch_prefix.removed);

                iSuffix.batch(x.id, ch_suffix.added);
                dSuffix.batch(x.id, ch_suffix.removed);

                iLookup.batch(x.id, ch_alternative.added);
                dLookup.batch(x.id, ch_alternative.removed);
            }
            
            if(opts & U_TAGS){
                std::set<Tag>   old_tags = make_set(tags(x));
                std::set<Tag>   new_tags = tags_set(data->tags, true);
                
                auto ch_tag = add_remove(old_tags, new_tags);
                iTag.batch(x.id, ids_for(ch_tag.added));
                dTag.batch(x.id, ids_for(ch_tag.removed));
            }
            
            return data;
        }
        
        
        void                update_icon(Class x)
        {
            if(!x)
                return ;
                
            static thread_local SQ u1("UPDATE Classes SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            Document    doc     = document(x);
            Image       img     = best_image(doc);
            if(img != icon(x)){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }


        
        namespace {
            std::vector<Class>    uses_sorted(Class c)
            {
                static thread_local SQ    s("SELECT base FROM CDepends INNER JOIN Classes ON CDepends.base=Classes.id WHERE class=? ORDER BY Classes.k");
                return s.vec<Class>(c.id);
            }

            std::vector<Class>    uses_unsorted(Class c)
            {
                static thread_local SQ    s("SELECT base FROM CDepends WHERE class=?");
                return s.vec<Class>(c.id);
            }
        }
        

        std::vector<Class>   uses(Class c, Sorted sorted)
        {
            return sorted ? uses_sorted(c) : uses_unsorted(c);
        }
        
        size_t              uses_count(Class c)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM CDepends WHERE class=?");
            return s.size(c.id);
        }

        std::vector<Class::Rank>    uses_ranked(Class c)
        {
            static thread_local SQ  s("SELECT base, hops FROM CDepends WHERE class=?");
            auto s_af = s.af();
            s.bind(1, c.id);
            std::vector<Class::Rank>    ret;
            while(s.step() == SqlQuery::Row)
                ret.push_back({ Class(s.v_uint64(1)), s.v_int64(2)});
            return ret;
        }

        std::vector<Class::Rank>    uses_ranked(Class c, int64_t depth)
        {
            static thread_local SQ  s("SELECT base, hops FROM CDepends WHERE class=? AND hops<=depth");
            auto s_af = s.af();
            s.bind(1, c.id);
            s.bind(2, depth);
            std::vector<Class::Rank>    ret;
            while(s.step() == SqlQuery::Row)
                ret.push_back({ Class(s.v_uint64(1)), s.v_int64(2)});
            return ret;
        }
        

        Class::SharedFile        writable(Class c, const Root* rt, cdb_options_t opts)
        {
            Document    d   = document(c);
            if(!d)
                return Class::SharedFile();
            if(rt && !rt->is_writable(DataRole::Config))
                return Class::SharedFile();

            Fragment    f   = fragment(d);
            if(f)
                return class_doc(f, opts | ALLOW_EMPTY);
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);

            Class::SharedFile ptr  = std::make_shared<Class::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            ptr -> reload();
            return ptr;
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Class v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            if(v)
                h << "<a href=\"/class?key=" << cdb::key(v) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v);
            if(v)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Class> v)
        {
            if(v.data)
                h << "<a href=\"/dev/class?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Plural<Class>v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v.data), h.context().session.icon_size);
            if(v.data)
                h << "<a href=\"/atoms?class=" << cdb::key(v.data) << "\">";
            //  start the url (later)
            if(th)
                h << th << " ";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }

        void        dev_table(WebHtml&h, const std::vector<Class>& classes)
        {
            auto ta = h.table();
            h << "<tr><th>ID</th><th>Key</th><th>Name</th><th>Doc</th><th>Brief</th></tr>\n";
            for(Class c : classes){
                auto i = cdb::info(c);
                h << "<tr><td>" << dev_id(c) << "</td><td>" << i.key << "</td><td>" << i.name << "</td><td>" 
                    << dev(i.doc) << "</td><td>" << i.brief << "</td></tr>\n";
            }
        }
        
    }

////////////////////////////////////////////////////////////////////////////////
//  (ATOM) FIELD DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

    Field::Data&  Field::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(pkey, b.pkey, fOverride);
        set_if_empty(name, b.name, fOverride);
        set_if_empty(plural, b.plural, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(category, b.category, fOverride);
        aliases += b.aliases;
        //type.set_if(b.type, fOverride);
        types += b.types;
        //atom.set_if(b.atom, fOverride);
        atoms += b.atoms;
        tags += b.tags;
        set_if_empty(expected, b.expected, fOverride);
        if((fOverride?b.multiplicity:multiplicity) != Multiplicity())
            multiplicity    = b.multiplicity;
        if((fOverride?b.restriction:restriction) != Restriction())
            restriction     = b.restriction;
        if(fOverride?b.max_count:!max_count)
            max_count       = b.max_count;
        //triggers += b.triggers;
        //for(const auto &i : b.values){
            //auto j = values.find(i.first);
            //if(j != values.end())
                //j->second.merge(i.second, fOverride);
            //else
                //values[i.first] = i.second;
        //}
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////

    void Field::File::reset() 
    {
        (*(Data*) this) = Data{};
    }

    #if FIELD_XML_RESAVE
    bool    Field::File::read(const XmlDocument& doc, std::string_view fname) 
    {
        const XmlNode*  xn  = doc.first_node(szYQField);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        xn      = xn -> first_node(szField);
        if(!xn){
            yError() << "No field! In: " << fname << "'";
            return false;
        }

        pkey            = read_child(xn, szPKey, x_string);
        name            = read_child(xn, szName, x_string);
        plural          = read_child(xn, szPlural, x_string);
        brief           = read_child(xn, szBrief, x_string);
        notes           = read_child(xn, szNotes, x_string);
        category        = read_child(xn, szCategory, x_string);
        aliases         = read_child_string_set(xn, szAlias);
        classes         = read_child_string_set(xn, szClass);
        types           = read_child_string_set(xn, szType);
        atoms           = read_child_string_set(xn, szAtom);
        expected        = read_child(xn, szExpected, x_string);
        tags            = read_child_string_set(xn, szTag);
        multiplicity    = read_child(xn, szMultiple, x_enum<Multiplicity>);
        restriction     = read_child(xn, szRestrict, x_enum<Restriction>);
        max_count       = read_child(xn, szMaxCount, x_uint64).value;
        return true;
    }
    #endif

    bool    Field::File::read(KVTree&&attrs, std::string_view fname) 
    {
        name            = attrs.value(kv::key({ "%", "name" }));
        pkey            = attrs.value("pkey");
        plural          = attrs.value("plural");
        brief           = attrs.value("brief");
        notes           = attrs.value("notes");
        category        = attrs.value("category");
        aliases        += attrs.values_set("alias");
        tags           += attrs.values_set("tag");
        classes        += attrs.values_set("class");
        types          += attrs.values_set("type");
        atoms          += attrs.values_set("atom");
        expected        = attrs.value("expected");
        multiplicity    = Multiplicity(attrs.value("multiple"));
        restriction     = Restriction(attrs.value("restrict"));
        max_count       = to_uint(attrs.value("max")).value;
        return true;
    }
    
    bool    Field::File::write(KVTree&attrs) const 
    {
        if(!name.empty())
            attrs.set("%", name);
        if(!pkey.empty())
            attrs.set("pkey", pkey);
        if(!plural.empty())
            attrs.set("plural", plural);
        if(!brief.empty())
            attrs.set("brief", brief);
        if(!notes.empty())
            attrs.set("notes", notes);
        if(category.empty())
            attrs.set("category", category);
        if(!classes.empty())
            attrs.set("class", join(classes, ","));
        if(!aliases.empty())
            attrs.set("alias", join(aliases, ","));
        if(!tags.empty())
            attrs.set("tag", join(tags, ","));
        if(!types.empty())
            attrs.set("type", join(types, ","));
        if(!atoms.empty())
            attrs.set("atom", join(atoms, ","));
        
        if(multiplicity != Multiplicity())
            attrs.set("multiple", multiplicity.key());
        if(restriction != Restriction())
            attrs.set("restrict", restriction.key());
        if(max_count)
            attrs.set("max", to_string(max_count));
        return true;
    }


////////////////////////////////////////////////////////////////////////////////


    namespace arg {
        Field field(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Field{};
                
            Field t   = cdb::field( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_field(i))
                return Field{i};
            return Field{};
        }
        
        Field field(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return field_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return field_key(k);
            
            k       = ctx.find_query("field");
            if(!k.empty())
                return field(k);
            return Field{};
        }
        
        
        Field field(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }
        
        Field field(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field(arg_string);
        }

        Field field_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_field(i))
                return Field{i};
            return Field{};
        }

        Field field_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        Field field_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_id(arg_string);
        }
        
        Field field_key(std::string_view arg_string)
        {
            return cdb::field(trimmed(arg_string));
        }
        
        Field field_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
        
        Field field_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return field_key(arg_string);
        }
    }


////////////////////////////////////////////////////////////////////////////////


    bool Field::less_key(Field a, Field b)
    {
        return is_less_igCase(cdb::key(a), cdb::key(b));
    }

    bool Field::less_label(Field a, Field b)
    {
        return is_less_igCase(cdb::label(a), cdb::label(b));
    }

    bool Field::less_name(Field a, Field b)
    {
        return is_less_igCase(cdb::name(a), cdb::name(b));
    }


    namespace cdb {
        
        namespace {
            inline std::string field_filename(std::string_view k)
            {
                return make_filename(k, Field::szExtension);
            }
        }

        string_set_t            aliases(Field f)
        {
            static thread_local SQ s("SELECT alias FROM FAlias WHERE field=?");
            return s.sset(f.id);
        }
        

        namespace {
            std::vector<Field>    all_fields_sorted()
            {
                static thread_local SQ s("SELECT id FROM Fields ORDER BY k");
                return s.vec<Field>();
            }
            
            std::vector<Field>    all_fields_unsorted()
            {
                static thread_local SQ s("SELECT id FROM Fields");
                return s.vec<Field>();
            }
        }
        
        std::vector<Field>           all_fields(Sorted sorted)
        {
            return sorted ? all_fields_sorted() : all_fields_unsorted();
        }
        
        size_t              all_fields_count()
        {
            static thread_local SQ s("SELECT COUNT(1) FROM Fields");
            return s.size();
        }
        
        
        std::string             brief(Field f)
        {
            static thread_local SQ s("SELECT brief FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        Category  category(Field f)
        {
            static thread_local SQ s("SELECT category FROM Fields WHERE id=?");
            return s.as<Category>(f.id);
        }
        
        Class               class_(Field f)
        {
            static thread_local SQ s("SELECT class FROM Fields WHERE id=?");
            return s.as<Class>(f.id);
        }

        
        namespace {
            std::vector<Class>       classes_sorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields INNER JOIN Classes ON CFields.class=Classes.id WHERE field=? ORDER BY Classes.k");
                return s.vec<Class>(f.id);
            }

            std::vector<Class>       classes_unsorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields WHERE field=?");
                return s.vec<Class>(f.id);
            }
        }
        
        std::vector<Class>       classes(Field f, Sorted sorted)
        {
            return sorted ? classes_sorted(f) : classes_unsorted(f);
        }

        std::set<uint64_t>        data_types(Field f)
        {
            static thread_local SQ    s("SELECT type FROM FDataTypes WHERE field=?");
            return s.set<uint64_t>(f.id);
        }

        Field               db_field(Document doc, bool *wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            if(!doc)
                return Field{};
            
            if(exists_field(doc.id))
                return Field{doc.id};
            
            std::string k   = skeyc(doc);
            if(k.empty())
                return Field();

            std::string_view    ck  = k;
            Class               cls;
            size_t  n   = ck.find('.');
            if(n != std::string_view::npos){
                cls = class_(ck.substr(0,n));
                ck  = ck.substr(n+1);
            }

            static thread_local SQ i("INSERT INTO Fields (id, k, class, ck) VALUES (?,?,?,?)");
            auto i_af = i.af();
            i.bind(1, doc.id);
            i.bind(2, k);
            i.bind(3, cls.id);
            i.bind(4, ck);
            if(i.exec()){
                if(wasCreated)
                    *wasCreated = true;
                return Field{doc.id};
            } else if(exists_field(doc.id)){
                return Field{doc.id};
            } else {
                yError() << "Unable to get the field from the database: " << k;
                return Field{};
            }
        }
        
        
        namespace {
            std::vector<Class>           def_classes_sorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields INNER JOIN Classes ON FDefClass.class=Classes.id WHERE field=? ORDER BY Classes.K");
                return s.vec<Class>(f.id);
            }

            std::vector<Class>           def_classes_unsorted(Field f)
            {
                static thread_local SQ s("SELECT class FROM CFields WHERE field=?");
                return s.vec<Class>(f.id);
            }
        }
        
        std::vector<Class>           def_classes(Field f, Sorted sorted)
        {
            return sorted ? def_classes_sorted(f) : def_classes_unsorted(f);
        }

        Document                document(Field f)
        {
            return Document(f.id);
        }
        
        void                erase(Field x)
        {
            static thread_local SQ stmts[] = {
                SQ("DELETE FROM CFields WHERE field=?"),
                SQ("DELETE FROM FTags WHERE field=?"),
                SQ("DELETE FROM Fields WHERE id=?")
            };

            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        
        bool                exists(Field f)
        {
            return exists_field(f.id);
        }
        

        bool                exists_field(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Fields WHERE id=?");
            return s.present(i);
        }

        Field               field(uint64_t i)
        {
            return exists_field(i) ? Field{i} : Field{};
        }
        
        Field                   field(Document doc, bool calc)
        {
            if(!doc)
                return Field();
            if(exists_field(doc.id))
                return Field{doc.id};
            if(calc && (folder(doc) == fields_folder())){
                std::string k   = skeyc(doc);
                return field(k);
            }
            return Field();
        }

        Field                   field(std::string_view k)
        {
            static thread_local SQ s("SELECT 1 FROM Fields WHERE k=?");
            return s.as<Field>(k);
        }

        Field::SharedFile        field_doc(Fragment f, cdb_options_t opts)
        {
            if(!f)
                return Field::SharedFile();
                
            std::filesystem::path       fp  = path(f);
            Fragment::Lock  lk;
            if(!(opts & DONT_LOCK)){
                lk  = Fragment::Lock::read(f);
                if(!lk){
                    yWarning() << "Unable to get read lock on fragment: " << fp;
                    return Field::SharedFile();
                }
            }

            auto ch   = file_bytes(fp);
            lk.free();
            if(ch.empty()){
                if(opts & ALLOW_EMPTY)
                    return std::make_shared<Field::File>();
                return Field::SharedFile();
            }
            
                            
            Field::SharedFile  td = std::make_shared<Field::File>();
            if(!td->load(std::move(ch), fp)){
                yError() << "Unable to read " << fp;
                return Field::SharedFile();
            }
            td -> set_file(fp);
            return td;
        }

        Image               icon(Field f)
        {
            static thread_local SQ    s("SELECT icon FROM Fields WHERE id=?");
            return s.as<Image>(f.id);
        }
        
        
        Field::Info         info(Field f, bool autoKey)
        {
            Field::Info        ret;
            static thread_local SQ s("SELECT k, class, name, pkey, plural, brief, category FROM Fields WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                ret.key     = s.v_string(1);
                ret.class_  = Class(s.v_uint64(2));
                ret.name    = s.v_string(3);
                ret.pkey    = s.v_string(4);
                ret.plural  = s.v_string(5);
                ret.brief   = s.v_string(6);
                if(autoKey && ret.name.empty())
                    ret.name    = ret.key;
                ret.category = Category{ s.v_uint64(7)};
            }
            return ret;
        }
        
        std::string             key(Field f)
        {
            static thread_local SQ s("SELECT k FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             label(Field f)
        {
            static thread_local SQ    s("SELECT coalesce(name,ck,k) FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        Field::SharedData            merged(Field f, cdb_options_t opts)
        {
            Field::SharedData        ret = std::make_shared<Field::Data>();;
            for(auto& i : reads(f, opts)){
                if(opts & IS_UPDATE)
                    update(i.first);
                ret->merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
            }
            return ret;
        }
        


        std::string             name(Field f)
        {
            static thread_local SQ    s("SELECT name FROM Fields WHERE id=?");
            return s.str(f.id);
        }

        NKI                     nki(Field f, bool autoKeyToName)
        {
            static thread_local SQ    s("SELECT name,icon,k FROM Fields WHERE id=?");
            auto s_af = s.af();
            s.bind(1, f.id);
            if(s.step() == SqlQuery::Row){
                NKI  ret;
                ret.name    = s.v_string(1);
                ret.icon    = Image(s.v_uint64(2)) ;
                ret.key     = s.v_string(3);
                if(autoKeyToName && ret.name.empty())
                    ret.name    = ret.key;
                return ret;
            }
            return NKI{};
        }    
        
        
        Class               parent(Field f)
        {
            return class_(f);
        }

        
        std::string             pkey(Field f)
        {
            static thread_local SQ s("SELECT pkey FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        
        std::string             plural(Field f)
        {
            static thread_local SQ s("SELECT plural FROM Fields WHERE id=?");
            return s.str(f.id);
        }
        

        Field::SharedFile        read(Field f, const Root*rt, cdb_options_t opts)
        {
            return field_doc(fragment(document(f), rt), opts);
        }

        std::vector<FieldFragDoc>    reads(Field f, cdb_options_t opts)
        {
            std::vector<FieldFragDoc>  ret;
            for(Fragment ff : fragments(document(f), DataRole::Config)){
                Field::SharedFile    p   = field_doc(ff, opts);
                if(p)
                    ret.push_back(FieldFragDoc(ff, p));
            }
            return ret;
        }

        std::vector<FieldFragDoc>  reads(Field f, class Root*rt, cdb_options_t opts)
        {
            std::vector<FieldFragDoc>  ret;
            for(Fragment ff : fragments(document(f), rt)){
                Field::SharedFile    p   = field_doc(ff, opts);
                if(p)
                    ret.push_back(FieldFragDoc(ff, p));
            }
            return ret;
        }

        namespace {
            std::vector<Tag>  tags_sorted(Field f)
            {
                static thread_local SQ s("SELECT tag FROM FTags INNER JOIN Tags ON FTags.tag=Tags.id WHERE field=? ORDER BY Tags.K");
                return s.vec<Tag>(f.id);
            }
            
            std::vector<Tag>  tags_unsorted(Field f)
            {
                static thread_local SQ s("SELECT tag FROM FTags WHERE field=?");
                return s.vec<Tag>(f.id);
            }
        }
        
        std::vector<Tag>  tags(Field f, Sorted sorted)
        {
            return sorted ? tags_sorted(f) : tags_unsorted(f);
        }

        
        size_t  tags_count(Field f)
        {
            static thread_local SQ s("SELECT COUNT(1) FROM FTags WHERE field=?");
            return s.size(f.id);
        }

        std::set<Tag>            tags_set(Field f)
        {
                static thread_local SQ s("SELECT tag FROM FTags WHERE field=?");
                return s.set<Tag>(f.id);
        }

        Field::SharedData       update(Field x, cdb_options_t opts)
        {
            if(!x)
                return Field::SharedData();
                
            if(opts & U_ICON)
                update_icon(x);

            Field::SharedData data = merged(x, IS_UPDATE | opts);
            if(!data)
                return Field::SharedData();
            
            static thread_local SQ iAlias("INSERT INTO FAlias (field, alias) VALUES (?,?)");
            static thread_local SQ dAlias("DELETE FROM FAlias WHERE field=? AND alias=?");
            static thread_local SQ iTypes("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
            static thread_local SQ dTypes("DELETE FROM FDataTypes WHERE field=? AND type=?");
            static thread_local SQ iTags("INSERT INTO FTags (field, tag) VALUES (?, ?)");
            static thread_local SQ dTags("DELETE FROM FTags WHERE field=? AND tag=?");
            static thread_local SQ uInfo("UPDATE Fields SET name=?,brief=?,multi=?,restrict=?,category=?,pkey=?,expected=?,maxcnt=?,plural=? WHERE id=?");
            
            if(opts & U_INFO){
                Category cat = category(data->category);

                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, data->multiplicity.value());
                uInfo.bind(4, data->restriction.value());
                uInfo.bind(5, cat.id);
                uInfo.bind(6, data->pkey);
                uInfo.bind(7, data->expected);
                uInfo.bind(8, data->max_count);
                uInfo.bind(9, data->plural);
                uInfo.bind(10, x.id);
                uInfo.exec();
                
                
                string_set_t    old_aliases = aliases(x);
                std::set<uint64_t> old_types = data_types(x);
                std::set<uint64_t> new_types = ids_for(TypeInfo::find_all(data->types, true));
                
                auto ch_aliases = add_remove(old_aliases, data->aliases);
                auto ch_types   = add_remove(old_types, new_types);
                
                iAlias.batch(x.id, ch_aliases.added);
                dAlias.batch(x.id, ch_aliases.removed);
                iTypes.batch(x.id, ch_types.added);
                dTypes.batch(x.id, ch_types.removed);
            }
            
            if(opts & U_TAGS){
                std::set<Tag>       old_tags    = tags_set(x);
                std::set<Tag>       new_tags    = tags_set(data->tags, true);
                
                auto ch_tags    = add_remove(old_tags, new_tags);
                iTags.batch(x.id, ids_for(ch_tags.added));
                dTags.batch(x.id, ids_for(ch_tags.removed));
            }
            
            //  AND MORE TO GO.....
            
            return data;
        }
        
        void                update_icon(Field x)
        {
            if(!x)
                return ;
                
            Document    doc     = document(x);
            Image       img     = best_image(doc);
            
            static thread_local SQ u1("UPDATE Fields SET icon=? WHERE id=?");
            static thread_local SQ u2("UPDATE Documents SET icon=? WHERE id=?");

            if(icon(doc) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        
        Field::SharedFile        writable(Field f, const Root* rt, cdb_options_t opts)
        {
            Document    d   = document(f);
            if(!d)
                return Field::SharedFile();
            if(rt && !rt->is_writable(DataRole::Config))
                return Field::SharedFile();

            Fragment    ff   = fragment(d);
            if(ff)
                return field_doc(ff, opts | ALLOW_EMPTY);
                
            Folder      fo  = folder(d);
            if((fo != cdb::top_folder()) && !exists(fo, rt))
                make_directory(fo, rt);
            Field::SharedFile ptr  = std::make_shared<Field::File>();
            ptr -> set_file( rt -> resolve(key(d)));
            return ptr;
        }
    }

////////////////////////////////////////////////////////////////////////////////

    namespace html {
        WebHtml&    operator<<(WebHtml&h, Field v)
        {
            Thumbnail th = cdb::thumbnail(cdb::icon(v), h.context().session.icon_size);
            
            //  start the url (later)
            if(th)
                h << th << " ";
            
            h << cdb::label(v);
            //  end the url (later)
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, Dev<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << cdb::label(v.data);
            if(v.data)
                h << "</a>";
            return h;
        }
        
        WebHtml&    operator<<(WebHtml&h, DevID<Field> v)
        {
            if(v.data)
                h << "<a href=\"/dev/field?id=" << v.data.id << "\">";
            h << v.data.id;
            if(v.data)
                h << "</a>";
            return h;
        }
        
        void        dev_table(WebHtml&h, const std::vector<Field>& fields)
        {
            auto t = h.table();
            h << "<tr><th>ID</th><th>Class</th><th>Key</th><th>Name</th><th>Brief</th>\n";
            for(Field f : fields){
                auto i = cdb::info(f);
                h << "<tr><td>" << dev_id(f) << "</td><td>" << dev(i.class_) << "</td><td>" 
                    << i.key <<"</td><td>" << i.name << "</td><td>"
                    << i.brief <<"</td></tr>\n";
            }
        }
    }

////////////////////////////////////////////////////////////////////////////////
//  (ATOM) PROPERTIES
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  (ATOM FIELD) VALUES
////////////////////////////////////////////////////////////////////////////////


    Value::Data&   Value::Data::merge(const Data&b, bool fOverride)
    {
        set_if_empty(notes, b.notes, fOverride);
        set_if_empty(brief, b.brief, fOverride);
        set_if_empty(data, b.data, fOverride);
        return *this;
    }

    
    Value::Data x_value(const XmlNode*xn)
    {
        Value::Data ret;
        read_xn(ret, xn);
        return ret;
    }
    
    bool    read_xn(Value::Data&ret, const XmlNode*xn)
    {
        ret.notes   = read_child(xn, szNotes, x_string);
        ret.brief   = read_child(xn, szBrief, x_string);
        ret.data    = read_child(xn, szData, x_string);
        return true;
    }
    
    void    write_xn(XmlNode*xn, const Value::Data&v)
    {
        write_child(xn, szData, v.data);
        if(!v.brief.empty())
            write_child(xn, szBrief, v.brief);
        if(!v.notes.empty())
            write_child(xn, szNotes, v.notes);
    }

    Value::ListData&   Value::ListData::merge(const ListData&b, bool fOverride)
    {
        values += b.values;
        return *this;
    }

////////////////////////////////////////////////////////////////////////////////


    bool    Value::ListFile::read(const XmlDocument&doc, std::string_view fname)
    {
        const XmlNode*  xn  = doc.first_node(szYQValues);
        if(!xn){
            yError() << "No appropriate root element!  In: '" << fname << "'";
            return false;
        }
        
        values  = read_children(xn, szValue, x_value);
        return true;
    }
    
    bool    Value::ListFile::write(XmlDocument&doc) const
    {
        XmlNode*  xroot  = doc.allocate_node(rapidxml::node_element, szYQUser);
        doc.append_node(xroot);
        
        write_children(xroot, szValue, values);
        return true;
    }

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
    namespace cdb {
        std::string     brief(Value v)
        {
            static thread_local SQ s("SELECT brief FROM Vals WHERE id=?");
            return s.str(v.id);
        }
        
        std::string     data(Value v)
        {
            static thread_local SQ s("SELECT data FROM Vals WHERE id=?");
            return s.str(v.id);
        }
    
        Value           db_value(Field field, std::string_view data, bool* wasCreated)
        {
            if(wasCreated)
                *wasCreated = false;
            
            if(data.empty() || !field)
                return Value();
            
            static thread_local SQ i("INSERT INTO Vals OR FAIL (field, data) VALUES (?,?)");
            static thread_local SQ s("SELECT id FROM Vals WHERE field=? AND data=?");
            
            auto iaf = i.af();
            auto saf = s.af();

            i.bind(1, field.id);
            i.bind(2, data);
            if(i.step(false) == SqlQuery::Done){
                if(wasCreated)
                    *wasCreated = true;
                return Value{(uint64_t) i.last_id()};
            } else {
                s.bind(1, field.id);
                s.bind(2, data);
                if(s.step() == SqlQuery::Row)
                    return Value{s.v_uint64(1)};
                cdbError << "Unable to get value ID";
                return Value();
            }
        }
        
        
        bool            exists(Value v)
        {
            return exists_value(v.id);
        }
        
        bool            exists_value(uint64_t i)
        {
            static thread_local SQ s("SELECT 1 FROM Vals WHERE id=? LIMIT 1");
            return s.present(i);
        }

        Field           field(Value v)
        {
            static thread_local SQ s("SELECT field FROM Vals WHERE id=?");
            return s.as<Field>(v.id);
        }
        
        Value::Info     info(Value v)
        {
            Value::Info ret;

            static thread_local SQ s("SELECT field, brief, data FROM Vals WHERE id=?");
            auto af = s.af();
            s.bind(1, v.id);
            if(s.step() == SqlQuery::Row){
                ret.field   = Field{ s.v_uint64(1) };
                ret.brief   = s.v_string(2);
                ret.data    = s.v_string(3);
            }
            return ret;
        }
        
        Value           value(uint64_t i)
        {
            return exists_value(i) ? Value{i} : Value{};
        }
        
        namespace {
            Vector<Value>   values_sorted(Field f)
            {
                static thread_local SQ s("SELECT values FROM Vals WHERE field=? ORDER BY data");
                return s.vec<Value>(f.id);
            }

            Vector<Value>   values_unsorted(Field f)
            {
                static thread_local SQ s("SELECT values FROM Vals WHERE field=?");
                return s.vec<Value>(f.id);
            }
        }

        Vector<Value>   values(Field f, Sorted sorted)
        {
            return sorted ? values_sorted(f) : values_unsorted(f);
        }
        
        Set<Value>      values_set(Field f)
        {
            static thread_local SQ s("SELECT values FROM Vals WHERE field=?");
            return s.set<Value>(f.id);
        }
    }

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
}
