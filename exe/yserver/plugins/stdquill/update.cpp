////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <yq/container/ByteArray.hpp>
#include <yq/core/Guarded.hpp>
#include <atomic>
#include <filesystem>

#include "common.hpp"

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



//#include <magick/magick-baseconfig.h>
#include <MagickCore/magick-baseconfig.h>
//  Know this is obsolete, however, there's no good way to automate it otherwise w/o tolerating the warning
#define MAGICKCORE_QUANTUM_DEPTH MAGICKCORE_QUANTUM_DEPTH_OBSOLETE_IN_H
#define MAGICKCORE_HDRI_ENABLE MAGICKCORE_HDRI_ENABLE_OBSOLETE_IN_H
#include <Magick++.h>

#include <yq/core/BasicApp.hpp>
#include <yq/core/DelayInit.hpp>
#include <yq/core/Logging.hpp>

#include <yq/container/Vector.hpp>
#include <yq/file/DirUtils.hpp>
#include <yq/file/FileUtils.hpp>
#include <yq/core/StreamOps.hpp>
#include <yq/stream/Bytes.hpp>
#include <yq/stream/Text.hpp>
#include <yq/shape/Size2.hxx>
#include <yq/meta/TypeMeta.hpp>

#include <yq/mithril/atom/AtomCDB.hpp>
#include <yq/mithril/atom/AtomDiff.hpp>
#include <yq/mithril/attribute/AttributeDiff.hpp>
#include <yq/mithril/category/Category.hpp>
#include <yq/mithril/category/CategoryCDB.hpp>
#include <yq/mithril/category/CategoryData.hpp>
#include <yq/mithril/category/CategoryDiff.hpp>
#include <yq/mithril/class/ClassCDB.hpp>
#include <yq/mithril/class/ClassData.hpp>
#include <yq/mithril/class/ClassDiff.hpp>
#include <yq/mithril/document/DocumentCDB.hpp>
#include <yq/mithril/doodle/DoodleCDB.hpp>
#include <yq/mithril/field/FieldCDB.hpp>
#include <yq/mithril/field/FieldData.hpp>
#include <yq/mithril/field/FieldDiff.hpp>
#include <yq/mithril/folder/FolderCDB.hpp>
#include <yq/mithril/fragment/FragmentCDB.hpp>
#include <yq/mithril/graphviz/GraphvizBuilder.hpp>
#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/image/ImageCDB.hpp>
#include <yq/mithril/image/ImageDiff.hpp>
#include <yq/mithril/leaf/LeafCDB.hpp>
#include <yq/mithril/leaf/LeafData.hpp>
#include <yq/mithril/leaf/LeafDiff.hpp>
#include <yq/mithril/meta/Meta.hpp>
#include <yq/mithril/notify/FileWatch.hpp>
#include <yq/mithril/notify/FileNotifyAdapters.hpp>
#include <yq/mithril/notify/Notifier.hpp>
#include <yq/mithril/notify/Stage2.hpp>
#include <yq/mithril/notify/Stage3.hpp>
#include <yq/mithril/notify/Stage4.hpp>
#include <yq/mithril/novel/NovelCDB.hpp>
//#include <yq/mithril/novel/NovelData.hpp>
#include <yq/mithril/tag/Tag.hpp>
#include <yq/mithril/tag/TagCDB.hpp>
#include <yq/mithril/tag/TagData.hpp>
#include <yq/mithril/tag/TagDiff.hpp>
#include <yq/mithril/user/UserCDB.hpp>
#include <yq/mithril/user/UserData.hpp>
#include <yq/mithril/user/UserDiff.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>
#include <yq/mithril/wksp/Workspace.hpp>

using namespace yq;
using namespace yq::mithril;

//#include "uLeaf.ipp"
//#include "uRoot.ipp"

//#include <yq/mithril/atom/AtomUpdate.hpp>
//#include <yq/mithril/class/ClassUpdate.hpp>
//#include <yq/mithril/document/DocumentUpdate.hpp>
//#include <yq/mithril/field/FieldUpdate.hpp>
//#include <yq/mithril/leaf/LeafUpdate.hpp>

//#include <yq/core/BasicApp.hpp>
//#include <0/basic/CollectionUtils.hpp>

//#include <yq/meta/TypeMeta.hpp>




//#include <yq/mithril/atom/AtomCDB.hpp>
//#include <yq/mithril/class/ClassCDB.hpp>
//#include <yq/mithril/folder/FolderCDB.hpp>
//#include <yq/mithril/field/FieldCDB.hpp>
//#include <yq/mithril/value/ValueCDB.hpp>
//#include <yq/mithril/attribute/AttributeCDB.hpp>
//#include <yq/mithril/document/DocumentCDB.hpp>
//#include <yq/mithril/root/RootDir.hpp>
//#include <yq/mithril/image/ImageCDB.hpp>
//#include <yq/mithril/io/Strings.hpp>
//#include <yq/mithril/leaf/LeafCDB.hpp>
//#include <yq/mithril/leaf/LeafData.hpp>
//#include <yq/mithril/notify/AtomNotifier.hpp>
//#include <yq/mithril/category/CategoryCDB.hpp>
//#include <yq/mithril/tag/TagCDB.hpp>
//#include <yq/mithril/user/User.hpp>
//#include <yq/mithril/user/UserCDB.hpp>

//#include <yq/mithril/atom.hpp>
//#include <yq/mithril/attribute.hpp>

//#include <yq/mithril/class.hpp>
//#include <yq/mithril/directory.hpp>
//#include <yq/mithril/document.hpp>
//#include <yq/mithril/field.hpp>
//#include <yq/mithril/folder.hpp>
//#include <yq/mithril/fragment.hpp>


//#include <yq/mithril/leaf.hpp>
//#include <yq/mithril/root.hpp>

//#include <yq/mithril/user.hpp>
//#include <yq/mithril/value.hpp>


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
//using namespace yq::mithril::update;
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
}


#include "u/atom.ipp"
#include "u/category.ipp"
#include "u/class.ipp"
#include "u/css.ipp"
#include "u/doodle.ipp"
#include "u/field.ipp"
#include "u/image.ipp"
#include "u/leaf.ipp"
#include "u/novel.ipp"
#include "u/style.ipp"
#include "u/tag.ipp"
#include "u/user.ipp"
    
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
        void reg_me()
        {
            using namespace yq::mithril::cdb;
            
            const FileSpec   fsCategory(CACHE, cdb::categories_folder(), "*.cat");
            const FileSpec   fsClass(CACHE, cdb::classes_folder(), "*.cls");
            const FileSpec   fsField(CACHE, cdb::fields_folder(), "*.fld");
            const FileSpec   fsLeaf(CACHE, "*.y");
            const FileSpec   fsNovel(CACHE, "*.nw");
            const FileSpec   fsStyle(CACHE, cdb::styles_folder(), "*.sty");
            const FileSpec   fsTag(CACHE, cdb::tags_folder(), "*.tag");
            const FileSpec   fsUser(CACHE, cdb::users_folder(), "*.usr");
            const FileSpec   fsDoodle(CACHE, "*.d3x");
        
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
                on_stage3<s3_image>(FileSpec(CACHE, z));
            
                //  Organization & users
            on_stage3<s3_category>(fsCategory);
            on_stage3<s3_style>(fsStyle);
            on_stage3<s3_tag>(fsTag);
            on_stage3<s3_user>(fsUser);
            
                //  Classes & fields
            on_stage3<s3_class>(fsClass);
            on_stage3<s3_field>(fsField);
            
            on_stage3<s3_class_propagate>(fsClass);
            
                //  LEAFS & atoms
            on_stage3<s3_leaf>(fsLeaf);
            on_stage3<s3_tag_leaf>(fsTag);
            on_stage3<s3_doodle>(fsDoodle);

            on_stage3<s3_novel>(fsNovel);
        
                //  STAGE 4 global related

            on_stage4<css_stage4>();        // <---  Must come AFTER background stage4
            on_stage4<page_stage4>();

            on_stage4<s4_class>();
            //on_stage4<s4_class_graph>();
            on_stage4<s4_atom>();
            
            
            //on_stage4<u_leaf_stage4_cleanup>();
            
                // Images change first (for icon changes)
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_change<s5_image>(FileSpec(CACHE, z));

                // Rest are less order dependent... 

                
            on_change<css_update>(FileSpec(gSharedCssFile));
            on_change<css_update>(FileSpec(CACHE, cdb::top_folder(), ".css"));

            on_change<page_update>(FileSpec(gSharedPageFile));
            on_change<page_update>(FileSpec(CACHE, cdb::top_folder(), ".page"));
                
            on_change<s5_category>(FileSpec(CACHE, cdb::categories_folder(), "*.cat"));
            on_change<s5_class>(fsClass);
            on_change<s5_doodle>(fsDoodle);
            on_change<s5_field>(fsField);
            on_change<s5_leaf>(fsLeaf);
            on_change<s5_novel>(fsNovel);
            on_change<s5_style>(fsStyle);
            on_change<s5_tag>(fsTag);
            on_change<s5_user>(fsUser);
            
            for(const char* z : Image::kSupportedExtensionWildcards){
                on_change<s5_category_icons>(FileSpec(CACHE, categories_folder(), z));
                on_change<s5_class_icons>(FileSpec(CACHE, classes_folder(), z));
                on_change<s5_field_icons>(FileSpec(CACHE, fields_folder(), z));
                on_change<s5_leaf_icons>(FileSpec(CACHE, z));
                on_change<s5_novel_icons>(FileSpec(CACHE, z));
                on_change<s5_style_icons>(FileSpec(CACHE, styles_folder(), z));
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
