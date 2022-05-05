////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PageTemplate.hpp"
#include "WebPage.hpp"


namespace yq {
    class WebHtml;
    struct WebContext;

    class WebTemplate : public WebPage {
    public:
    
        class Writer;
        
        void        update();
        
    
    private:
        std::filesystem::path       m_master;
        std::vector<std::string>    m_fragments; // for overrides
        Guarded<Ref<PageTemplate>>  m_body;
        
    };
    
    
    class WebTemplate::Writer : public WebPage::Writer {
    public:
    
        Writer(WebTemplate* t) : WebPage::Writer(t), m_template(t) {}
    
        Writer&     source(const std::filesystem::path&);
        Writer&     source(std::string_view);
        Writer&     source(Folder, std::string_view);
    
    
    private:
        WebTemplate*    m_template = nullptr;
    };
    
    WebTemplate::Writer     reg_webtemplate(std::string_view, const std::source_location& sl = std::source_location::current());

}
