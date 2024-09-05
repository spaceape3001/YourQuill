////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PageTemplate.hpp"
#include <yq-toolbox/text/match.hpp>
#include <yq-toolbox/text/transform.hpp>
#include <yq-toolbox/text/vsplit.hpp>

namespace yq::mithril {
    PageTemplate::PageTemplate(std::string&&data) : m_data(std::move(data))
    {
        std::string_view    p;
        std::string_view    t;
        if(starts(m_data, "#!")){   // we're using the web-template form
            const char* z   = vsplit(m_data, '\n', [&](std::string_view y) -> const char* {
                if(y.empty())
                    return y.data();
                if(!starts(y, "#!"))
                    return y.data();
                if(starts_igCase(y, "#!script ")){
                    m_scripts.push_back(trimmed(y.substr(9)));
                } else if(starts_igCase(y, "#!css ")){
                    m_css.push_back(trimmed(y.substr(6)));
                } else if(starts_igCase(y, "#!! ")){
                    m_headers.push_back(trimmed(y.substr(4)));
                } else {
                    t       = trimmed(y.substr(2));
                }
                return nullptr;
            });
            if(z){
                p   = m_data;
                p   = p.substr(z - m_data.data());
            }
        } else {
            p       = m_data;
        }
    
        if(!t.empty())
            m_title   = new Template(t);
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
        for(auto& s : m_scripts)
            h.add_hscript(s);
        for(auto& s : m_css)
            h.add_hcss(s);
        if(m_title){
            auto ti = h.title();
            m_title->execute(h);
        }
        if(m_body){
            m_body -> execute(h);
        }
    }
    
}
