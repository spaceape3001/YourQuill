////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/mithril/document/DocumentCDB.hpp>
#include <yq/mithril/document/DocumentUpdate.hpp>
#include <yq/mithril/file/FileSpec.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>
#include <yq/mithril/leaf/LeafUpdate.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>

namespace yq::mithril::update {
    Atom ULeaf::i_atom(Leaf x)
    {
        Document doc    = cdb::document(x);
        Atom    at      = cdb::db_atom(doc);
        
        static thread_local CacheQuery sql("UPDATE Leafs SET atom=? WHERE id=?");
        sql.exec(at.id, x.id);
        return at;
    }

    void             ULeaf::icons(Fragment frag, [[maybe_unused]] Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "ULeaf::icons('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif
    
        Leaf    x   = cdb::leaf(cdb::document(frag), true);
        if(!x)
            return ;
        u_icon(x);
    }
    
    const FileSpec&  ULeaf::lookup()
    {
        static const FileSpec  s_ret(CACHE, "*.y");
        return s_ret;
    }

    void             ULeaf::notify(Fragment frag,Change chg)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "ULeaf::notify('" << cdb::path(frag) << "', " << chg.key() << ")";
        #endif
        
        Document    doc = cdb::document(frag);
        if(cdb::hidden(cdb::folder(doc)))
            return;

        if(chg == Change::Removed){
            if(cdb::fragments_count(doc) <= 1){
                Leaf        x   = cdb::leaf(doc);
                if(x){
                    x_erase(x);
                }
                return ;
            }
        }
        
        bool    cr  = false;
        Leaf    x   = cdb::db_leaf(doc, &cr);
        auto    def = reload(x);
        if(!def)
            return ;
        Atom    at;
        
        if(cr){
            at      = i_atom(x);
            u_icon(x);
        } else {
            at      = cdb::atom(x);
        }
        
        u_info(x, *def);
        u_tags(x, *def);
        
        if(cr){
            UAtom::i_atom(at, def->attrs);
            
            //  binding.... 
            
            UAtom::i_notify(at, true);
        } else {
            UAtom::u_atom(at, def->attrs);
        }
    }
    
    Leaf::SharedData ULeaf::reload(Leaf x)
    {
        return cdb::merged(x, { cdb::IS_UPDATE | cdb::IGNORE_CONTEXT });
    }

    void             ULeaf::s3(Document doc)
    {
        #ifdef YQ_UPDATE_ECHO_STEPS
            YQ_UPDATE_ECHO_STEPS << "ULeaf::s3('" << cdb::key(doc) << "')";
        #endif

        if(cdb::hidden(cdb::folder(doc)))
            return;
        
        Leaf x      = cdb::db_leaf(doc);
        Atom at     = i_atom(x);
        auto def    = reload(x);
        if(!def)
            return ;
        
        u_icon(x);
        u_info(x, *def);
        u_tags(x, *def);
        UAtom::i_atom(at, def->attrs);
    }

    void         ULeaf::u_icon(Leaf x, Image img)
    {
        static thread_local CacheQuery sql("UPDATE Leafs SET icon=? WHERE id=?");
        sql.exec(img.id, x.id);
    }
    
    void         ULeaf::u_icon(Leaf x)
    {
        Document    doc     = cdb::document(x);
        if(!doc)
            return ;

        Atom        atom    = cdb::atom(x);
        Image       icon    = cdb::icon(x);
        Image       img     = cdb::best_image(doc);
        
        if(img != icon){
            ULeaf::u_icon(x, img);
            UDocument::u_icon(doc, img);
            UAtom::u_icon(atom, img);
        }
    }
    
    void ULeaf::u_info(Leaf x, const Leaf::Data &def)
    {
        if(!x)
            return;

        std::string     key;

        static thread_local CacheQuery uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");
        std::string_view title   = def.title();
        std::string_view abbr    = def.abbr();
        std::string_view brief   = def.brief();
        if(title.empty())
            title                = def.attrs.value(kv::key({"nick", "name"}));
        
        if(title.empty()){
            key     = cdb::key(x);
            uInfo.bind(1, key);     // fall back (for now) 
                                    // TODO ... make this smarter (later)
        } else 
            uInfo.bind(1, title);

        uInfo.bind(2, abbr);
        uInfo.bind(3, brief);
        uInfo.bind(4, x.id);
        uInfo.exec();
    }
    
    void ULeaf::u_tags(Leaf x, const Leaf::Data& def)
    {
        if(!x)
            return ;
    
        TagSet  old_tags    = cdb::tags_set(x);
        TagSet  new_tags    = cdb::find_tags_set(def.tags(), true);
        auto    chg         = add_remove(old_tags, new_tags);
        
        static thread_local CacheQuery dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
        static thread_local CacheQuery iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
        
        for(Tag t : chg.added)
            iTag.exec(x.id, t.id);
        for(Tag t : chg.removed)
            dTag.exec(x.id, t.id);
    }

    void    ULeaf::x_erase(Leaf x)
    {
        static thread_local CacheQuery  stmts[] = {
            CacheQuery( "DELETE FROM LTags WHERE leaf=?" ),
            CacheQuery( "DELETE FROM Leafs WHERE id=?" )
        };
        for(auto& sq : stmts)
            sq.exec(x.id);
    }
}

