////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WebTemplate.hpp"

#include <yq/c++/trait/not_copyable.hpp>
#include <yq/c++/trait/not_moveable.hpp>
#include <yq/type/Ref.hpp>

namespace yq {

    /*! \brief HTML Web Template
    */
    class PageTemplate : public RefCount, trait::not_copyable, trait::not_moveable {
    public:
   
        PageTemplate(std::string&&);
        //PageTemplate(const std::filesystem::path&);
        ~PageTemplate();
        void    execute(WebHtml&) const;
    
    private:
        std::string             m_data;
        Ref<WebTemplate>        m_page;
        Ref<WebTemplate>        m_title;
    };
}
