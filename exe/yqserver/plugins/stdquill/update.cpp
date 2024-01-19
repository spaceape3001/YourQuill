////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <MagickCore/magick-baseconfig.h>
//  Know this is obsolete, however, there's no good way to automate it otherwise w/o tolerating the warning
#define MAGICKCORE_QUANTUM_DEPTH MAGICKCORE_QUANTUM_DEPTH_OBSOLETE_IN_H
#define MAGICKCORE_HDRI_ENABLE MAGICKCORE_HDRI_ENABLE_OBSOLETE_IN_H
#include <Magick++.h>

#include <0/basic/BasicApp.hpp>
#include <0/basic/DelayInit.hpp>
#include <0/basic/Logging.hpp>
#include <0/basic/TextUtils.hpp>
#include <0/basic/Vector.hpp>
#include <0/io/DirUtils.hpp>
#include <0/io/FileUtils.hpp>
#include <0/io/StreamOps.hpp>
#include <0/io/stream/Bytes.hpp>
#include <0/io/stream/Text.hpp>
#include <0/math/shape/Size2.hxx>

#include <mithril/category/Category.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/category/CategoryData.hpp>
#include <mithril/category/CategoryDiff.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/graphviz/GraphvizBuilder.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/image/ImageDiff.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Notifier.hpp>
#include <mithril/notify/Stage2.hpp>
#include <mithril/notify/Stage3.hpp>
#include <mithril/notify/Stage4.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/tag/TagCDB.hpp>
#include <mithril/tag/TagData.hpp>
#include <mithril/tag/TagDiff.hpp>
#include <mithril/user/UserCDB.hpp>
#include <mithril/user/UserData.hpp>
#include <mithril/user/UserDiff.hpp>
#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

using namespace yq;
using namespace yq::mithril;

//#include "uLeaf.ipp"
//#include "uRoot.ipp"

#include <mithril/atom/AtomUpdate.hpp>
#include <mithril/class/ClassUpdate.hpp>
#include <mithril/document/DocumentUpdate.hpp>
#include <mithril/field/FieldUpdate.hpp>
#include <mithril/leaf/LeafUpdate.hpp>

//#include <0/basic/BasicApp.hpp>
//#include <0/basic/CollectionUtils.hpp>

//#include <0/meta/TypeInfo.hpp>




//#include <mithril/atom/AtomCDB.hpp>
//#include <mithril/class/ClassCDB.hpp>
//#include <mithril/folder/FolderCDB.hpp>
//#include <mithril/field/FieldCDB.hpp>
//#include <mithril/value/ValueCDB.hpp>
//#include <mithril/attribute/AttributeCDB.hpp>
//#include <mithril/document/DocumentCDB.hpp>
//#include <mithril/root/RootDir.hpp>
//#include <mithril/image/ImageCDB.hpp>
//#include <mithril/io/Strings.hpp>
//#include <mithril/leaf/LeafCDB.hpp>
//#include <mithril/leaf/LeafData.hpp>
//#include <mithril/notify/AtomNotifier.hpp>
//#include <mithril/category/CategoryCDB.hpp>
//#include <mithril/tag/TagCDB.hpp>
//#include <mithril/user/User.hpp>
//#include <mithril/user/UserCDB.hpp>

#include "common.hpp"

//#include <mithril/atom.hpp>
//#include <mithril/attribute.hpp>

//#include <mithril/class.hpp>
//#include <mithril/directory.hpp>
//#include <mithril/document.hpp>
//#include <mithril/field.hpp>
//#include <mithril/folder.hpp>
//#include <mithril/fragment.hpp>


//#include <mithril/leaf.hpp>
//#include <mithril/root.hpp>

//#include <mithril/user.hpp>
//#include <mithril/value.hpp>


alignas(64) Guarded<std::string>        gTextColor;
alignas(64) Guarded<std::string>        gBkColor;
alignas(64) std::atomic<bool>           gHasBackground{false};
alignas(64) Guarded<SharedByteArray>    gCss;
alignas(64) std::filesystem::path       gSharedCssFile;
alignas(64) std::filesystem::path       gSharedPageFile;
alignas(64) std::atomic<uint64_t>       gClassGraph{0};

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
using namespace yq::mithril::update;
//using namespace yq::mithril::cdb;


namespace {
    void    page__(cdb_options_t opts=0)
    {
        Ref<Template>        wTemplate;
        std::string     r;
        
        for(Fragment f : cdb::fragments(cdb::document(kPage))){
            r       = cdb::frag_string(f, opts);
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
        page__(cdb::DONT_LOCK);
    }
    
    void    page_update()
    {
        page__();
    }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  CATEGORY
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void    u_category(Category t, Change chg)
        {
            Category::Diff   x { 
                .x = t, 
                .chg = chg, 
                .key = cdb::key(t)
            };
            
            if(chg != Change::Added){
                Category::Info ii  = cdb::info(t);
                x.icon.from     = ii.icon;
                x.name.from     = std::move(ii.name);
                x.brief.from    = std::move(ii.brief);
            }
            
            Document doc    = cdb::document(t);
            if(chg != Change::Removed){
                auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
                if(!def){
                    yInfo() << "Category " << x.key << " cannot be read.";
                    return;
                }
                
                x.icon.to       = cdb::best_image(doc);
                x.name.to       = std::move(def->name);
                x.brief.to      = std::move(def->brief);
                x.notes         = std::move(def->notes);
            }

            static thread_local CacheQuery uDocIcon("UPDATE Documents SET icon=? WHERE id=?");
            static thread_local CacheQuery uCategoryInfo("UPDATE Categories SET name=?,icon=?,brief=? WHERE id=?");
            static thread_local CacheQuery xCategoryStmts[] = {
                CacheQuery( "UPDATE Classes SET category=0 WHERE category=?" ),
                CacheQuery( "DELETE FROM Categories WHERE id=?" )
            };

            if(chg != Change::Removed){
                if(x){
                    if(x.icon)
                        uDocIcon.exec(x.icon.to.id, doc.id);
                    
                    uCategoryInfo.bind(1, x.name.to);
                    uCategoryInfo.bind(2, x.icon.to.id);
                    uCategoryInfo.bind(3, x.brief.to);
                    uCategoryInfo.bind(4, t.id);
                    uCategoryInfo.exec();
                } 
            }
            
            Category::Notify::notify(x);
            
            if(chg == Change::Removed){
                for(auto& sq : xCategoryStmts)
                    sq.exec(t.id);
            }
        }

        void    s3_category(Document doc)
        {
            bool        created     = false;
            Category x       = cdb::db_category(doc, &created);
            u_category(x, Change::Added);
        }
        

        void    s5_category(Fragment f, Change chg)
        {
            Document    doc = cdb::document(f);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    Category x = cdb::category(doc);
                    if(x){
                        u_category(x, Change::Removed);
                    }
                    return ;
                }
                
                chg = Change::Modified;
            }
            
            bool        created     = false;
            Category x       = cdb::db_category(doc, &created);
            if(created){
                u_category(x, Change::Added);
            } else
                u_category(x, Change::Modified);
        }
        
        void    s5_category_icons(Fragment f, Change chg)
        {
            Category    x   = cdb::category(cdb::document(f), true);
            if(!x)
                return ;
            u_category(x, Change::Modified);
        }
        
    
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  CLASS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        void    class_graph_update()
        {
        }
    
    
        void    class_notify(Fragment frag,Change chg)
        {
            UClass::notify(frag, chg);
            class_graph_update();
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
            css__(cdb::DONT_LOCK);
        }

        void    css_update()
        {
            css__();
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  IMAGE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Size2U    size_for(const Magick::Geometry& sz)
        {
            return { (unsigned) sz.width(), (unsigned) sz.height() };
        }
        
        Size2U    u_thumbnail(Image x)
        {
            if(!x)
                return ZERO;
            if(!cdb::is_raster(x))
                return ZERO;
            
            Magick::Blob            bS, bM, bL;
            Size2U                  isize;
            
            static thread_local CacheQuery u("UPDATE Images SET small=?,medium=?,large=?,width=?,height=? WHERE id=?");
            auto u_af = u.af();
            
            Fragment                frag    = cdb::fragment(x);
            std::filesystem::path   fp  = cdb::path(frag);
            cdb::update(frag);
            
            try {
                Magick::Image theImage( fp.string());
                
                std::string magickString    = theImage.magick();    // needed for saving

                isize          = size_for(theImage.size());
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
                return {};
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
            u.bind(4, (int) isize.x);
            u.bind(5, (int) isize.y);
            u.bind(6, x.id);
            u.exec();

            return isize;
        }
        
        void    u_image(Image v, Change chg)
        {
            static thread_local CacheQuery xImage[] = {
                CacheQuery("UPDATE Atoms SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Categories SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Classes SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Documents SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Fields SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Leafs SET icon=0 WHERE icon=?"),
                CacheQuery("UPDATE Tags SET icon=0 WHERE icon=?"),
                CacheQuery("DELETE FROM Images WHERE id=?")
            };
        
            Image::Diff     x{.x = v, .chg = chg};
            
            if(chg != Change::Added){
                Image::Info     ii  = cdb::info(v);
                x.type              = ii.type;
                x.dim.from          = ii.dim;
            }
            
            if(chg != Change::Removed)
                x.dim.to            = u_thumbnail(v);

            Image::Notify::notify(x);
        
            if(chg == Change::Removed){
                for(auto& sq : xImage)
                    sq.exec(v.id);
            }
        }
        

        void    s2_image()
        {
            Magick::InitializeMagick( BasicApp::app_name().data());
        }
        
        void    s3_image(Document doc)
        {
            for(Fragment frag : cdb::fragments(doc))
                u_image(cdb::db_image(frag), Change::Added);
        }

        void    s5_image(Fragment frag, Change chg)
        {
            Image   v;
            switch(chg){
            case Change::Added:
            case Change::Modified:
                u_image(cdb::db_image(frag), chg);
                break;
            case Change::Removed:
                v   = cdb::image(frag);
                if(v)
                    u_image(v, chg);
                break;
            default:
                break;
            }
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  TAG
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void    u_tag(Tag t, Change chg)
        {
            Tag::Diff   x { 
                .x = t, 
                .chg = chg, 
                .key = cdb::key(t)
            };
            
            if(chg != Change::Added){
                Tag::Info       ii  = cdb::info(t);
                x.leaf.from     = ii.leaf;
                x.icon.from     = ii.icon;
                x.name.from     = std::move(ii.name);
                x.brief.from    = std::move(ii.brief);
            }
            
            static thread_local CacheQuery uDocIcon("UPDATE Documents SET icon=? WHERE id=?");
            static thread_local CacheQuery uTagInfo("UPDATE Tags SET name=?,icon=?,leaf=?,brief=? WHERE id=?");
            static thread_local CacheQuery xTagStmts[] = {
                CacheQuery( "DELETE FROM CTags WHERE tag=?" ),
                CacheQuery( "DELETE FROM FTags WHERE tag=?" ),
                CacheQuery( "DELETE FROM LTags WHERE tag=?" ),
                CacheQuery( "DELETE FROM Tags WHERE id=?" )
            };

            Document doc    = cdb::document(t);
            if(chg != Change::Removed){
                auto def        = cdb::merged(t, cdb::DONT_LOCK|cdb::IS_UPDATE);
                if(!def){
                    yInfo() << "Tag " << x.key << " cannot be read.";
                    return;
                }
                
                x.icon.to       = cdb::best_image(doc);
                x.name.to       = std::move(def->name);
                x.leaf.to       = cdb::leaf(def->leaf);
                x.brief.to      = std::move(def->brief);
                x.notes         = std::move(def->notes);

                if(x.icon)
                    uDocIcon.exec(x.icon.to.id, doc.id);

                if(x){
                    uTagInfo.bind(1, x.name.to);
                    uTagInfo.bind(2, x.icon.to.id);
                    uTagInfo.bind(3, x.leaf.to.id);
                    uTagInfo.bind(4, x.brief.to);
                    uTagInfo.bind(5, t.id);
                    uTagInfo.exec();
                } 
            }
            
            Tag::Notify::notify(x);
            
            if(chg == Change::Removed){
                for(auto& sq : xTagStmts)
                    sq.exec(t.id);
            }
        }

        void    s3_tag(Document doc)
        {
            bool        created     = false;
            Tag x       = cdb::db_tag(doc, &created);
            u_tag(x, Change::Added);
        }
        
        void    s3_tag_leaf(Document doc)
        {
            Tag    x   = cdb::tag(doc);
            if(!x)
                return ;
            u_tag(x, Change::Modified);
        }

        void    s5_tag(Fragment f, Change chg)
        {
            Document    doc = cdb::document(f);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    Tag x = cdb::find_tag(doc);
                    if(x){
                        u_tag(x, Change::Removed);
                    }
                    return ;
                }
                
                chg = Change::Modified;
            }
            
            bool        created     = false;
            Tag x       = cdb::db_tag(doc, &created);
            if(created){
                u_tag(x, Change::Added);
            } else
                u_tag(x, Change::Modified);
        }
        
        void    s5_tag_icons(Fragment f, Change chg)
        {
            Tag    x   = cdb::find_tag(cdb::document(f), true);
            if(!x)
                return ;
            u_tag(x, Change::Modified);
        }
        
        
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  USER
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
        void    u_user(User u, Change chg)
        {
            User::Diff   x { 
                .x = u, 
                .chg = chg, 
                .key = cdb::key(u)
            };
            
            if(chg != Change::Added){
                User::Info       ii  = cdb::info(u);
                x.icon.from     = ii.icon;
                x.name.from     = std::move(ii.name);
                x.brief.from    = std::move(ii.brief);
                x.owner.from    = ii.is_owner;
                x.admin.from    = ii.is_admin;
                x.writer.from   = ii.is_writer;
                x.reader.from   = ii.is_reader;
                x.guest.from    = ii.is_guest;
            }
            
            static thread_local CacheQuery uDocIcon("UPDATE Documents SET icon=? WHERE id=?");
            static thread_local CacheQuery uInfo("UPDATE Users SET name=?,icon=?,brief=?,is_owner=?,is_admin=?,is_writer=?,is_reader=?,is_guest=? WHERE id=?");

            Document    doc     = cdb::document(u);
            if(chg != Change::Removed){
                auto def        = cdb::merged(u, cdb::DONT_LOCK|cdb::IS_UPDATE);
                if(!def){
                    yInfo() << "User " << x.key << " cannot be read.";
                    return;
                }
                
                x.icon.to       = cdb::best_image(doc);
                x.name.to       = std::move(def->name);
                x.brief.to      = std::move(def->brief);
                x.owner.to      = def->permissions(Permission::Owner);
                x.admin.to      = def->permissions(Permission::Admin);
                x.writer.to     = def->permissions(Permission::Writer);
                x.reader.to     = def->permissions(Permission::Reader);
                x.guest.to      = def->permissions(Permission::Guest);
                
                if(x.icon)
                    uDocIcon.exec(x.icon.to.id, doc.id);
                
                if(x){
                    uInfo.bind(1, x.name.to);
                    uInfo.bind(2, x.icon.to.id);
                    uInfo.bind(3, x.brief.to);
                    uInfo.bind(4, x.owner.to);
                    uInfo.bind(5, x.admin.to);
                    uInfo.bind(6, x.writer.to);
                    uInfo.bind(7, x.reader.to);
                    uInfo.bind(8, x.guest.to);
                    uInfo.bind(9, u.id);
                    uInfo.exec();
                }
            }
            
            User::Notify::notify(x);
            
            static thread_local CacheQuery xUserStmts[] = {
                CacheQuery( "DELETE FROM Users WHERE id=?" )
            };

            if(chg == Change::Removed){
                for(auto& sq : xUserStmts)
                    sq.exec(u.id);
            }
        }
        
        void    s3_user(Document doc)
        {
            bool        created     = false;
            User x       = cdb::db_user(doc, &created);
            u_user(x, Change::Added);
        }
        
        void    s5_user(Fragment frag, Change chg)
        {
            Document    doc = cdb::document(frag);
            if(chg == Change::Removed){
                if(cdb::fragments_count(doc) <= 1){
                    User x = cdb::user(doc);
                    if(x){
                        u_user(x, Change::Removed);
                    }
                    return ;
                }
                
                chg = Change::Modified;
            }
            
            bool        created     = false;
            User x       = cdb::db_user(doc, &created);
            if(created){
                u_user(x, Change::Added);
            } else
                u_user(x, Change::Modified);
        }
        
        void    s5_user_icons(Fragment frag, Change chg)
        {
            User    x   = cdb::user(cdb::document(frag), true);
            if(!x)
                return ;
            u_user(x, Change::Modified);
        }
    
    
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void reg_me()
        {
            using namespace yq::mithril::cdb;
            
            //static constexpr FileSpec   tagsLookup(CACHE, cdb::tags_folder(), "*.tag");
        
            //  -----------------------------------------------
            //  WARNING... the following can be order dependent
            //  -----------------------------------------------
        
                // FIX any necessary paths
                
            gSharedCssFile      = wksp::shared(kStdCSS);;
            gSharedPageFile     = wksp::shared(kStdPage);

                //  I wanna get this separate, but for now....
            on_stage2<s2_image>();
            
                // needed first for icon detection
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_stage3<s3_image>(by_cache(z));
            
                //  Organization & users
            on_stage3<s3_category>(FileSpec(CACHE, cdb::categories_folder(), "*.cat"));
            on_stage3<s3_tag>(FileSpec(CACHE, cdb::tags_folder(), "*.tag"));
            on_stage3<s3_user>(FileSpec(CACHE, cdb::users_folder(), "*.user"));
            
                //  Classes & fields
            on_stage3<UClass::s3>(UClass::lookup());
            on_stage3<UClass::s3_bind>(UClass::lookup());
            on_stage3<UClass::s3_extend>(UClass::lookup());
            on_stage3<UClass::s3_derives>(UClass::lookup());
            
            on_stage3<UField::s3>(UField::lookup());
            
            on_stage3<UClass::s3_deduce>(UClass::lookup());
            on_stage3<UClass::s3_propagate>(UClass::lookup());

                //  LEAFS & atoms
            on_stage3<ULeaf::s3>(ULeaf::lookup());
            on_stage3<s3_tag_leaf>(FileSpec(CACHE, cdb::tags_folder(), "*.tag"));

        
                //  STAGE 4 global related

            on_stage4<css_stage4>();        // <---  Must come AFTER background stage4
            on_stage4<page_stage4>();

            on_stage4<class_graph_update>();
            on_stage4<UAtom::s4>();
            
            
            //on_stage4<u_leaf_stage4_cleanup>();
            
                // Images change first (for icon changes)
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_change<s5_image>(by_cache(z));

                // Rest are less order dependent... 

                
            on_change<css_update>(FileSpec(gSharedCssFile));
            on_change<css_update>(by_cache(top_folder(), ".css"));

            on_change<page_update>(FileSpec(gSharedPageFile));
            on_change<page_update>(by_cache(top_folder(), ".page"));
                
            on_change<s5_category>(FileSpec(CACHE, cdb::categories_folder(), "*.cat"));
            on_change<class_notify>(UClass::lookup());
            on_change<UField::notify>(UField::lookup());
            on_change<ULeaf::notify>(ULeaf::lookup());
            on_change<s5_tag>(FileSpec(CACHE, cdb::tags_folder(), "*.tag"));
            on_change<s5_user>(FileSpec(CACHE, cdb::users_folder(), "*.user"));
            
            for(const char* z : Image::kSupportedExtensionWildcards){
                on_change<s5_category_icons>(by_cache(categories_folder(), z));
                on_change<UClass::icons>(by_cache(classes_folder(), z));
                on_change<UField::icons>(by_cache(fields_folder(), z));
                on_change<ULeaf::icons>(by_cache(z));
                on_change<s5_tag_icons>(FileSpec(CACHE, tags_folder(), z));
                on_change<s5_user_icons>(FileSpec(CACHE, users_folder(), z));
            }
        }

    //  now outside the above function, we register....
        
    YQ_INVOKE( reg_me(); )
}

void update_css()
{
    css__();
}
