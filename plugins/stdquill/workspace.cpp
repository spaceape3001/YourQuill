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

Guarded<HttpDataPtr>            gCss;
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
    void    var_abbr(Stream&str, WebContext&)
    {
        html_escape_write(str, wksp::abbreviation());
    }

    void    var_author(Stream&str, WebContext&)
    {
        html_escape_write(str, wksp::author());
    }
    
    void    var_body(Stream& str, WebContext&ctx)
    {
        str << ctx.var_body;
    }
    
    
    void    var_footer(Stream&str, WebContext&ctx)
    {
        Ref<WebTemplate>    footer  = gFooter;
        if(footer)
            footer -> execute(str, ctx);
    }
    

    void    var_home(Stream&str, WebContext&)
    {
        html_escape_write(str, wksp::home());
    }

    void    var_host(Stream& str, WebContext&)
    {
        html_escape_write(str, wksp::host());
    }


    void    var_name(Stream& str, WebContext&)
    {
        html_escape_write(str, wksp::name());
    }

    void    var_port(Stream& str, WebContext&)
    {
        str << wksp::port();
    }
    
    void    add_script(Stream& str, std::string_view name)
    {
        switch(mimeTypeForExt(file_extension(name))){
        case ContentType::javascript:
            str << "\t\t<script srce=\"/js/" << name << "\">\n";
            break;
        default:
            //  do nothing if unclear...
            break;
        }
    }
    
    void    var_scripts(Stream& str, WebContext& ctx)
    {
        for(std::string_view s : ctx.page->scripts())
            add_script(str, s);
        for(const std::string& s : ctx.var_scripts)
            add_script(str, s);
    }
    
    void    var_summary(Stream&str, WebContext&ctx)
    {
        Ref<WebTemplate>    summary  = gSummary;
        if(summary)
            summary -> execute(str, ctx);
    }
    
    void    var_tabbar(Stream& str, WebContext& ctx)
    {
        const WebGroup* grp = ctx.page -> group();
        if(!grp)
            return ;
    
        str << "<table class=\"tabbar\"><tr>\n";
        for(const WebPage* p : grp -> pages){
            bool    us = p == ctx.page;
            if(us) {
                str << "<td class=\"tb-select\">";
                html_escape_write(str, p -> label());
                str << "</td>";
            } else {
                UrlView url = ctx.url;
                url.path    = p->path();
                str << "<td class=\"tabbar\"><a href=\"" << url << "\">";
                html_escape_write(str, p -> label());
                str << "</a></td>";
            }
        }
        str << "</td></table>\n";
    }

    void    var_time(Stream& str, WebContext& ctx)
    {
        str << ctx.timestamp;
    }
    
    void    var_title(Stream& str, WebContext&ctx)
    {
        html_escape_write(str, ctx.var_title);
    }
    
    void    var_year(Stream& str, WebContext& ctx)
    {
        str << (ctx.timeparts.tm_year+1900);
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
        gCss        = newCssData;
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

    void    dev_wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::roots());
    }

    void  dev_wksp_info(WebHtml& h)
    {
        h.title("Workspace '"s + copy(wksp::name()) + "'"s);
        
        auto t = html::table(h);
        html::kvrow(h, "Name") << wksp::name();
        html::kvrow(h, "Author") << wksp::author();
        html::kvrow(h, "Abbreviation") << wksp::abbreviation();
        html::kvrow(h, "Cache") << wksp::cache();
        //html::kvrow(h, "Config") << dev(cdb::config_folder());
        html::kvrow(h, "Copyright") << wksp::copyright().text;
        html::kvrow(h, "Key") << wksp::quill_key();
        html::kvrow(h, "Host") << wksp::host();
        html::kvrow(h, "Port") << wksp::port();
        html::kvrow(h, "Read Timeout") << wksp::read_timeout();
        html::kvrow(h, "Start") << wksp::start();
        html::kvrow(h, "Templates") << join(wksp::templates(), ", ");
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
    
    
    void    page_index(WebHtml& out)
    {
        Ref<Index>  index   = gIndex;
        if(!index)
            return ;
        
        if(index->title){
            std::string&    title   = out.context().var_title;
            title.reserve(256);
            stream::Text    text(title);
            index->title->execute(text, out.context());
        }
        
        if(index->page)
            index->page -> execute(out, out.context());
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void    reg_me()
    {
        unsigned int n = 0;
        for(auto& fs : wksp::shared_dirs())
            yInfo() << "share directory " << (++n) << ": "  << fs;

        reg_webpage("/img/**", wksp::shared_all("www/img"sv));
        reg_webpage("/help/**", wksp::shared_all("www/help"sv));
        reg_webpage("/js/**", wksp::shared_all("www/js"sv));

        reg_webgroup({
            reg_webpage<dev_wksp_info>("/dev/wksp/info").label("Info"),
            reg_webpage<dev_wksp_roots>("/dev/wksp/roots").label("Roots")
        });

        reg_webpage<page_css>("/css");
        reg_webpage<page_background>("/background");
        reg_webpage<page_index>("/");
        
        reg_webvar<var_abbr>("abbr");
        reg_webvar<var_author>("author");
        reg_webvar<var_body>("body");
        reg_webvar<var_footer>("footer");
        reg_webvar<var_home>("home");
        reg_webvar<var_host>("host");
        reg_webvar<var_name>("name");
        reg_webvar<var_port>("port");
        reg_webvar<var_scripts>("scripts");
        reg_webvar<var_summary>("summary");
        reg_webvar<var_tabbar>("tabbar");
        reg_webvar<var_time>("time");
        reg_webvar<var_title>("title");
        reg_webvar<var_year>("year");

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

