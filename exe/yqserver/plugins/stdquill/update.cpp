////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/BasicApp.hpp>
#include <basic/CollectionUtils.hpp>
#include <basic/DelayInit.hpp>
#include <io/DirUtils.hpp>
#include <io/FileUtils.hpp>
#include <basic/Logging.hpp>
#include <io/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <basic/Vector.hpp>

#include <meta/TypeInfo.hpp>

#include <io/stream/Bytes.hpp>
#include <io/stream/Text.hpp>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/atom/Property.hpp>
#include <mithril/value/ValueCDB.hpp>
#include <mithril/attribute/AttributeCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/root/Root.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/io/Strings.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/leaf/LeafData.hpp>
#include <mithril/notify/AtomNotifier.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Stage2.hpp>
#include <mithril/notify/Stage3.hpp>
#include <mithril/notify/Stage4.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/user/User.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

#include <math/shape/Size2.hxx>

#include "common.hpp"

#include <mithril/atom.hpp>
#include <mithril/attribute.hpp>

#include <mithril/class.hpp>
#include <mithril/directory.hpp>
#include <mithril/document.hpp>
#include <mithril/field.hpp>
#include <mithril/folder.hpp>
#include <mithril/fragment.hpp>

#include <mithril/leaf.hpp>
#include <mithril/property.hpp>
#include <mithril/root.hpp>

//#include <mithril/user.hpp>
#include <mithril/value.hpp>

#include <MagickCore/magick-baseconfig.h>
//  Know this is obsolete, however, there's no good way to automate it otherwise w/o tolerating the warning
#define MAGICKCORE_QUANTUM_DEPTH MAGICKCORE_QUANTUM_DEPTH_OBSOLETE_IN_H
#define MAGICKCORE_HDRI_ENABLE MAGICKCORE_HDRI_ENABLE_OBSOLETE_IN_H
#include <Magick++.h>

alignas(64) Guarded<std::string>        gTextColor;
alignas(64) Guarded<std::string>        gBkColor;
alignas(64) std::atomic<bool>           gHasBackground{false};
alignas(64) Guarded<SharedByteArray>    gCss;
alignas(64) std::filesystem::path       gSharedCssFile;
alignas(64) std::filesystem::path       gSharedPageFile;

namespace {
    static constexpr const char*    kPage               = ".page";
    static const std::string_view       kStdCSS         = "www/css";
    static const std::string_view       kStdPage        = "www/page.ht";
}


struct Sigma {
    unsigned short  count   = USHRT_MAX;
    
    Sigma& operator=(unsigned short v)
    {
        count   = v;
        return *this;
    }
    
    Sigma&  operator <<= (unsigned short v)
    {
        count = std::min(v, count);
        return *this;
    }
    
    constexpr auto operator<=>(const Sigma&) const noexcept = default;
    
};

using namespace yq;
using namespace yq::mithril;
using namespace yq::mithril::cdb;


namespace {
    void    page__(cdb_options_t opts=0)
    {
        Ref<Template>        wTemplate;
        std::string     r;
        
        for(Fragment f : fragments(document(kPage))){
            r       = frag_string(f, opts);
            if(r.empty())
                continue;
        }
        
        if(r.empty())
            r   = file_string(gSharedPageFile);
        
        wTemplate   = new Template(std::move(r));
        web::set_template(wTemplate);
    }       

    void    page_stage4()
    {
        page__(DONT_LOCK);
    }
    
    void    page_update()
    {
        page__();
    }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  FORWARD DECLARATIONS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using ClsHopsMap   = std::map<Class,uint64_t>;

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ATOM
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        /*

            On creation
            
                -> classes insert (immedate)
                -> attributes insert
                -> property conversion (w/o edge detection)
                
            
                -> class insert notifications
                
                -> field insert notifications
                
                
                Note, we're going to defer inferrence & change considerations for later... :) 

        */



        //! Used for inserting classes on a (known to be) new atom
        //! \param[in]  Atom to associate these classes with
        //! \param[in]  
        void    ati_classes(Atom at, const ClassSet& classes)
        {
            static thread_local CacheQuery  iClass("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?)");
            for(auto& itr : cdb::base_classes_ranked_merged_map(classes, true)){
                iClass.bind(1, at.id);
                iClass.bind(2, itr.first.id);
                iClass.bind(3, itr.second.cnt);
                iClass.exec();
            }
        }

        //! Used for inserting classes on a (known to be) new atom
        void    ati_classes(Atom at, KVTree& attrs, bool autoDeleteKeys=true)
        {
            auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
            if(autoDeleteKeys)
                attrs.erase_all(svL_aClass);
            return ati_classes(at, clsset);
        }

        SetChanges<Class>   atu_classes(Atom at, const ClassSet& classes)
        {
            SetChanges<Class>   ret;
            ClassCountMap   omap    = cdb::classes_and_hops_map(at);
            ClassCountMap   nmap    = cdb::base_classes_ranked_merged_map(classes, true);
            map_difference_exec(omap, nmap, 
                [&](const ClassCountMap::value_type&pp){    // REMOVED
                    ret.removed.insert(pp.first);

                    static thread_local CacheQuery  d("DELETE FROM AClasses WHERE atom=? AND class=?");
                    auto d_af   = d.af();
                    d.bind(1, at.id);
                    d.bind(2, pp.first.id);
                    d.exec();
                },
                [&](const ClassCountMap::value_type& pp){   // MODIFIED
                    ret.modified.insert(pp.first);

                    static thread_local CacheQuery  u("UPDATE AClasses SET hops=? WHERE atom=? AND class=?");
                    auto u_af   = u.af();
                    u.bind(1, pp.second);
                    u.bind(2, at.id);
                    u.bind(3, pp.first.id);
                    u.exec();
                },
                [&](const ClassCountMap::value_type& pp){   // INSERTED
                    ret.added.insert(pp.first);
                    
                    static thread_local CacheQuery  i("INSERT INTO AClasses (atom, class, hops) VALUES (?,?,?,?)");
                    auto i_af   = i.af();
                    i.bind(1, at.id);
                    i.bind(2, pp.first.id);
                    i.bind(3, pp.second);
                    i.exec();
                }
            );
            return ret;
        }

        SetChanges<Class>   atu_classes(Atom at, KVTree& attrs, bool autoDeleteKeys=true)
        {
            auto clsset     = make_set(cdb::db_classes(attrs.values_set(svL_aClass)));
            if(autoDeleteKeys)
                attrs.erase_all(svL_aClass);
            return atu_classes(at, clsset);
        }


        //  ---------------------------------------------------------------------------------------------------------------

        void    ati_tags(Atom at, const TagSet& tags)
        {
            static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?,?)");
            for(Tag t : tags){
                iTag.bind(1, at.id);
                iTag.bind(2, t.id);
                iTag.exec();
            }
        }

        void    ati_tags(Atom at, KVTree& attrs, bool autoDeleteKeys=true)
        {
            auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
            if(autoDeleteKeys)
                attrs.erase_all(svL_aTag);
            ati_tags(at, tagset);
        }

        SetChanges<Tag>    atu_tags(Atom at, const TagSet& tags)
        {
            TagSet              otags   = make_set(cdb::tags(at));
            SetChanges<Tag> ret = add_remove(otags, tags);
            if(!ret.removed.empty()){
                static thread_local CacheQuery dTag("DELETE FROM ATags WHERE atom=? AND tag=?");
                for(Tag t : ret.removed)
                    dTag.exec(at.id, t.id);
            }
            if(!ret.added.empty()){
                static thread_local CacheQuery iTag("INSERT INTO ATags (atom, tag) VALUES (?, ?)");
                for(Tag t : ret.added)
                    iTag.exec(at.id, t.id);
            }
            return ret;
        }

        SetChanges<Tag>    atu_tags(AtomChangeData& acd, Atom at, KVTree& attrs, bool autoDeleteKeys=true)
        {
            auto tagset = make_set(cdb::db_tags(attrs.values_set(svL_aTag)));
            if(autoDeleteKeys)
                attrs.erase_all(svL_aTag);
            return atu_tags(at, tagset);
        }

        //  ---------------------------------------------------------------------------------------------------------------

        std::string     atq_title(Atom at, const KVTree& attrs)
        {
            std::string    t   = std::string(attrs.value(svL_aTitle));
            if(!t.empty())
                return t;
            return cdb::key(at);
        }

        void    ati_title(Atom at, const KVTree& attrs)
        {
            static thread_local CacheQuery uTitle("UPDATE Atoms SET name=? WHERE id=?");
            std::string    t   = atq_title(at, attrs);
            uTitle.bind(1, t);
            uTitle.bind(2, at);
            uTitle.exec();
        }

        bool    atu_title(Atom at, const KVTree& attrs)
        {
            std::string     n   = cdb::name(at);
            static thread_local CacheQuery uTitle("UPDATE Atoms SET name=? WHERE id=?");
            std::string    t   = atq_title(at, attrs);
            if(t != n){
                uTitle.bind(1, t);
                uTitle.bind(2, at);
                uTitle.exec();
                return true;
            } else
                return false;
        }

        //  ---------------------------------------------------------------------------------------------------------------

        std::string     atq_abbr(Atom at, const KVTree& attrs)
        {
            return std::string(attrs.value(svL_aAbbreviation));
        }

        void    ati_abbr(Atom at, const KVTree& attrs)
        {
            static thread_local CacheQuery uTitle("UPDATE Atoms SET abbr=? WHERE id=?");
            std::string    t   = atq_abbr(at, attrs);
            uTitle.bind(1, t);
            uTitle.bind(2, at);
            uTitle.exec();
        }

        bool    atu_abbr(Atom at, const KVTree& attrs)
        {
            std::string     n   = cdb::abbreviation(at);
            static thread_local CacheQuery uTitle("UPDATE Atoms SET abbr=? WHERE id=?");
            std::string    t   = atq_abbr(at, attrs);
            if(t != n){
                uTitle.bind(1, t);
                uTitle.bind(2, at);
                uTitle.exec();
                return true;
            } else
                return false;
        }

        //  ---------------------------------------------------------------------------------------------------------------

        void    ati_attributes(Atom at, KVTree& attrs)
        {
            Document    doc = cdb::document(at);
            auto rep    = diff::additions(doc, attrs);
            rep.exec_inserts();
        }

        void    atu_attributes(Atom at, KVTree& attrs)
        {
            Document    doc = cdb::document(at);
            auto rep = diff::changes(doc, attrs);
            rep.exec_inserts();
            rep.exec_reindex();
            rep.exec_removals();
        }

        //  ---------------------------------------------------------------------------------------------------------------

        bool    triggered(const AtomChangeData&acd, const AtomNotifier*an)
        {
            Class   c;
            Field   f;
            const AtomSpec&  as  = an->spec();
            switch(as.type){
            case AtomSpec::Never:
                return false;
            case AtomSpec::Always:
                return true;
            case AtomSpec::ByClassKey:
            case AtomSpec::ByClassId:
                c   = as.class_();
                if(an->change().is_set(Change::Added))
                    return acd.classes.added.contains(c);
                if(an->change().is_set(Change::Removed))
                    return acd.classes.removed.contains(c);
                if(an->change().is_set(Change::Modified))
                    return acd.classes.modified.contains(c);
                if(an->change().is_set(Change::None))
                    return acd.classes.same.contains(c);
                return false;
            case AtomSpec::ByFieldKey:
            case AtomSpec::ByFieldId:
                f   = as.field();
                if(an->change().is_set(Change::Added))
                    return acd.fields.added.contains(f);
                if(an->change().is_set(Change::Removed))
                    return acd.fields.removed.contains(f);
                if(an->change().is_set(Change::Modified))
                    return acd.fields.modified.contains(f);
                if(an->change().is_set(Change::None))
                    return acd.fields.same.contains(f);
                return false;
            default:
                return false;
            }
        }

        void    u_atom_execute(const AtomChangeData&acd)
        {
            static const std::vector<const AtomNotifier*>&  notifiers   = AtomNotifier::all();
            for(const AtomNotifier* an : notifiers){
                if(!an) [[unlikely]]
                    continue;
                if(triggered(acd, an))
                    an -> change(acd);
            }
        }

        void    i_atom_notify(Atom x, bool recursive=true)
        {
            AtomChangeData  acd;
            acd.atom            = x;
            acd.doc             = cdb::document(x);
            acd.classes.added   = make_set(cdb::classes(x));
            acd.tags.added      = make_set(cdb::tags(x));
            acd.title           = true;
            acd.abbreviation    = true;
            u_atom_execute(acd);
            
            //  If recursive
        }


        Atom    s3_atom_create(KVTree& attrs, Document doc)
        {
            Atom        x   = cdb::db_atom(doc);
            
            ati_classes(x, attrs);
            ati_title(x, attrs);
            ati_abbr(x, attrs);
            ati_tags(x, attrs);
            ati_attributes(x, attrs);
            return x;
        }

        void    s3_atom_bind(Atom, bool recursive=true)
        {
        }

        void    s3_atom_notify(Atom x, bool recursive=true)
        {
            i_atom_notify(x, recursive);
        }

        void    i_atom(Atom x, KVTree& attrs)
        {
            ati_classes(x, attrs);
            ati_title(x, attrs);
            ati_tags(x, attrs);
            ati_abbr(x, attrs);
            ati_attributes(x, attrs);
            i_atom_notify(x);
        }

        void    u_atom(Atom x, KVTree& attrs)
        {
            AtomChangeData acd;
            acd.doc             = cdb::document(x);
            acd.atom            = x;
            acd.classes         = atu_classes(x, attrs);
            acd.title           = atu_title(x, attrs);
            acd.abbreviation    = atu_abbr(x, attrs);
            acd.tags            = atu_tags(acd, x, attrs);
            atu_attributes(x, attrs);
            u_atom_execute(acd);
        }

        Atom    u_atom(KVTree& attrs, Document doc)
        {
            bool    created = false;
            Atom    x   = cdb::db_atom(doc, &created);
            if(created){
                i_atom(x, attrs);
            } else {
                u_atom(x, attrs);
            }
            return x;
        }


        #if 0


        void    u_atom(Atom at, Document doc, const Attribute::Report& rep, cdb_options_t opts, const ClassSet& cset)
        {
        //yInfo() << "Update on " << cdb::key(at) << " ... with " << cset.size() << " class(es)";
            ClassSet    oldcls    = make_set(cdb::classes(at));
            ClsHopsMap  newhops;
            //determine_hops(newhops, cset);
            //update_aclasses(at, doc, newhops);
            ClassSet    newcls    = make_set(cdb::classes(at));
            auto        clschgs   = changes(oldcls, newcls);
            
            static auto&    anotes  = AtomNotifier::change_map();
            
            for(Class c : clschgs.removed){
                for(const AtomNotifier* an : anotes[Change::Removed]){
                    if(!an)
                        continue;
                    //an -> change(at, c, Change::Removed);
                }
            }
            
            for(Class c : clschgs.added){
                for(const AtomNotifier* an : anotes[Change::Added]){
                    if(!an)
                        continue;
                    //an -> change(at, c, Change::Added);
                }
            }
            
            for(Class c : clschgs.same){
                for(const AtomNotifier* an : anotes[Change::Modified]){
                    if(!an)
                        continue;
                    //an -> change(at, c, Change::Modified);
                }
            }
        }
        #endif
    
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  CATEGORY
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Category::SharedData    u_category_update(Category x, cdb_options_t opts);
        
        void    u_category_erase(Category x)
        {
            static thread_local CacheQuery  stmts[] = {
                CacheQuery( "UPDATE Classes SET category=0 WHERE category=?" ),
                CacheQuery( "DELETE FROM Categories WHERE id=?" )
            };
            
            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    u_category_erase(Document doc)
        {
            u_category_erase(cdb::category(doc));
        }

        void    u_category_icon(Category x)
        {
            if(!x)
                return ;

            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            
            if(img != cdb::icon(x)){
                static thread_local CacheQuery u1("UPDATE Categories SET icon=? WHERE id=?");
                static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");
                
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    u_category_notify(Fragment frag,Change chg)
        {
            Document    doc = cdb::document(frag);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    u_category_erase(doc);
                    return ;
                }
            }
            
            bool created = false;
            Category         x   = cdb::db_category(doc, &created);
            if(!x)
                return ;

            cdb_options_t       opts    = cdb::U_INFO | cdb::IS_UPDATE;
            if(created)
                opts |= cdb::U_ICON;

            Category::SharedData data = u_category_update(x, opts);
            if(!data){
                yWarning() << "Unable to update category '" << cdb::key(x) << "' due to lack of data";
                return ;
            }
        }

        void    u_category_notify_icons(Fragment frag,Change)
        {
            u_category_icon(cdb::category(cdb::document(frag), true));
        }


        void    u_category_stage3(Document doc)
        {
            Category    x       = cdb::db_category(doc);
            Category::SharedData data = u_category_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON|cdb::IS_UPDATE);
            if(!data){
                yWarning() << "Unable to update category '" << cdb::key(x) << "' due to lack of data";
                return ;
            }
        }

        Category::SharedData    u_category_update(Category x, cdb_options_t opts)
        {
            if(!x)
                return Category::SharedData();

            if(opts & cdb::U_ICON)
                u_category_icon(x);
            
            Category::SharedData data = cdb::merged(x, opts);
            if(!data)
                return Category::SharedData();
                
            if(opts & cdb::U_INFO){
                static thread_local CacheQuery u("UPDATE Categories SET name=?,brief=? WHERE id=?");
                u.bind(1, data->name);
                u.bind(2, data->brief);
                u.bind(3, x.id);
                u.exec();
            }
            return data;
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  CLASS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        void                u_class_erase(Class x)
        {
            static thread_local CacheQuery stmts[] = {
                CacheQuery("DELETE FROM CAlias WHERE class=?"),
                CacheQuery("DELETE FROM CPrefix WHERE class=?"),
                CacheQuery("DELETE FROM CSuffix WHERE class=?"),
                CacheQuery("DELETE FROM CTags WHERE class=?"),
                CacheQuery("DELETE FROM CLookup WHERE class=? AND priority=1"),
                CacheQuery("DELETE FROM Classes WHERE id=?")
            };
            
            if(!x)
                return ;
                
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void                u_class_erase(Document doc)
        {
            u_class_erase(cdb::class_(doc));
        }

        void                u_class_icon(Class x)
        {
            if(!x)
                return ;
                
            static thread_local CacheQuery u1("UPDATE Classes SET icon=? WHERE id=?");
            static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            if(img != cdb::icon(x)){
                    // we'll eventually do inheritance....
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    u_class_notify(Fragment frag, Change chg)
        {
            Document    doc = cdb::document(frag);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    u_class_erase(doc);
                    return ;
                }
            }

            bool        created = false;
            Class       x   = cdb::db_class(doc, &created);
            if(!x)
                return ;

            if(!x)
                return;
            
            Class::SharedData data = cdb::merged(x, cdb::DONT_LOCK|cdb::IS_UPDATE);
            if(!data)
                return;
            
            Category    cat;
            if(!data->category.empty())
                cat = cdb::db_category(data->category);

            static thread_local CacheQuery uInfo("UPDATE Classes SET name=?, plural=?, brief=?, category=?, binding=? WHERE id=?");

            uInfo.bind(1, data->name);
            uInfo.bind(2, data->plural);
            uInfo.bind(3, data->brief);
            uInfo.bind(4, cat.id);
            uInfo.bind(5, data->binding);
            uInfo.bind(6, x.id);
            uInfo.exec();

            
            static thread_local CacheQuery iTag("INSERT INTO CTags (class, tag) VALUES (?,?)");
            static thread_local CacheQuery dTag("DELETE FROM CTags WHERE class=? AND tag=?");

            std::set<Tag>   new_tags = cdb::find_tags_set(data->tags, true);
            
            if(created){
                u_class_icon(x);
                for(Tag t : new_tags)
                    iTag.exec(x.id, t.id);

                //  TODO inheritance....
                
            } else {
                std::set<Tag>   old_tags = make_set(cdb::tags(x));
                auto ch_tag = add_remove(old_tags, new_tags);
                for(Tag t : ch_tag.added)
                    iTag.exec(x.id, t.id);
                for(Tag t : ch_tag.removed)
                    dTag.exec(x.id, t.id);
                    
                // Do class analysis here

                //  TODO inheritance....
            }
        }

        void    u_class_notify_icons(Fragment frag, Change)
        {
            u_class_icon(cdb::class_(cdb::document(frag), true));
        }

        void    u_class_stage3_pass1_create(Document doc)
        {
            std::string k   = cdb::base_key(doc);
            if(k.empty())   //  skipping, no key
                return ;

            //  First, do a manual merge
            auto dp     = std::make_shared<Class::Data>();
            for(Fragment f : cdb::fragments(doc, DataRole::Config)){
                Class::SharedFile    p   = class_doc(f, cdb::DONT_LOCK);
                if(!p)
                    continue;
                cdb::update(f);
                dp->merge(*p, false);
            }
            
            Category    cat;
            if(!dp->category.empty())
                cat = cdb::db_category(dp->category);
            
            std::string name    = dp->name.empty() ? k : dp->name;
            std::string plural  = dp->plural.empty() ? ( name + 's' ) : name;
            
            Image       img     = cdb::best_image(doc);

            static thread_local CacheQuery iInfo("INSERT OR REPLACE INTO Classes (id,k,name,plural,brief,category,binding,icon,url,devurl) VALUES (?,?,?,?,?,?,?,?,?,?)");
            iInfo.bind(1, doc.id);
            iInfo.bind(2, k);
            iInfo.bind(3, name);
            iInfo.bind(4, plural);
            iInfo.bind(5, dp->brief);
            iInfo.bind(6, cat.id);
            iInfo.bind(7, dp->binding); // Maybe....?
            iInfo.bind(8, img.id);
            iInfo.bind(9, dp->url);
            iInfo.bind(10, dp->dev_url);
            iInfo.exec();
            
            Class   x{doc.id};
            
            static thread_local CacheQuery iTag("INSERT INTO CTags (class, tag) VALUES (?, ?)");
            for(std::string_view tk : dp->tags){
                Tag     t   = cdb::db_tag(tk);
                if(!t){
                    yWarning() << "Uncreatable tag " << tk << " referenced in class " << k;
                    continue;
                }
                iTag.bind(1, doc.id);
                iTag.bind(2, t.id);
                iTag.exec();
            }
            
            static thread_local CacheQuery iAlias("INSERT INTO CAlias (class, alias) VALUES (?,?)");
            
            for(auto& p : dp->prefixes){
                std::string a   = p+k;
                iAlias.exec(x.id, a);
             
                for(auto& s : dp->suffixes){
                    a   = p+k+s;
                    iAlias.exec(x.id, a);
                }
            }
            
            for(auto& s : dp->suffixes){
                std::string a   = k+s;
                iAlias.exec(x.id, a);
            }
            
            static thread_local CacheQuery iPrefix("INSERT INTO CPrefix (class, prefix) VALUES (?,?)");
            for(auto& p : dp->prefixes)
                iPrefix.exec(x.id, p);
                
            static thread_local CacheQuery iSuffix("INSERT INTO CSuffix (class, suffix) VALUES (?,?)");
            for(auto& s : dp->suffixes)
                iSuffix.exec(x.id, s);
            
        }

        void    u_class_stage3_pass2_bind(Document doc)
        {
            Class   x       = cdb::db_class(doc);
            auto dp         = cdb::merged(x, cdb::DONT_LOCK);
            if(!dp)
                return ;
            
            static thread_local CacheQuery iUse("INSERT INTO CDepends (class, base) VALUES (?,?)");
            for(std::string_view ck : dp->use){
                Class   y   = cdb::db_class(ck);
                if(!y){
                    yWarning() << "Uncreatable USE " << ck << " referenced in class " << cdb::key(x);
                    continue;
                }
                
                iUse.bind(1, x.id);
                iUse.bind(2, y.id);
                iUse.exec();
            }
            
            static thread_local CacheQuery iTarget("INSERT INTO CTargets (class, target) VALUES (?,?)");
            for(std::string_view ck : dp->targets){
                Class   y = cdb::db_class(ck);
                if(!y){
                    yWarning() << "Uncreatable TARGET " << ck << " referenced in class " << cdb::key(x);
                    continue;
                }

                iTarget.bind(1, x.id);
                iTarget.bind(2, y.id);
                iTarget.exec();
            }
            
            static thread_local CacheQuery iSource("INSERT INTO CSources (class, source) VALUES (?,?)");
            for(std::string_view ck : dp->sources){
                Class   y = cdb::db_class(ck);
                if(!y){
                    yWarning() << "Uncreatable SOURCE " << ck << " referenced in class " << cdb::key(x);
                    continue;
                }

                iSource.bind(1, x.id);
                iSource.bind(2, y.id);
                iSource.exec();
            }

        #if 0
            static thread_local CacheQuery iReverse("INSERT INTO CReverses (class, reverse) VALUES (?,?)");
            for(std::string_view ck : dp->reverse){
                Class   y = cdb::db_class(ck);
                if(!y){
                    yWarning() << "Uncreatable REVERSE " << ck << " referenced in class " << cdb::key(x);
                    continue;
                }

                iReverse.bind(1, x.id);
                iReverse.bind(2, y.id);
                iReverse.exec();
            }
        #endif

        }

        void    u_class_stage3_pass3_extend(Document doc)
        {
            Class x = cdb::db_class(doc);
            if(!x)
                return;


            static thread_local CacheQuery iUse("INSERT INTO CDepends (class, base, hops) VALUES (?,?,?)");

            ClassCountMap   bases  = cdb::make_count_map(cdb::base_classes_ranked(x));
            ClassCountMap   copy    = bases;
            for(auto & itr : copy){
                for(auto& cr : cdb::base_classes_ranked(itr.first)){
                    if(bases.contains(cr.cls))        // already in our base list
                        continue;
                    uint64_t    cnt   = 1ULL + itr.second + cr.rank;
                    bases[cr.cls]     = HCountU64{ cnt };  // insert so we don't override
                }
            }

            for(auto & itr : bases){
                if(!itr.second.cnt)
                    continue;
                    
                iUse.bind(1, x.id);
                iUse.bind(2, itr.first.id);
                iUse.bind(3, itr.second.cnt);
                iUse.exec();
                iUse.reset();
            }
        }

        void    u_class_stage3_pass4_deduce(Document doc)
        {
            Class x = cdb::db_class(doc);
            if(!x)
                return;
                
                
            /* This should be the finalize step... */
            
            ClassCountMap   bases  = cdb::make_count_map(cdb::base_classes_ranked(x));
            
        }

        void    u_class_update(Class x)
        {
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  CSS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        void    css__(cdb_options_t opts=0)
        {
            std::string       css;
            for(Fragment f : cdb::fragments(".css", DataRole::Style)){
                css += cdb::frag_string(f, opts);
                css += '\n';
            }
                
            ByteArray           newCssData;
            {
                stream::Bytes       ncss(newCssData);
                
                std::string     newTxtColor = "black";
                std::string     newBkColor  = "white";

                Map<std::string, std::string_view, IgCase> vars;
                bool    inBody  = false;
                vsplit(css, '\n', [&](std::string_view l){
                    if(l.empty())
                        return;
                        
                    l = trimmed(l);
                    if(inBody){
                        if(l[0] == '}'){
                            inBody  = false;
                            return;
                        }
                        size_t i  = l.find_first_of(':');
                        if(i == std::string_view::npos)
                            return;
                        std::string       k     = to_lower(trimmed(l.substr(0,i)));
                        std::string_view  v     = trimmed(l.substr(i+1));
                        
                        
                        if(is_similar(k, "color"))
                            newTxtColor      = v;
                        if(is_similar(k, "background-color"))
                            newBkColor      = v;
                        vars[k]     = v;
                    } else if(starts_igCase(l, "body")) {
                        inBody  = true;
                    }
                });
                
                gTextColor  = newTxtColor;
                gBkColor    = newBkColor;
                
                ncss << css;
                
                if(!vars.empty()){
                    ncss << "\n/*\n    Variables auto-picked from existing CSS.\n*/\n";
                    ncss << ":root {\n";
                    for(auto& j : vars)
                        ncss << "    --" << j.first << ": " << j.second << '\n';
                    ncss << "}\n\n";
                }
                if(gHasBackground){
                    ncss << "body {\n"
                            "    background-image: url(\"/background\");\n"
                            "}\n";
                }
                
                ncss << file_string(gSharedCssFile);
            }
            gCss        = std::make_shared<ByteArray>(std::move(newCssData));
        }

        void    css_stage4()
        {
            css__(DONT_LOCK);
        }

        void    css_update()
        {
            css__();
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  FIELD
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        #if 0

            // called POST class-expansion
        void    field_s3_classes(Document doc)
        {
            Field               x   = db_field(doc);
            Field::SharedData data  = merged(x, DONT_LOCK);
            if(!data){
                yWarning() << "Unable to initialize field '" << key(x) << "'";
                return ;
            }
            
            static thread_local CacheQuery iAtoms("INSERT INTO FAtomTypes (field, class) VALUES (?,?)");
            iAtoms.batch(x.id, ids_for(classes_set(data->atoms, true)));
        }

            // called BEFORE class-expansion
        void    field_s3_init(Document doc)
        {
            Field            x   = db_field(doc);
            Field::SharedData data   = update_this(x, DONT_LOCK|U_INFO|U_ICON|U_TAGS);
            if(!data){
                yWarning() << "Unable to initialize field '" << key(x) << "'";
                return ;
            }
            
            static thread_local CacheQuery iClass("INSERT INTO CFields (field, class, hops) VALUES (?, ?, 0)");
            
            Class   c = class_(x);
            if(c){
                iClass.exec(x.id, c.id);
            } else
                iClass.batch(x.id, ids_for(classes_set(data->classes, true)));
        }

        void    field_update(Fragment frag, Change chg)
        {
            Document    doc = document(frag);
            if(chg == Change::Removed){
                if(fragments_count(doc) <= 1){
                    update::field_erase(doc);
                    return ;
                }
            }

            bool        created = false;
            Field       x   = db_field(doc, &created);
            if(!x)
                return ;
                
            cdb_options_t       opts    = U_INFO|U_TAGS|IS_UPDATE;
            if(created)
                opts |= U_ICON;
                
            Field::SharedData   data    = update_this(x, opts);
            if(!data){
                yWarning() << "Unable to update field '" << key(x) << "'";
                return ;
            }
        }

        #endif


        void    u_field_erase(Field x)
        {
            static thread_local CacheQuery stmts[] = {
                CacheQuery("DELETE FROM CFields WHERE field=?"),
                CacheQuery("DELETE FROM FTags WHERE field=?"),
                CacheQuery("DELETE FROM Fields WHERE id=?")
            };

            if(!x)
                return ;
            
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    u_field_erase(Document doc)
        {
            u_field_erase(field(doc));
        }

        void    u_field_icon(Field x)
        {
            if(!x)
                return ;
                
            Document    doc     = document(x);
            Image       img     = best_image(doc);
            
            static thread_local CacheQuery u1("UPDATE Fields SET icon=? WHERE id=?");
            static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

            if(icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    u_field_notify(Fragment frag, Change chg)
        {
        }

        void    u_field_notify_icons(Fragment frag, Change)
        {
            u_field_icon(field(document(frag), true));
        }

        void    u_field_stage3(Document doc)
        {
            std::string k   = skeyc(doc);
            if(k.empty())   //  skipping, no key
                return ;
            
            if(k.empty())
                return;

            std::string_view    ck  = k;
            Class               cls;
            size_t  n   = ck.find('.');
            bool            anycls  = true;
            if(n != std::string_view::npos){
                cls     = db_class(ck.substr(0,n));
                ck      = ck.substr(n+1);
                anycls  = false;
            }
            
            
            //  For the data, do a manual merge
            auto dp     = std::make_shared<Field::Data>();
            for(Fragment f : fragments(doc, DataRole::Config)){
                Field::SharedFile    p   = field_doc(f, DONT_LOCK);
                if(!p)
                    continue;
                update(f);
                dp->merge(*p, false);
            }

            Category    cat;
            if(!dp->category.empty())
                cat = db_category(dp->category);

            Image       img     = best_image(doc);
            

            static thread_local CacheQuery iField("INSERT INTO FIELDS (id,k,class,ck,expected,icon,name,plural,pkey,brief,multi,restrict,category,maxcnt,anycls) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
                
            iField.bind(1, doc.id);
            iField.bind(2, k);
            iField.bind(3, cls.id);
            iField.bind(4, ck);
            iField.bind(5, dp->expected);
            iField.bind(6, img.id);
            iField.bind(7, dp->name);
            iField.bind(8, dp->plural);
            iField.bind(9, dp->pkey);
            iField.bind(10, dp->brief);
            iField.bind(11, (int) dp->multiplicity);
            iField.bind(12, (int) dp->restriction);
            iField.bind(13, cat.id);
            iField.bind(14, dp->max_count);
            iField.bind(15, anycls);
            iField.exec();
            iField.reset();
            
            Field   x   = { doc.id };
            
            static thread_local CacheQuery iTag("INSERT INTO FTags (field, tag) VALUES (?,?)");
            for(Tag t : find_tags_set(dp->tags, true))
                iTag.exec(x.id, t.id);

            static thread_local CacheQuery iAlias("INSERT INTO FAlias (field, alias) VALUES (?,?)");
            for(std::string_view s : dp->aliases)
                iAlias.exec(x.id, s);

            static thread_local CacheQuery iDataType("INSERT INTO FDataTypes (field, type) VALUES (?,?)");
            for(std::string_view s : dp->types){
                const TypeInfo* ti  = TypeInfo::find(s);
                if(!ti){
                    yInfo() << "Invalid data type '" << s << "' specified in field " << k;
                    continue;
                }
                
                iDataType.exec(x.id, ti->id());
            }
            
            static thread_local CacheQuery iAtomType("INSERT INTO FAtomTypes (field, class, hops) VALUES (?, ?, ?)");
            auto cset   = make_set(db_classes(dp->atoms));
            for(auto itr : derived_classes_ranked_merged_map(cset, true)){
                iAtomType.bind(1, x.id);
                iAtomType.bind(2, itr.first.id);
                iAtomType.bind(3, itr.second.cnt);
                iAtomType.exec();
            }

            static thread_local CacheQuery iClass0("INSERT INTO CFields (class, field, hops) VALUES (?, ?, 0)");
            iClass0.exec(cls.id, x.id);
            
            static thread_local CacheQuery iClassA("INSERT INTO CFields (class, field, hops) VALUES (?, ?, -1)");
            static thread_local CacheQuery iClass( "INSERT INTO CFields (class, field, hops) VALUES (?, ?, ?)");
            
            if(anycls){
                for(Class c : all_classes())
                    iClassA.exec(c.id, x.id);
                    
                
            } else {
                for(auto& itr : derived_classes_ranked(cls)){
                    iClass.bind(1, itr.cls.id);
                    iClass.bind(2, x.id);
                    iClass.bind(3, 1+itr.rank);
                    iClass.exec();
                }
            }
            
            
            
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  IMAGES
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void    u_image_thumbnails(Image img, cdb_options_t opts=0);

        yq::Size2U    size_for(const Magick::Geometry& sz)
        {
            return { (unsigned) sz.width(), (unsigned) sz.height() };
        }


        void    u_image_erase(Image x)
        {
            if(!x)
                return ;
                
            static thread_local CacheQuery stmts[] = {
                CacheQuery("UPDATE Atoms SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Categories SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Classes SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Documents SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Fields SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Leafs SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Tags SET icon=0 WHERE icon=?"),
                CacheQuery("DELETE FROM Images WHERE id=?")
            };
            
            if(!x)
                return ;
                
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    u_image_erase(Fragment frag)
        {
            u_image_erase(cdb::image(frag));
        }

        void    u_image_notify(Fragment frag, Change chg)
        {
            switch(chg){
            case Change::Added:
            case Change::Modified:
                u_image_thumbnails(cdb::db_image(frag));
                break;
            case Change::Removed:
                u_image_erase(frag);
                break;
            default:
                break;
            }
        }

        void    u_image_stage2()
        {
            Magick::InitializeMagick( BasicApp::app_name().data());
        }

        void    u_image_stage3(Document doc)
        {
            for(Fragment frag : cdb::fragments(doc))
                u_image_thumbnails(cdb::db_image(frag), cdb::DONT_LOCK|cdb::IS_UPDATE);
        }

        void    u_image_thumbnails(Image img, cdb_options_t opts)
        {
            if(!img)
                return;
            if(!cdb::is_raster(img))
                return ;

            Magick::Blob            bS, bM, bL;
            Fragment                frag = cdb::fragment(img);
            
            static thread_local CacheQuery u("UPDATE Images SET small=?,medium=?,large=? WHERE id=?");
            auto u_af = u.af();

            std::filesystem::path   fp  = cdb::path(frag);

            Fragment::Lock lk;
            if(!(opts&cdb::DONT_LOCK)){
                lk = Fragment::Lock::read(frag);
                if(!lk){
                    yWarning() << "Unable to get fragment lock for " << fp;
                    return;
                }
            }
                
            try {
                Magick::Image theImage( fp.string());
                
                std::string magickString    = theImage.magick();    // needed for saving

                Size2U isize   = size_for(theImage.size());
                Size2U ssize   = shrink_to_fit_within(isize, Image::kSmall);
                Size2U msize   = shrink_to_fit_within(isize, Image::kMedium);
                Size2U lsize   = shrink_to_fit_within(isize, Image::kLarge);
                
                if(ssize != isize){
                    Magick::Image   imgSmall    = theImage;
                    imgSmall.resize(Magick::Geometry(ssize.x, ssize.y));
                    imgSmall.write(&bS);
                } else
                    theImage.write(&bS);

                if(msize != isize){
                    Magick::Image   imgMedium   = theImage;
                    imgMedium.resize(Magick::Geometry(msize.x, msize.y));
                    imgMedium.write(&bM);
                } else
                    theImage.write(&bM);
                
                if(lsize != isize){
                    Magick::Image   imgLarge    = theImage;
                    imgLarge.resize(Magick::Geometry(lsize.x, lsize.y));
                    imgLarge.write(&bL);
                } else
                    theImage.write(&bL);
            }
            catch( Magick::Exception& error_)
            {
                yError()  << "Caught exception: " << error_.what();
            }
            
            if(bS.length() != 0)
                u.bind(1, bS.data(), bS.length());
            else
                u.bind(1);
            if(bM.length() != 0)
                u.bind(2, bM.data(), bM.length());
            else
                u.bind(2);
            if(bL.length() != 0)
                u.bind(3, bL.data(), bL.length());
            else
                u.bind(3);
            
            u.bind(4, img.id);
            u.exec();
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  LEAF
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////



        /*

            LEAF is a markdown document with attributes defined at the top.
            
            LEAF will posit an atom, the defacto "home" for a (big/important) scope atom, 
            however, an atom can be defined elsewhere.

        */



        #if 0
        void         u_leaf(Document doc, cdb_options_t opts)
        {
            bool    created = false;
            Leaf        x   = (opts & REMOVED) ? cdb::leaf(doc) : cdb::db_leaf(doc, &created);
            if(!x)
                return ;
            
            created = created || (opts & STARTUP);
            if(created)
                opts |= U_ICON | CREATED;

            if(opts & U_ICON)
                u_leaf_icon(x);

            auto dp  = merged(x, opts|IS_UPDATE);
            if(!dp){
                yWarning() << "Unable to load/merge leaf data for '" << key(x) << "'";
                return;
            }
            
            static thread_local CacheQuery uInfo("UPDATE Leafs SET title=?, abbr=?, brief=? WHERE id=?");
            if(opts & U_INFO){
                std::string_view title   = dp->title();
                std::string_view abbr    = dp->abbr();
                std::string_view brief   = dp->brief();
                if(title.empty())
                    title       = dp->attrs.value(kv::key({"nick", "name"}));
                
                if(title.empty()){
                    uInfo.bind(1, key(x));  // fall back (for now) 
                                        // TODO ... make this smarter (later)
                } else 
                    uInfo.bind(1, title);
                uInfo.bind(2, abbr);
                uInfo.bind(3, brief);
                uInfo.bind(4, x.id);
                uInfo.exec();
            }
            
            static thread_local CacheQuery dTag("DELETE FROM LTags WHERE leaf=? AND tag=?");
            static thread_local CacheQuery iTag("INSERT INTO LTags (leaf, tag) VALUES (?,?)");
            
            if(opts & U_TAGS){
                std::set<Tag>   old_tags = tags_set(x);
                std::set<Tag>   new_tags = find_tags_set(dp->tags(), true);

                auto ch_tag = add_remove(old_tags, new_tags);
                iTag.batch(x.id, ids_for(ch_tag.added));
                dTag.batch(x.id, ids_for(ch_tag.removed));
            }

            Attribute::Report   rep;
            Atom                xa;
            
            if(created){
                xa              = db_atom(doc);
            } else {
                xa              = find_atom(doc);
            }
            
            if(opts & U_ATTRIBUTES){
                if(created){
                    rep             = diff::additions(doc, dp->attrs);
                    rep.exec_inserts();
                } else {
                    rep             = diff::changes(doc, dp->attrs);
                    rep.exec_inserts();
                    rep.exec_reindex();
                    rep.exec_removals();
                }

                std::set<Class>     clsset = make_set(db_classes(dp->classes()));
                u_atom(xa, doc, rep, opts, clsset);

                if(opts & REMOVED){
                    if(fragments_count(doc) <= 1)
                        u_leaf_erase(x);
                }
            }
        }
        #endif

        void    u_leaf_erase(Leaf x)
        {
            static thread_local CacheQuery  stmts[] = {
                CacheQuery( "DELETE FROM LTags WHERE leaf=?" ),
                CacheQuery( "DELETE FROM Leafs WHERE id=?" )
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        #if 0
        //  UNUSED
        void    u_leaf_erase(Document doc)
        {
            u_leaf_erase(cdb::leaf(doc));
        }
        #endif

        void    u_leaf_icon(Leaf x)
        {
            if(!x)
                return ;
                
            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            static thread_local CacheQuery u1("UPDATE Leafs SET icon=? WHERE id=?");
            static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");

            if(cdb::icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    u_leaf_notify(Fragment frag, Change chg)
        {
            if(hidden(folder(frag)))
                return;
            cdb_options_t   opts    = IS_UPDATE | U_TAGS | U_ATTRIBUTES | U_INFO;
            if(chg == Change::Removed)
                opts |= REMOVED;
            
            Document    doc = document(frag);
            Leaf        x   = cdb::db_leaf(document(frag));
            
            auto dp   = cdb::merged(x, IS_UPDATE | DONT_LOCK);
            if(!dp){
                yWarning() << "Unable to load leaf data for '" << cdb::key(doc) << "'";
                return ;
            }
            
            u_atom(dp->attrs, doc);
            
            //  if there's any context stuff, do it here.....
        }

        void    u_leaf_notify_icons(Fragment frag, Change)
        {
            u_leaf_icon(cdb::leaf(cdb::document(frag), true));
        }
        
        void    u_leaf_title(Leaf x, Leaf::SharedData dp)
        {
            auto t = dp->title();
            if(t != cdb::title(x)){
                static thread_local CacheQuery u("UPDATE Leafs SET title=? WHERE id=?");
                auto u_af = u.af();
                u.bind(1, t);
                u.bind(2, x.id);
                u.exec();
            }
        }
            
        void    s3_leaf_pass1(Document doc)
        {
            if(hidden(folder(doc)))
                return;
            
            Leaf    x   = cdb::db_leaf(doc);
            if(!x){
                yWarning() << "Unable to create leaf for '" << cdb::key(doc) << "'";
                return;
            }

            u_leaf_icon(x);
            
            auto dp   = cdb::merged(x, IS_UPDATE | DONT_LOCK);
            if(!dp){
                yWarning() << "Unable to load leaf data for '" << cdb::key(doc) << "'";
                return ;
            }
            
            u_leaf_title(x, dp);
            
            Atom xa = s3_atom_create(dp->attrs, doc);
            s3_atom_bind(xa);
            s3_atom_notify(xa);
            
            static thread_local CacheQuery u("UPDATE Leafs SET atom=? WHERE id=?");
            auto u_af = u.af();
            u.bind(1, xa.id);
            u.bind(2, x.id);
            u.exec();
        }

        void    s3_leaf_pass2(Document doc)
        {
            if(hidden(folder(doc)))
                return;
            
            Atom xa = cdb::find_atom(doc);
            if(!xa)
                return;
                
            s3_atom_bind(xa);
            s3_atom_notify(xa);
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ROOT
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void    u_set_root_icon(const Root*rt, Image img)
        {
            if(!rt)
                return ;
                
            static thread_local CacheQuery r("REPLACE INTO RootIcons (root, icon) VALUES (?, ?)");
            auto r_af = r.af();
            r.bind(1, rt->id);
            r.bind(2, img.id);
            r.step();
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TAGS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Tag::SharedData     u_tag_update(Tag, cdb_options_t opts=0);

        void    u_tag_erase(Tag x)
        {
            static thread_local CacheQuery  stmts[] = {
                CacheQuery( "DELETE FROM CTags WHERE tag=?" ),
                CacheQuery( "DELETE FROM FTags WHERE tag=?" ),
                CacheQuery( "DELETE FROM LTags WHERE tag=?" ),
                CacheQuery( "DELETE FROM Tags WHERE id=?" )
            };
            for(auto& sq : stmts)
                sq.exec(x.id);
        }

        void    u_tag_erase(Document doc)
        {
            u_tag_erase(cdb::find_tag(doc));
        }

        void    u_tag_icon(Tag x)
        {
            if(!x)
                return ;

            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);

            if(cdb::icon(x) != img){
                static thread_local CacheQuery u1("UPDATE Tags SET icon=? WHERE id=?");
                static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }

        void    u_tag_notify(Fragment frag,Change chg)
        {
            Document    doc = cdb::document(frag);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    u_tag_erase(doc);
                    return ;
                }
            }
            
            bool    created = false;
            Tag     x   = cdb::db_tag(doc, &created);
            if(!x)
                return ;
            
            cdb_options_t   opts    = cdb::IS_UPDATE|cdb::U_INFO|cdb::U_LEAF;
            if(created)
                opts |= cdb::U_ICON;
            

            Tag::SharedData data = u_tag_update(x, opts);
            if(!data){
                yWarning() << "Unable to update tag '" << cdb::key(x) << "'";
                return ;
            }
        }

        void    u_tag_notify_icons(Fragment frag,Change)
        {
            u_tag_icon(cdb::find_tag(cdb::document(frag), true));
        }

        void    u_tag_stage3(Document doc) 
        {
            Tag     x       = cdb::db_tag(doc);
            Tag::SharedData data = u_tag_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON);
            if(!data){
                yWarning() << "Unable to initialize tag '" << cdb::key(x) << "'";
                return ;
            }
        }

        void    u_tag_stage3_leaf(Document doc)
        {
            Tag     x       = cdb::db_tag(doc);
            Tag::SharedData data = u_tag_update(x, cdb::DONT_LOCK|cdb::U_LEAF);
            if(!data){
                yWarning() << "Unable to set tag leaf '" << cdb::key(x) << "'";
                return ;
            }
        }

        Tag::SharedData     u_tag_update(Tag x, cdb_options_t opts)
        {
            if(!x)
                return Tag::SharedData();

            if(opts & cdb::U_ICON)
                u_tag_icon(x);

            auto data  = cdb::merged(x, opts);
            if(!data)
                return Tag::SharedData();
             
            static thread_local CacheQuery uInfo("UPDATE Tags SET name=?,brief=? WHERE id=?");
            static thread_local CacheQuery uLeaf("UPDATE Tags SET leaf=? WHERE id=?");
            
            if(opts&cdb::U_INFO){
                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, x.id);
                uInfo.exec();
            }
            if(opts&cdb::U_LEAF){
                Leaf    l   = cdb::leaf(data->leaf);
                uLeaf.exec(l.id, x.id);
            }
            
            return data;
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  USER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //! Updates user data from files, return merged data
        User::SharedData        u_user_update(User, cdb_options_t opts=0);


        void    u_user_erase(User u)
        {
            static thread_local CacheQuery d("DELETE FROM Users WHERE id=?");
            d.exec(u.id);
        }

        void    u_user_erase(Document doc)
        {
            u_user_erase(cdb::user(doc));
        }

        void    u_user_icon(User x)
        {
            if(!x)
                return ;
                
            Document    doc     = cdb::document(x);
            Image       img     = cdb::best_image(doc);
            static thread_local CacheQuery u1("UPDATE Users SET icon=? WHERE id=?");
            static thread_local CacheQuery u2("UPDATE Documents SET icon=? WHERE id=?");
            
            if(cdb::icon(x) != img){
                u1.exec(img.id, x.id);
                u2.exec(doc.id, x.id);
            }
        }


        void    u_user_notify(Fragment frag, Change chg)
        {
            Document    doc = cdb::document(frag);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    u_user_erase(doc);
                    return ;
                }
            }

            bool    created = false;
            User    x   = cdb::db_user(doc, &created);
            if(!x)
                return ;
                
            cdb_options_t   opts = cdb::IS_UPDATE|cdb::U_INFO;
            if(created)
                opts |= cdb::U_ICON;

            User::SharedData data = u_user_update(x, opts);
            if(!data){
                yWarning() << "Unable to update user '" << cdb::key(x) << "'";
                return ;
            }
        }

        void    u_user_notify_icons(Fragment frag, Change)
        {
            u_user_icon(cdb::user(cdb::document(frag), true));
        }

        void    u_user_stage3(Document doc)
        {
            User    x   = cdb::db_user(doc);
            User::SharedData data = u_user_update(x, cdb::DONT_LOCK|cdb::U_INFO|cdb::U_ICON);
            if(!data){
                yWarning() << "Unable to initialize user '" << cdb::key(x) << "'";
                return ;
            }
        }

        //! Updates user data from files, return merged data
        User::SharedData    u_user_update(User x, cdb_options_t opts)
        {
            if(!x)
                return User::SharedData();

            if(opts & cdb::U_ICON)
                u_user_icon(x);

            auto data  = cdb::merged(x, opts|cdb::IS_UPDATE);
            if(!data)
                return User::SharedData();

            static thread_local CacheQuery uInfo("UPDATE Users SET name=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=? WHERE id=?");
            if(opts & cdb::U_INFO){
                uInfo.bind(1, data->name);
                uInfo.bind(2, data->brief);
                uInfo.bind(3, data->permissions.is_set( Permission::Owner ));
                uInfo.bind(4, data->permissions.is_set( Permission::Admin ));
                uInfo.bind(5, data->permissions.is_set( Permission::Writer ));
                uInfo.bind(6, data->permissions.is_set( Permission::Reader ));
                uInfo.bind(7, x.id);
                uInfo.exec();
            }

            return data;
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void reg_me()
        {
            using namespace yq;
        
            //  -----------------------------------------------
            //  WARNING... the following can be order dependent
            //  -----------------------------------------------
        
                // FIX any necessary paths
                
            gSharedCssFile      = wksp::shared(kStdCSS);;
            gSharedPageFile     = wksp::shared(kStdPage);

                //  I wanna get this separate, but for now....
            on_stage2<u_image_stage2>();
            
                // needed first for icon detection
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_stage3<u_image_stage3>(by_cache(z));

            auto    classes_lookup  = by_cache(classes_folder(), "*.class");
            auto    fields_lookup   = by_cache(fields_folder(), "*.field");
            auto    leafs_lookup    = by_cache("*.y");
            auto    tags_lookup     = by_cache(tags_folder(), "*.tag");
            
                //  Organization & users
            on_stage3<u_category_stage3>(by_cache(categories_folder(), "*.cat"));
            on_stage3<u_tag_stage3>(tags_lookup);
            on_stage3<u_user_stage3>(by_cache(users_folder(), "*.user"));
            
                //  Classes & fields
            on_stage3<u_class_stage3_pass1_create>(classes_lookup);
            on_stage3<u_class_stage3_pass2_bind>(classes_lookup);
            on_stage3<u_class_stage3_pass3_extend>(classes_lookup);
            
            on_stage3<u_field_stage3>(fields_lookup);
            

                //  LEAFS & atoms
            on_stage3<s3_leaf_pass1>(leafs_lookup);
            on_stage3<s3_leaf_pass2>(leafs_lookup);
            on_stage3<u_tag_stage3_leaf>(tags_lookup);

        
                //  STAGE 4 global related

            on_stage4<css_stage4>();        // <---  Must come AFTER background stage4
            on_stage4<page_stage4>();
            //on_stage4<u_leaf_stage4_cleanup>();
            
                // Images change first (for icon changes)
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_change<u_image_notify>(by_cache(z));

                // Rest are less order dependent... 

                
            on_change<css_update>(by_file(gSharedCssFile));
            on_change<css_update>(by_cache(top_folder(), ".css"));

            on_change<page_update>(by_file(gSharedPageFile));
            on_change<page_update>(by_cache(top_folder(), ".page"));
                
            on_change<u_category_notify>(by_cache(categories_folder(), "*.cat"));
            on_change<u_class_notify>(classes_lookup);
            on_change<u_field_notify>(fields_lookup);
            on_change<u_leaf_notify>(leafs_lookup);
            on_change<u_tag_notify>(tags_lookup);
            on_change<u_user_notify>(by_cache(users_folder(), "*.user"));
            
            for(const char* z : Image::kSupportedExtensionWildcards){
                on_change<u_class_notify_icons>(by_cache(classes_folder(), z));
                on_change<u_field_notify_icons>(by_cache(fields_folder(), z));
                on_change<u_leaf_notify_icons>(by_cache(z));
                on_change<u_user_notify_icons>(by_cache(users_folder(), z));

                on_change<u_category_notify_icons>(by_cache(categories_folder(), z));
                on_change<u_tag_notify_icons>(by_cache(tags_folder(), z));
            }
        }

    //  now outside the above function, we register....
        
    YQ_INVOKE( reg_me(); )
}

void update_css()
{
    css__();
}
