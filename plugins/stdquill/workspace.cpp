////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"
#include <yq/file/FileUtils.hpp>
#include <yq/srv/NotifyAdapters.hpp>
#include <yq/srv/Stage3.hpp>
#include <yq/srv/Stage4.hpp>
#include <yq/stream/Text.hpp>
#include <yq/web/TypedBytes.hpp>

using namespace yq;

Guarded<HttpDataPtr>            gCss;
Guarded<std::string>            gTextColor, gBkColor;
std::atomic<bool>               gHasBackground{false};
Guarded<Ref<TypedBytes>>        gBackground, gIndex;
Guarded<std::string>            gFooter;
Guarded<std::string>            gSummary;
std::filesystem::path           gSharedCssFile;

namespace {
    void    var_abbr(Stream&str, WebContext&)
    {
        str << wksp::abbreviation();
    }

    void    var_author(Stream&str, WebContext&)
    {
        str << wksp::author();
    }

    void    var_home(Stream&str, WebContext&)
    {
        str << wksp::home();
    }

    void    var_host(Stream& str, WebContext&)
    {
        str << wksp::host();
    }

    void    var_name(Stream& str, WebContext&)
    {
        str << wksp::name();
    }

    void    var_port(Stream& str, WebContext&)
    {
        str << wksp::port();
    }
    
    void    var_time(Stream& str, WebContext& ctx)
    {
        str << ctx.timestamp;
    }
    
    void    update_css();
    
    void    update_background()
    {
        Document    doc = cdb::document({ ".background.jpg", ".background.gif", ".background.png", ".background.svg" });
        bool    now = false;

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
        
        bool    was = gHasBackground.exchange(now);
        if(now != was)
            update_css();
    }
    
    void    update_css()
    {
    yInfo() << "Updating CSS";
        std::string       css;
        if(wksp::can_cdb()){
            for(Fragment f : cdb::fragments("*.css", DataRole::Style))
                css += cdb::frag_string(f);
        }
        
        HttpDataPtr         newCssData = HttpData::make();
        {
            HttpDataStream      ncss(newCssData);
            
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
        yInfo() << "Updating CSS to " << newCssData->count() << " size.";
        gCss        = newCssData;
    }
    
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
        ctx.tx_content.push_back(gCss);
    }
}

YQ_INVOKE(

    unsigned int n = 0;
    for(auto& fs : wksp::shared_dirs())
        yInfo() << "share directory " << (++n) << ": "  << fs;

    if(!web::set_template(wksp::shared("std/page"sv)))
        yWarning() << "Failed to set web template!";

    reg_webpage("/img/**", wksp::shared_all("www/img"sv));
    reg_webpage("/help/*", wksp::shared_all("www/help"sv));
    reg_webpage("/js/*", wksp::shared_all("www/js"sv));
    reg_webpage<page_css>("/css");
    reg_webpage<page_background>("/background");

    
    reg_webvar<var_abbr>("abbr");
    reg_webvar<var_author>("author");
    reg_webvar<var_home>("home");
    reg_webvar<var_host>("host");
    reg_webvar<var_name>("name");
    reg_webvar<var_port>("port");
    reg_webvar<var_time>("time");

    //reg_web("img/**", wksp::shared_dir("www/img"));
    //reg_web("help/*", wksp::shared_dir("www/help"));
    //reg_web("js/*", wksp::shared_dir("www/js"));
    //reg_web("markdown/*", wksp::shared_dir("www/markdown"));
    
    gSharedCssFile      = wksp::shared("std/css"sv);
    on_stage3<update_css>(cdb::top_folder(), ".css"sv);
    on_change<update_css>(cdb::top_folder(), ".css"sv);
    on_change<update_css>(gSharedCssFile);
    on_stage4<update_background>();
    on_change<update_background>(cdb::top_folder(), ".background.jpg");
    on_change<update_background>(cdb::top_folder(), ".background.gif");
    on_change<update_background>(cdb::top_folder(), ".background.png");
    on_change<update_background>(cdb::top_folder(), ".background.svg");
)

