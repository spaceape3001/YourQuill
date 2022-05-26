////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WebPage.hpp"
#include <yq/file/Folder.hpp>
#include <basic/Guarded.hpp>
#include "TypedBytes.hpp"
#include <vector>

namespace yq {
    class WebImage : public WebPage {
    public:
    
        WebImage(std::string_view kpath, const std::filesystem::path&, Folder, std::string_view, const std::source_location&sl);
        virtual void    handle(WebContext&) const override;
        bool            hasImage() const { return m_loaded; }

        class Writer;

    private:
        std::filesystem::path           m_fallback;
        std::vector<std::string>        m_files;
        Guarded<Ref<TypedBytes>>        m_data;
        Folder                          m_parent;
        std::function<void(WebImage&)>  m_post;
        bool                            m_loaded = false;

        void    update();
    };
    
    class WebImage::Writer : public WebPage::Writer {
    public:
    
        Writer(WebImage*i) : WebPage::Writer(i), m_image(i) {}
        Writer&         post(std::function<void(WebImage&)>);
        
    private:
        WebImage*       m_image = nullptr;
    };
    
    WebImage::Writer    reg_webimage(std::string_view, const std::filesystem::path&, Folder, std::string_view, const std::source_location& sl=std::source_location::current());
    
}
