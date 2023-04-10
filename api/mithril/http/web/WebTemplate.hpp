////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PageTemplate.hpp"
#include "WebPage.hpp"

#include <mithril/kernel/preamble.hpp>
#include <mithril/kernel/file/Folder.hpp>
#include <basic/Guarded.hpp>

namespace yq::mithril {
    class WebHtml;
    struct WebContext;

    /*! \brief Template from file(s)
    
    */
    class WebTemplate : public WebPage {
    public:
    
        class Writer;

        WebTemplate(std::string_view, const std::filesystem::path&, const std::source_location&);
        void        update();
        void        handle(WebContext&) const override;
    
    private:
        
        std::filesystem::path       m_master;
        std::vector<FolderStr>      m_sources;
        Guarded<Ref<PageTemplate>>  m_body;
    };
    
    
    class WebTemplate::Writer : public WebPage::Writer {
    public:
    
        Writer(WebTemplate* t) : WebPage::Writer(t), m_template(t) {}
    
        Writer&     source(std::string_view, const std::source_location& sl = std::source_location::current());
        Writer&     source(Folder, std::string_view, const std::source_location& sl = std::source_location::current());
    
    
    private:
        WebTemplate*    m_template = nullptr;
    };
    
    WebTemplate::Writer     reg_webtemplate(std::string_view, const std::filesystem::path&, const std::source_location& sl = std::source_location::current());

}
