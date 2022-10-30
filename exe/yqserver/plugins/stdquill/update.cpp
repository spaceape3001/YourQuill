////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/CollectionUtils.hpp>
#include <basic/DelayInit.hpp>
#include <basic/DirUtils.hpp>
#include <basic/FileUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <basic/Vector.hpp>

#include <basic/stream/Bytes.hpp>
#include <basic/stream/Text.hpp>

#include <kernel/atom/ClassCDB.hpp>
#include <kernel/db/SQ.hpp>
#include <kernel/file/DocumentCDB.hpp>
#include <kernel/file/Root.hpp>
#include <kernel/image/ImageCDB.hpp>
#include <kernel/leaf/LeafCDB.hpp>
#include <kernel/leaf/LeafData.hpp>
#include <kernel/notify/FileWatch.hpp>
#include <kernel/notify/FileNotifyAdapters.hpp>
#include <kernel/notify/Stage2.hpp>
#include <kernel/notify/Stage3.hpp>
#include <kernel/notify/Stage4.hpp>
#include <kernel/org/CategoryCDB.hpp>
#include <kernel/org/TagCDB.hpp>

#include <kernel/user/UserCDB.hpp>
#include <kernel/wksp/Workspace.hpp>

#include <update/uField.hpp>

#include <update/uLeaf.hpp>

#include "uAtom.ipp"

#include "uField.ipp"
#include "uLeaf.ipp"

//#include "uUser.ipp"

#include "common.hpp"

#include <yq/atom.hpp>
#include <yq/attribute.hpp>

#include <yq/class.hpp>
#include <yq/directory.hpp>
#include <yq/document.hpp>
#include <yq/field.hpp>
#include <yq/folder.hpp>
#include <yq/fragment.hpp>

#include <yq/leaf.hpp>
#include <yq/property.hpp>
#include <yq/root.hpp>

//#include <yq/user.hpp>
#include <yq/value.hpp>

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
}

#include "u_atom.ipp"
#include "u_category.ipp"
#include "u_class.ipp"
#include "u_css.ipp"
#include "u_field.ipp"
#include "u_image.ipp"
#include "u_leaf.ipp"
#include "u_root.ipp"
#include "u_tag.ipp"
#include "u_user.ipp"

namespace {

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
            
            on_stage3<update::field_stage3>(fields_lookup);
            

                //  LEAFS & atoms
            on_stage3<update::leaf_stage3_pass1_declare>(leafs_lookup);
            on_stage3<leaf_stage3>(leafs_lookup);
            on_stage3<u_tag_stage3_leaf>(by_cache(tags_folder(), "*.tag"));

        
                //  STAGE 4 global related

            on_stage4<css_stage4>();        // <---  Must come AFTER background stage4
            on_stage4<page_stage4>();
            
            
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
            on_change<update::field_notify>(fields_lookup);
            on_change<leaf_update>(leafs_lookup);
            on_change<u_tag_notify>(tags_lookup);
            on_change<u_user_notify>(by_cache(users_folder(), "*.user"));
            
            for(const char* z : Image::kSupportedExtensionWildcards){
                on_change<u_class_notify_icons>(by_cache(classes_folder(), z));
                on_change<update::field_notify_icons>(by_cache(fields_folder(), z));
                on_change<update::leaf_notify_icons>(by_cache(z));
                on_change<u_user_notify_icons>(by_cache(users_folder(), z));

                on_change<u_category_notify_icons>(by_cache(categories_folder(), z));
                on_change<u_tag_notify_icons>(by_cache(tags_folder(), z));
            }
        }

    //  now outside the above function, we register....
        
    YQ_INVOKE( reg_me(); )
}
