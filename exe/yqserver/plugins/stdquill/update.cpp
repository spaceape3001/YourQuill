////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "uLeaf.ipp"


#include "uAtom.ipp"
#include "uCategory.ipp"
#include "uClass.ipp"
#include "uField.ipp"
#include "uImage.ipp"
#include "uRoot.ipp"
#include "uTag.ipp"
#include "uUser.ipp"

//#include <basic/BasicApp.hpp>
//#include <basic/CollectionUtils.hpp>
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
#include <mithril/notify/FileWatch.hpp>
#include <mithril/notify/FileNotifyAdapters.hpp>
#include <mithril/notify/Stage2.hpp>
#include <mithril/notify/Stage3.hpp>
#include <mithril/notify/Stage4.hpp>
//#include <mithril/category/CategoryCDB.hpp>
//#include <mithril/tag/TagCDB.hpp>
//#include <mithril/user/User.hpp>
//#include <mithril/user/UserCDB.hpp>
//#include <mithril/wksp/CacheQuery.hpp>
#include <mithril/wksp/Workspace.hpp>

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
//using namespace yq::mithril::cdb;

struct UClass;
struct UField;

static constexpr const size_t       kMinAlloc   = 1024uz;

//  configured at stage 3... only

#if 0
struct UField {
    static UField&  get(Field f)
    {
        static std::vector<UField>      s_lookup(kMinAlloc);
        if(s_lookup.size() <= f.id)
            s_lookup.resize(f.id+kMinAlloc);
        return s_lookup[f.id];
    }
};

struct UClass {
    static UClass&  get(Class c)
    {
        static std::vector<UClass>      s_lookup(kMinAlloc);
        if(s_lookup.size() <= c.id)
            s_lookup.resize(c.id+kMinAlloc);
        return s_lookup[c.id];
    }
};
#endif

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
    //  FORWARD DECLARATIONS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    using ClsHopsMap   = std::map<Class,uint64_t>;



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
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void reg_me()
        {
            using namespace yq::mithril::cdb;
        
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
