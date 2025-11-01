////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Template.hpp"
#include "WebVariable.hpp"

#include <yq/mithril/preamble.hpp>
#include <yq/mithril/folder/Folder.hpp>
#include <yq/core/Guarded.hpp>
#include <filesystem>

namespace yq::mithril {
    class VarTemplate : public WebVariable {
    public:
    
        class Writer;
        VarTemplate(std::string_view, const std::filesystem::path&, const std::source_location&);
        void    update();
        void handle(WebHtml&) const override;

    private:
        std::filesystem::path   m_master;
        std::vector<FolderStr>  m_sources;
        Guarded<Ref<Template>>  m_text;
        
    };

    class VarTemplate::Writer : public WebVariable::Writer {
    public:
    
        Writer(VarTemplate* t) : WebVariable::Writer(t), m_template(t) {}
    
        Writer&     source(std::string_view, const std::source_location& sl = std::source_location::current());
        Writer&     source(Folder, std::string_view, const std::source_location& sl = std::source_location::current());
    
    
    private:
        VarTemplate*    m_template = nullptr;
    };
    
    VarTemplate::Writer     reg_webvar(std::string_view, const std::filesystem::path&, const std::source_location& sl = std::source_location::current());
}
