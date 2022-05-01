////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "common.hpp"
#include "imaging.hpp"
#include <db/db.hpp>

#include <yq/app/DelayInit.hpp>
#include <yq/file/dir_utils.hpp>
#include <yq/file/file_utils.hpp>
#include <yq/log/Logging.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage2.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/srv/Stage4.hpp>
#include <yq/stream/Bytes.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/text_utils.hpp>


using namespace yq;
using namespace yq::cdb;

Guarded<std::string>            gTextColor, gBkColor;
Guarded<Ref<PageTemplate>>      gIndex;
Guarded<Ref<WebTemplate>>       gFooter, gSummary;
Guarded<Ref<TypedBytes>>        gBackground;
std::atomic<bool>               gHasBackground{false};
Guarded<SharedByteArray>        gCss;
std::vector<std::string>        gBackgroundFiles;
std::filesystem::path           gSharedCssFile, gSharedFooterFile, gSharedIndexFile, gSharedPageFile, gSharedSummaryFile;

namespace {
    static constexpr const char*    kPage       = ".page";
    static constexpr const char*    kBackground = ".background";

    static const std::string_view       kStdCSS         = "std/css";
    static const std::string_view       kStdPage        = "std/page";
    static const std::string_view       kStdIndex       = "std/index";
    static const std::string_view       kStdFooter      = "std/footer";
    static const std::string_view       kStdSummary     = "std/summary";

    static const std::initializer_list<std::string_view>    kIndexFiles = { ".index", ".index.html", ".index.htm" };

    void        css__(cdb_options_t opts=0);

    //  ================================================================================================================
    //      BACKGROUND
    //  ================================================================================================================

        bool        background__(cdb_options_t opts=0)
        {
            Document    doc = cdb::first_document(gBackgroundFiles);
            bool    now = false;

            if(doc){
                do {
                    Fragment    frag = cdb::fragment(doc, DataRole::Image);
                    if(!frag)
                        break;
                    Fragment::Lock  lk;
                    if(!(opts & DONT_LOCK))
                        lk      = Fragment::Lock::read(frag);
                    Ref<TypedBytes> tb  = TypedBytes::load(cdb::path(frag));
                    if(!tb)
                        break;
                    gBackground = tb;
                    now = true;
                } while(false);
            }

            return gHasBackground.exchange(now) != now;
        }
        
        void        background_stage4()
        {
            background__(DONT_LOCK);
        }

        void        background_update()
        {
            if(background__())
                css__();
        }

    //  ================================================================================================================
    //      CATEGORY
    //  ================================================================================================================
    
        void    category_icons(Fragment frag, Change)
        {
            update_icon(category(document(frag), true));
        }
        
        void    category_stage3(Document doc)
        {
            Category    x       = db_category(doc);
            Category::SharedData data = update(x, DONT_LOCK|U_INFO|U_ICON|IS_UPDATE);
            if(!data){
                yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
                return ;
            }
        }
        
        void    category_update(Fragment frag, Change chg)
        {
            Document    doc = document(frag);
            if(chg == Change::Removed){
                if(fragments_count(doc) <= 1){
                    erase(category(doc));
                    return ;
                }
            }
            
            bool created = false;
            Category         x   = db_category(doc, &created);
            if(!x)
                return ;

            cdb_options_t       opts    = U_INFO | IS_UPDATE;
            if(created)
                opts |= U_ICON;

            Category::SharedData data = update(x, opts);
            if(!data){
                yWarning() << "Unable to update category '" << key(x) << "' due to lack of data";
                return ;
            }
        }
        

    //  ================================================================================================================
    //      CLASSES (ATOMS)
    //  ================================================================================================================
    
        void    class_icons(Fragment frag)
        {
            update_icon(class_(document(frag), true));
        }

        void    class_s3_create(Document doc)
        {
            db_class(doc);
        }

        void    class_s3_init(Document doc)
        {
            Class x = db_class(doc);

            Class::SharedData       data    = update(x, DONT_LOCK|IS_UPDATE|U_INFO|U_ICON|U_TAGS);
            if(!data)
                yWarning() << "Unable to update class '" << key(x) << "'";
        }
        
        void    class_update(Fragment frag, Change chg)
        {
            Document    doc = document(frag);
            if(chg == Change::Removed){
                if(fragments_count(doc) <= 1){
                    erase(class_(doc));
                    return ;
                }
            }

            bool        created = false;
            Class       x   = db_class(doc, &created);
            if(!x)
                return ;
                
            cdb_options_t       opts    = U_INFO|U_TAGS|IS_UPDATE;
            if(created)
                opts |= U_ICON;
                
            Class::SharedData   data    = update(x, opts);
            if(!data){
                yWarning() << "Unable to update class '" << key(x) << "'";
                return ;
            }
        }
        

    //  ================================================================================================================
    //      CSS
    //  ================================================================================================================

        void    css__(cdb_options_t opts)
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

    //  ================================================================================================================
    //      FIELDS (ATOMS)
    //  ================================================================================================================

        void    field_stage3(Document doc)
        {
        }
        
        void    field_icons(Fragment frag)
        {
            update_icon(field(document(frag), true));
        }

    //  ================================================================================================================
    //      FOOTER (PAGE)
    //  ================================================================================================================

        void    footer__(cdb_options_t opts=0)
        {
            std::string     r;
            Fragment    f   = cdb::first_fragment(cdb::document(".footer"));
            if(f)
                r   = cdb::frag_string(f, DONT_LOCK);
            if(r.empty())
                r       = file_string(gSharedFooterFile);
            gFooter = new WebTemplate(std::move(r));
        }
        
        void    footer_stage4()
        {
            footer__(DONT_LOCK);
        }
        
        void    footer_update()
        {
            footer__();
        }

    //  ================================================================================================================
    //      IMAGES
    //  ================================================================================================================

        void    image_stage3(Document doc)
        {
            for(Fragment frag : fragments(doc))
                update_image(db_image(frag), DONT_LOCK|IS_UPDATE);
        }

        void    image_update(Fragment frag, Change chg)
        {
            switch(chg){
            case Change::Added:
            case Change::Modified:
                update_image(db_image(frag));
                break;
            case Change::Removed:
                erase(image(frag));
                break;
            default:
                break;
            }
        }
    
    //  ================================================================================================================
    //      INDEX (PAGE)
    //  ================================================================================================================
        
        void    index__(cdb_options_t opts=0)
        {
            std::string r;
            Document    d   = cdb::first_document(kIndexFiles);
            if(d)
                r   = cdb::frag_string(cdb::first_fragment(d), opts);
            if(r.empty())
                r       = file_string(gSharedIndexFile);
            gIndex  = new PageTemplate(std::move(r));
        }

        void    index_stage4()
        {
            index__(DONT_LOCK);
        }
        
        void    index_update()
        {
            index__();
        }

    //  ================================================================================================================
    //      LEAFS
    //  ================================================================================================================

        void    leaf_icons(Fragment frag)
        {
            if(hidden(folder(frag)))
                return;
            update_icon(leaf(document(frag), true));
        }

        void    leaf_stage3(Document doc)
        {
            if(hidden(folder(doc)))
                return;
            Leaf            x   = db_leaf(doc);
            Leaf::SharedData data   = update(x, DONT_LOCK|U_INFO|U_ICON|U_TAGS);
            if(!data){
                yWarning() << "Unable to initialize leaf '" << key(x) << "'";
                return ;
            }

            auto rep        = diff::additions(doc, data->attrs);
            
            
            rep.exec_inserts();

            // do the rest...later
        }
        
        void    leaf_update(Fragment frag, Change chg)
        {
            if(hidden(folder(frag)))
                return;

            Document    doc = document(frag);
            if(chg == Change::Removed){
                if(fragments_count(doc) <= 1){
                    erase(leaf(doc));
                    return ;
                }
            }
            
            bool    created = false;
            Leaf     x   = db_leaf(doc, &created);
            if(!x)
                return ;
            
            cdb_options_t   opts    = IS_UPDATE|U_INFO|U_TAGS;
            if(created)
                opts |= U_ICON;
            

            Leaf::SharedData data = update(x, opts);
            if(!data){
                yWarning() << "Unable to update leaf '" << key(x) << "'";
                return ;
            }
            
            if(created){
            } else {
                //  MORE TODO 
                auto rep     = diff::changes(doc, data->attrs);
                rep.exec_inserts();
                rep.exec_reindex();
                rep.exec_removals();
            }
            
        }


    //  ================================================================================================================
    //      PAGE
    //  ================================================================================================================


        void    page__(cdb_options_t opts=0)
        {
            Ref<WebTemplate>        wTemplate;
            std::string     r;
            
            for(Fragment f : fragments(document(kPage))){
                r       = frag_string(f, opts);
                if(r.empty())
                    continue;
            }
            
            if(r.empty())
                r   = file_string(gSharedPageFile);
            
            wTemplate   = new WebTemplate(std::move(r));
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


    //  ================================================================================================================
    //      SUMMARY
    //  ================================================================================================================

        void    summary__(cdb_options_t opts=0)
        {
            std::string     r;
            Fragment    f   = cdb::first_fragment(cdb::document(".summary"));
            if(f)
                r   = cdb::frag_string(f);
            if(r.empty())
                r       = file_string(gSharedSummaryFile);
            gSummary = new WebTemplate(std::move(r));
        }
        
        void    summary_stage4()
        {
            summary__(DONT_LOCK);
        }
        
        void    summary_update()
        {
            summary__();
        }

    //  ================================================================================================================
    //      TAGS
    //  ================================================================================================================
        
        void    tag_stage3(Document doc)
        {
            Tag     x       = db_tag(doc);
            Tag::SharedData data = update(x, DONT_LOCK|U_INFO|U_ICON);
            if(!data){
                yWarning() << "Unable to initialize tag '" << key(x) << "'";
                return ;
            }
        }
        
        void    tag_stage3_leaf(Document doc)
        {
            Tag     x       = db_tag(doc);
            Tag::SharedData data = merged(x, DONT_LOCK|U_LEAF);
            if(!data){
                yWarning() << "Unable to set tag leaf '" << key(x) << "'";
                return ;
            }
        }

        void    tag_icons(Fragment frag)
        {
            update_icon(tag(document(frag), true));
        }
        
        void    tag_update(Fragment frag, Change chg)
        {
            Document    doc = document(frag);
            if(chg == Change::Removed){
                if(fragments_count(doc) <= 1){
                    erase(tag(doc));
                    return ;
                }
            }
            
            bool    created = false;
            Tag     x   = db_tag(doc, &created);
            if(!x)
                return ;
            
            cdb_options_t   opts    = IS_UPDATE|U_INFO|U_LEAF;
            if(created)
                opts |= U_ICON;
            

            Tag::SharedData data = update(x, opts);
            if(!data){
                yWarning() << "Unable to update tag '" << key(x) << "'";
                return ;
            }
        }
        
    
    //  ================================================================================================================
    //      USERS
    //  ================================================================================================================

        void    user_icons(Fragment frag)
        {
            update_icon(user(document(frag), true));
        }

        void    user_stage3(Document doc)
        {
            User    x   = db_user(doc);
            User::SharedData data = update(x, DONT_LOCK|U_INFO|U_ICON);
            if(!data){
                yWarning() << "Unable to initialize user '" << key(x) << "'";
                return ;
            }
        }
        
        void    user_update(Fragment frag, Change chg)
        {
            Document    doc = document(frag);
            if(chg == Change::Removed){
                if(fragments_count(doc) <= 1){
                    erase(user(doc));
                    return ;
                }
            }

            bool    created = false;
            User    x   = db_user(doc, &created);
            if(!x)
                return ;
                
            cdb_options_t   opts = IS_UPDATE|U_INFO;
            if(created)
                opts |= U_ICON;

            User::SharedData data = update(x, opts);
            if(!data){
                yWarning() << "Unable to update user '" << key(x) << "'";
                return ;
            }
        }


    //  ================================================================================================================
    //      VALUES
    //  ================================================================================================================




    //  ================================================================================================================
    //      REGISTRATION
    //  ================================================================================================================

        void reg_me()
        {
        
            //  -----------------------------------------------
            //  WARNING... the following can be order dependent
            //  -----------------------------------------------
        
                // FIX any necessary paths
                
            for(const char* z : Image::kSupportedExtensions)
                gBackgroundFiles.push_back(".background."s + z);
            gSharedCssFile      = wksp::shared(kStdCSS);;
            gSharedFooterFile   = wksp::shared(kStdFooter);
            gSharedIndexFile    = wksp::shared(kStdIndex);
            gSharedPageFile     = wksp::shared(kStdPage);
            gSharedSummaryFile  = wksp::shared(kStdSummary);;

                //  I wanna get this separate, but for now....
            on_stage2<init_magicka>();
            
                // needed first for icon detection
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_stage3<image_stage3>(z);
            
                //  Organization & users
            on_stage3<category_stage3>(categories_folder(), "*.cat");
            on_stage3<tag_stage3>(tags_folder(), "*.tag");
            on_stage3<user_stage3>(users_folder(), "*.user");
            
                //  Classes & fields
            on_stage3<class_s3_create>(classes_folder(), "*.class");
            on_stage3<class_s3_init>(classes_folder(), "*.class");


                //  LEAFS & atoms
            on_stage3<leaf_stage3>("*.y");
            on_stage3<tag_stage3_leaf>(tags_folder(), "*.tag");

        
                //  STAGE 4 global related

            on_stage4<background_stage4>();
            on_stage4<css_stage4>();        // <---  Must come AFTER background stage4
            on_stage4<footer_stage4>();
            on_stage4<index_stage4>();
            on_stage4<page_stage4>();
            on_stage4<summary_stage4>();
            
            
                // Images change first (for icon changes)
            for(const char* z : Image::kSupportedExtensionWildcards)
                on_change<image_update>(z);

                // Rest are less order dependent... 

                
            on_change<css_update>(gSharedCssFile);
            on_change<css_update>(top_folder(), ".css");

            on_change<footer_update>(gSharedFooterFile);
            on_change<footer_update>(top_folder(), ".footer");

            on_change<index_update>(gSharedIndexFile);
            for(std::string_view k : kIndexFiles)
                on_change<index_update>(top_folder(), k);
                
            on_change<page_update>(gSharedPageFile);
            on_change<page_update>(top_folder(), ".page");
                
            on_change<summary_update>(gSharedSummaryFile);
            on_change<summary_update>(top_folder(), ".summary");

            on_change<category_update>(categories_folder(), "*.cat");
            on_change<class_update>(classes_folder(), "*.class");
            on_change<leaf_update>("*.y");
            on_change<tag_update>(tags_folder(), "*.tag");
            on_change<user_update>(users_folder(), "*.user");
            
            for(const char* z : Image::kSupportedExtensionWildcards){
                on_change<category_icons>(categories_folder(), z);
                on_change<class_icons>(classes_folder(), z);
                on_change<field_icons>(fields_folder(), z);
                on_change<leaf_icons>(z);
                on_change<user_icons>(users_folder(), z);
                on_change<tag_icons>(classes_folder(), z);
            }

            for(std::string_view f : gBackgroundFiles)
                on_change<background_update>(top_folder(), f);
        }

    //  now outside the above function, we register....
        
    YQ_INVOKE( reg_me(); )
}
        

//#include "update/s3_field.ipp"
//#include "update/u_field.ipp"


