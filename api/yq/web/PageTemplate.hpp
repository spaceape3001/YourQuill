////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Template.hpp"

#include <basic/trait/not_copyable.hpp>
#include <basic/trait/not_moveable.hpp>
#include <basic/Ref.hpp>

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
        std::string     m_data;
        Ref<Template>   m_body;
        Ref<Template>   m_title;
    };
}
