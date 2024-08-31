////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <0/basic/ByteArray.hpp>
#include <yq/basic/Guarded.hpp>
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

#include <0/basic/BasicApp.hpp>
#include <yq/basic/DelayInit.hpp>
#include <0/basic/Logging.hpp>
#include <0/basic/TextUtils.hpp>
#include <0/basic/Vector.hpp>
#include <0/io/DirUtils.hpp>
#include <0/io/FileUtils.hpp>
#include <0/io/StreamOps.hpp>
#include <0/io/stream/Bytes.hpp>
#include <0/io/stream/Text.hpp>
#include <0/math/shape/Size2.hxx>
#include <0/meta/TypeInfo.hpp>

#include <mithril/atom/AtomCDB.hpp>
#include <mithril/atom/AtomDiff.hpp>
#include <mithril/attribute/AttributeDiff.hpp>
#include <mithril/category/Category.hpp>
#include <mithril/category/CategoryCDB.hpp>
#include <mithril/category/CategoryData.hpp>
#include <mithril/category/CategoryDiff.hpp>
#include <mithril/class/ClassCDB.hpp>
#include <mithril/class/ClassData.hpp>
#include <mithril/class/ClassDiff.hpp>
#include <mithril/document/DocumentCDB.hpp>
#include <mithril/doodle/DoodleCDB.hpp>
#include <mithril/field/FieldCDB.hpp>
#include <mithril/field/FieldData.hpp>
#include <mithril/field/FieldDiff.hpp>
#include <mithril/folder/FolderCDB.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/graphviz/GraphvizBuilder.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <mithril/image/ImageDiff.hpp>
#include <mithril/leaf/LeafCDB.hpp>
#include <mithril/leaf/LeafData.hpp>
#include <mithril/leaf/LeafDiff.hpp>
#include <mithril/meta/Meta.hpp>
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Notifier.hpp>
#include <mithril/notify/Stage2.hpp>
#include <mithril/notify/Stage3.hpp>
#include <mithril/notify/Stage4.hpp>
#include <mithril/novel/NovelCDB.hpp>
//#include <mithril/novel/NovelData.hpp>
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

//#include <mithril/atom/AtomUpdate.hpp>
//#include <mithril/class/ClassUpdate.hpp>
//#include <mithril/document/DocumentUpdate.hpp>
//#include <mithril/field/FieldUpdate.hpp>
//#include <mithril/leaf/LeafUpdate.hpp>

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
