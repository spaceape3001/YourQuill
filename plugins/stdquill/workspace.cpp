////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <yq/file/file_utils.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/srv/Stage4.hpp>
#include <yq/stream/Bytes.hpp>
#include <yq/stream/Text.hpp>
#include <yq/web/TypedBytes.hpp>

#include <db/filesys_html.hpp>
#include <db/workspace.hpp>

using namespace yq;
//using namespace yq::arg;
//using namespace yq::cdb;
using namespace yq::html;

struct Index : public RefCount {
    std::string             data;
    Ref<WebTemplate>        page;
    Ref<WebTemplate>        title;
    
    Index(std::string&& dat, ContentType ct) : data(std::move(dat))
    {
        std::string_view    t   = web_title(data);
        std::string_view    p;
        if(!t.empty()){
            const char* z0  = t.data()+t.size();
            const char* z1  = data.data() + data.size();
            p       = std::string_view(z0, z1);
            t       = trimmed(t);
            title   = new WebTemplate(t);
        } else {
            p   = data;
        }
        
        page            = new WebTemplate(p, ct);
    }
};

Guarded<std::shared_ptr<ByteArray>>     gCss;
Guarded<std::string>            gTextColor, gBkColor;
std::atomic<bool>               gHasBackground{false};
Guarded<Ref<TypedBytes>>        gBackground;
Guarded<Ref<WebTemplate>>       gFooter, gSummary;
Guarded<Ref<Index>>             gIndex;
std::filesystem::path           gSharedCssFile, gSharedFooterFile, gSharedIndexFile, gSharedPageFile, gSharedSummaryFile;

namespace {
    static const std::initializer_list<std::string_view>    kIndexFiles = { ".index", ".index.md", ".index.html", ".index.htm" };
    static const std::initializer_list<std::string_view>    kBackgroundFiles = { ".background.jpg", ".background.gif", ".background.png", ".background.svg" };
    static const std::string_view       kStdCSS         = "std/css";
    static const std::string_view       kStdPage        = "std/page";
    static const std::string_view       kStdIndex       = "std/index";
    static const std::string_view       kStdFooter      = "std/footer";
    static const std::string_view       kStdSummary     = "std/summary";
}

namespace {

    
    
    
    void    var_footer(WebHtml&h)
    {
        Ref<WebTemplate>    footer  = gFooter;
        if(footer)
            footer -> execute(h);
    }
    





    
    
    void    var_summary(WebHtml&h)
    {
        Ref<WebTemplate>    summary  = gSummary;
        if(summary)
            summary -> execute(h);
    }
    

    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void    update_css();
    
    void    update_background()
    {
        Document    doc = cdb::first_document(kBackgroundFiles);
        bool    now = false;

        if(doc){
            do {
                Fragment    frag = cdb::fragment(doc, DataRole::Image);
                if(!frag)
                    break;
                Ref<TypedBytes> tb  = TypedBytes::load(cdb::path(frag));
                if(!tb)
                    break;
                gBackground = tb;
                now = true;
                
            } while(false);
        }
        
        bool    was = gHasBackground.exchange(now);
        if(now != was)
            update_css();
    }
    
    void    update_css()
    {
        std::string       css;
        for(Fragment f : cdb::fragments("*.css", DataRole::Style))
            css += cdb::frag_string(f);
        
        ByteArray           newCssData;
        {
            stream::Bytes       ncss(newCssData);
            
            std::string     newTxtColor = "black";
            std::string     newBkColor  = "white";

            Map<std::string, std::string, IgCase> vars;
            bool    inBody  = false;
            for(std::string_view l : split(css, '\n')){
                if(l.empty())
                    continue;
                    
                if(inBody){
                    if(l[0] == '}'){
                        inBody  = false;
                        continue;
                    }
                    size_t i  = l.find_first_of(':');
                    if(i == std::string_view::npos)
                        continue;
                    std::string  k0  = simplify(l.substr(0,i));
                    std::string  k   = to_lower(k0);
                    std::string  v   = std::string(trimmed(l.substr(i+1)));
                    if(is_similar(k, "color"))
                        newTxtColor      = v;
                    if(is_similar(k, "background-color"))
                        newBkColor      = v;
                    vars[k]     = v;
                } else if(starts_igCase(l, "body")) {
                    inBody  = true;
                }
            }
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
                        "    background-image: url(\"/background\");"
                        "}\n";
            }
            
            ncss << file_string(gSharedCssFile);
        }
        gCss        = std::make_shared<ByteArray>(std::move(newCssData));
    }
    
    void    update_footer()
    {
        std::string     r;
        Fragment    f   = cdb::first_fragment(cdb::document(".footer"));
        if(f)
            r   = cdb::frag_string(f);
        if(r.empty())
            r       = file_string(gSharedFooterFile);
        gFooter = new WebTemplate(std::move(r));
    }
    
    void    update_page()
    {
        if(!web::set_template(gSharedPageFile))
            yWarning() << "Failed to set web template!";
    }
    
    void    update_index()
    {
        std::string     r;
        Document    d   = cdb::first_document(kIndexFiles);
        ContentType ct;
        if(d){
            std::string ext = cdb::suffix(d);
            r       = cdb::frag_string(cdb::first_fragment(d));
            ct      = mimeTypeForExt(ext);
        }
        if(ct == ContentType())
            ct  = ContentType::markdown;
        if(r.empty())
            r       = file_string(gSharedIndexFile);
        gIndex      = new Index(std::move(r), ct);
    }
    
    void    update_summary()
    {
        std::string     r;
        Fragment    f   = cdb::first_fragment(cdb::document(".summary"));
        if(f)
            r   = cdb::frag_string(f);
        if(r.empty())
            r       = file_string(gSharedSummaryFile);
        gSummary = new WebTemplate(std::move(r));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    void    page_background(WebContext& ctx)
    {
        Ref<TypedBytes>     data = gBackground;
        if(!data){
            ctx.status  = HttpStatus::ServiceUnavailable;
            return;
        }
        
        ctx.tx_content_type = data -> type;
        ctx.tx_content      = data -> data;
        ctx.tx_header("Date", data->modified);
        ctx.status          = HttpStatus::Success;
    }
    
    void    page_css(WebContext& ctx)
    {
        ctx.tx_content_type = ContentType::css;
        ctx.tx_content = gCss;
    }
    
    
    void    page_index(WebHtml& out)
    {
        Ref<Index>  index   = gIndex;
        if(!index)
            return ;
        
        if(index->title){
            auto ti = out.title();
        
            index->title->execute(out);
        
        
            //std::string&    title   = out.context().var_title;
            //title.reserve(256);
            //stream::Text    text(title);
            //index->title->execute(text, out.context());
        }
        
        if(index->page)
            index->page -> execute(out);
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void    reg_me()
    {
        unsigned int n = 0;
        for(auto& fs : wksp::shared_dirs())
            yInfo() << "share directory " << (++n) << ": "  << fs;



        

        
        
        
        
        reg_webpage("/favicon.ico", wksp::shared("www/img/yquill.svg"sv));
        
        //  for now....
        reg_webpage("/logo", wksp::shared("www/img/yquill.svg"sv));


        reg_webpage<page_css>("/css");
        reg_webpage<page_background>("/background");
        reg_webpage<page_index>("/");


        
        
        reg_webvar<var_footer>("footer");
        
        
        
        
        
        reg_webvar<var_summary>("summary");
        
        
        
        

        //reg_web("img/**", wksp::shared_dir("www/img"));
        //reg_web("help/*", wksp::shared_dir("www/help"));
        //reg_web("js/*", wksp::shared_dir("www/js"));
        //reg_web("markdown/*", wksp::shared_dir("www/markdown"));

            // PAGE TEMPLATE
        gSharedPageFile     = wksp::shared(kStdPage);
        on_stage4<update_page>();
        on_change<update_page>(gSharedPageFile);
        
            // CSS
        gSharedCssFile      = wksp::shared(kStdCSS);
        on_stage4<update_css>();
        on_change<update_css>(cdb::top_folder(), ".css"sv);
        on_change<update_css>(gSharedCssFile);
        
            // BACKGROUND
        on_stage4<update_background>();
        for(std::string_view k : kBackgroundFiles)
            on_change<update_background>(cdb::top_folder(), k);

            // FOOTER
        gSharedFooterFile   = wksp::shared(kStdFooter);
        on_stage4<update_footer>();
        on_change<update_footer>(cdb::top_folder(), ".footer");
        on_change<update_footer>(gSharedFooterFile);

            // MAIN INDEX
        gSharedIndexFile    = wksp::shared(kStdIndex);
        on_stage4<update_index>();
        for(std::string_view k : kIndexFiles)
            on_change<update_index>(cdb::top_folder(), k);
        on_change<update_index>(gSharedIndexFile);
        
            // SUMMARY
        gSharedSummaryFile  = wksp::shared(kStdSummary);
        on_stage4<update_summary>();
        on_change<update_summary>(cdb::top_folder(), ".summary");
        on_change<update_summary>(gSharedSummaryFile);
    }
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

YQ_INVOKE( reg_me(); )

