////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Template.hpp"

#include <yq-toolbox/trait/not_copyable.hpp>
#include <yq-toolbox/trait/not_moveable.hpp>
#include <yq-toolbox/basic/Ref.hpp>

namespace yq::mithril {

    /*! \brief HTML Web Template
    */
    class PageTemplate : public RefCount, not_copyable, not_moveable {
    public:
   
        PageTemplate(std::string&&);
        //PageTemplate(const std::filesystem::path&);
        ~PageTemplate();
        void    execute(WebHtml&) const;
    
    private:
        std::string             m_data;
        string_view_vector_t    m_css;
        string_view_vector_t    m_scripts;
        string_view_vector_t    m_headers;  // other header things....
        Ref<Template>           m_body;
        Ref<Template>           m_title;
    };
}
