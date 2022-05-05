////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PageTemplate.hpp"

namespace yq {
    PageTemplate::PageTemplate(std::string&&data) : m_data(std::move(data))
    {
        std::string_view    t   = web_title(m_data);
        std::string_view    p;
        if(!t.empty()){
            const char* z0  = t.data()+t.size();
            const char* z1  = m_data.data() + m_data.size();
            p       = std::string_view(z0, z1);
            t       = trimmed(t);
            m_title   = new Template(t);
        } else {
            p   = m_data;
        }
        
        m_body          = new Template(p);
    }
    
    //PageTemplate::PageTemplate(const std::filesystem::path& pth) : PageTemplate( file_string(pth))
    //{
    //}
    
    PageTemplate::~PageTemplate()
    {
    }
    
    void    PageTemplate::execute(WebHtml&h) const
    {
        if(m_title){
            auto ti = h.title();
            m_title->execute(h);
        }
        if(m_body)
            m_body -> execute(h);
    }
    
}
