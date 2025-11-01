////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "StyleCDB.hpp"
#include <yq/container/ByteArray.hpp>
#include <yq/file/FileUtils.hpp>

#include <yq/mithril/bit/NKI.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/root/RootDir.hpp>
#include <yq/mithril/style/StyleFile.hpp>
#include <yq/mithril/style/StyleInfo.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

namespace yq::mithril::cdb {
    namespace {
        inline std::string style_filename(std::string_view k)
        {
            return make_filename(k, Style::EXTENSION);
        }
    }
    
    StyleVector        all_styles(Sorted sorted)
    {
        static thread_local CacheQuery qs("SELECT id FROM " TBL_STYLES " ORDER BY k");
        static thread_local CacheQuery qu("SELECT id FROM " TBL_STYLES "");
        CacheQuery& s = sorted ? qs : qu;
        return s.vec<Style>();
    }
    
    size_t              all_styles_count()
    {
        static thread_local CacheQuery s("SELECT COUNT(1) FROM " TBL_STYLES "");
        return s.size();
    }
    
    
    Style                db_style(Document doc, bool*wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
            
        if(!doc)
            return Style();

        if(exists_style(doc.id))
            return Style{ doc.id };
        
        std::string     tk  = key(doc);
        std::string     k(strip_extension(tk));
        if(k.empty()){
            yError() << "Cannot create to blank key!";
            return Style();
        }
        
        static thread_local CacheQuery i("INSERT INTO " TBL_STYLES " (id,k) VALUES (?,?)");
        auto i_af   = i.af();
        i.bind(1,doc.id);
        i.bind(2, k);
        if(i.exec()){
            if(wasCreated)
                *wasCreated = true;
            return Style(doc.id);
        } else if(exists_style(doc.id)){
            return Style(doc.id);
        } else {
            yError() << "Unable to create or find the style: " << k;
            return Style();
        }
    }

#if 0
    Folder              detail_folder(Style l)
    {
        Folder  f   = folder(Document{l.id});
        std::string sk  = skeyb(Document{l.id});
        return db_folder(f, sk+".d");
    }
#endif
    
    Document            document(Style l)
    {
        return exists(l) ?  Document{l.id} : Document{};
    }
    

    bool                exists(Style l)
    {
        return exists_style(l.id);
    }
    

    bool                exists_style(uint64_t i)
    {
        static thread_local CacheQuery s("SELECT 1 FROM " TBL_STYLES " WHERE id=? LIMIT 1");
        return s.present(i);
    }

    
    Image               icon(Style l)
    {
        static thread_local CacheQuery    s("SELECT icon FROM " TBL_STYLES " WHERE id=? LIMIT 1");
        return s.as<Image>(l.id);
    }
    

    Style::Info          info(Style l, bool autoKey)
    {
        Style::Info    ret;
        static thread_local CacheQuery s("SELECT k, name, icon, brief FROM " TBL_STYLES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            ret.doc     = Document(l.id);
            ret.key     = s.v_string(1);
            ret.name    = s.v_string(2);
            if(autoKey && ret.name.empty())
                ret.name   = ret.key;
            ret.icon    = { s.v_uint64(3) };
            ret.brief   = s.v_string(4);
        }
        return ret;
    }

    //bool                    is_style(Atom);
    
    
    std::string             key(Style l)
    {
        static thread_local CacheQuery s("SELECT k FROM " TBL_STYLES " WHERE id=?");
        return s.str(l.id);
    }

    std::string             label(Style l)
    {
        static thread_local CacheQuery    s("SELECT ifnull(name,k) FROM " TBL_STYLES " WHERE id=?");
        return s.str(l.id);
    }
    

    Style                     make_style(std::string_view k, const RootDir* rt, cdb_options_t opts, bool* wasCreated)
    {
        if(wasCreated)
            *wasCreated = false;
        if(k.empty()){
            yError() << "Cannot create a BLANK style.";
            return Style();
        }
    
        if(!rt)
            rt  = wksp::root_first(DataRole::Config);
        if(!rt){
            yError() << "No root_dir specified to create the style in!";
            return Style{};
        }
        
        std::string     tfn = style_filename(k);
        Document    doc = db_document(styles_folder(), tfn);
        bool            was = false;
        Style         t   = db_style(doc, &was);
        if(!t){
            yWarning() << "Unable to create/find style: " << k;
            return t;
        }
        if(wasCreated)
            *wasCreated = was;
        if(!was)
            return t;
        if(fragments_count(doc))
            return t;
            
        Id::Lock   lk;
        if(!(opts & DONT_LOCK))
            lk  = Id(t).lock(true);
        
            // prelude, we're first....
        Style::SharedFile td  = write(t, rt, opts);
        td -> name  = k;
        td -> save();
        return t;
    }

    
    Style::SharedData         merged(Style l, cdb_options_t opts)
    {
        if(!l)
            return Style::SharedData();
        
        Id::Lock   lk;
        if(!(opts & DONT_LOCK)){
            lk   = Id(l).lock(false);
            if(!lk){
                yWarning() << "Unable to acquire read lock on style: " << key(l);
                return Style::SharedData();
            }
        }

        Style::SharedData     ret = std::make_shared<Style::Data>();
        for(auto& i : reads(l, opts)){
            if(opts & IS_UPDATE)
                cdb::update(i.first);
            ret -> merge(*(i.second), static_cast<bool>(opts&OVERRIDE));
        }
        return ret;
    }
    
    std::string             name(Style l)
    {
        static thread_local CacheQuery    s("SELECT name FROM " TBL_STYLES " WHERE id=?");
        return s.str(l);
    }

    NKI                 nki(Style l, bool autoKey)
    {
        static thread_local CacheQuery    s("SELECT name,icon,k FROM " TBL_STYLES " WHERE id=?");
        auto s_af = s.af();
        s.bind(1, l.id);
        if(s.step() == SQResult::Row){
            NKI  ret;
            ret.name    = s.v_string(1);
            ret.icon    = Image(s.v_uint64(2)) ;
            ret.key     = s.v_string(3);
            if(autoKey && ret.name.empty())
                ret.name    = ret.key;
            return ret;
        }
        return NKI{};
    }

    Style::SharedFile         read(Style l, const RootDir* rt, cdb_options_t opts)
    {
        return style_doc(fragment(document(l), rt), opts);
    }

    std::vector<StyleFragDoc>     reads(Style l, cdb_options_t opts)
    {
        std::vector<StyleFragDoc>  ret;
        for(Fragment f : fragments(document(l), DataRole::DB)){
            Style::SharedFile p   = style_doc(f, opts);
            if(p)
                ret.push_back(StyleFragDoc(f,p));
        }
        return ret;
    }

    std::vector<StyleFragDoc>   reads(Style l, class RootDir*rt, cdb_options_t opts)
    {
        std::vector<StyleFragDoc>  ret;
        for(Fragment f : fragments(document(l), rt)){
            Style::SharedFile p   = style_doc(f, opts);
            if(p)
                ret.push_back(StyleFragDoc(f,p));
        }
        return ret;
    }



    
    Style                style(uint64_t i)
    {
        return exists_style(i) ? Style{i} : Style();
    }

    Style                style(std::string_view  k)
    {
        static thread_local CacheQuery s("SELECT id FROM " TBL_STYLES " WHERE k=?");
        return s.as<Style>(k);
    }

    Style                style(Document d, bool calc)
    {
        if(!d)
            return Style();
        if(exists_style(d.id))
            return Style(d.id);
        if(calc){
            std::string k   = key(folder(d)) + "/" + skeyb(d);
            return style(k);
        }
        return Style();
    }
    
    #if 0
    Style                style(Folder f)
    {
        return style(key(f));
    }
    #endif
    
    Style::SharedFile         style_doc(Fragment f, cdb_options_t opts)
    {
        if(!f)
            return Style::SharedFile();

        std::filesystem::path       fp  = path(f);

        const RootDir* rt  = root_dir(f);
        if(!rt)
            yWarning() << "No root_dir for: " << fp;

        Id::Lock  lk;
        if(!(opts & DONT_LOCK)){
            lk  = Id(f).lock(false);
            if(!lk){
                yWarning() << "Unable to get read lock on fragment: " << fp;
                return Style::SharedFile();
            }
        }

        auto    ch   = file_bytes(fp);
        lk.free();
        if(ch.empty()){
            if(opts & ALLOW_EMPTY)
                return std::make_shared<Style::File>() ;
            return Style::SharedFile();
        }
        
        Style::SharedFile     td  = std::make_shared<Style::File>();
        if(td->load(std::move(ch), fp) != std::error_code()){
            yError() << "Unable to read " << fp;
            return Style::SharedFile();
        }
        td -> set_file(fp);
        //for(auto& ctx : td -> context)
            //ctx.root_dir    = rt;
        return td;
    }


    Style::SharedFile         write(Style l, const RootDir* rt, cdb_options_t opts)
    {
        if(!l)
            return Style::SharedFile();
        Document    d   = document(l);
        if(!d){
            yWarning() << "write(Style '" << key(l) << "'): Has no document!";
            return Style::SharedFile();
        }
        if(rt && !rt->is_writable(DataRole::DB)){
            yWarning() << "write(Style '" << key(l) << "'): RootDir " << rt->key << " cannot be written to!";
            return Style::SharedFile();
        }

        Fragment    f   = rt ? fragment(d, rt) : writable(d, DataRole::DB);
        if(f)
            return style_doc(f, opts | ALLOW_EMPTY);
            
        Folder      fo  = folder(d);
        if((fo != cdb::top_folder()) && !exists(fo, rt))
            make_directory(fo, rt);
        Style::SharedFile ptr  = std::make_shared<Style::File>();
        ptr -> set_file( rt -> resolve(key(d)));
        ptr -> reload();
        return ptr;
    }

}
