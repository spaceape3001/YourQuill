////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace yq {
    WebAutoClose::WebAutoClose(WebHtml&wh, std::string_view sv) : 
        m_html(&wh)
    {
        m_close = [sv](WebHtml&h) {
            h << sv;
        };
    }
    
    WebAutoClose::WebAutoClose(WebHtml& wh, CloseHandler onClose) : m_html(&wh), m_close(onClose)
    {
    }
    
    WebAutoClose::WebAutoClose(WebAutoClose&&mv) : m_html(mv.m_html), m_close(std::move(mv.m_close))
    {
        mv.m_html   = nullptr;
    }
    
    WebAutoClose& WebAutoClose::operator=(WebAutoClose&&mv)
    {
        if(&mv != this){
            close();
            std::swap(m_html, mv.m_html);
            m_close         = std::move(mv.m_close);
        }
        return *this;
    }
    
    WebAutoClose::~WebAutoClose()
    {
        close();
    }
    
    void    WebAutoClose::close()
    {
        if(m_html){
            if(m_close)
                m_close(*m_html);
            m_html  = nullptr;
        }
    }

}
