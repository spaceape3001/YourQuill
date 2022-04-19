////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/net/Http.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    class WebHtml;
    struct WebContext;

    class WebTemplate : public RefCount {
    public:
        struct Token {
            std::string_view        token;
            bool                    variable;
        };

        WebTemplate(ContentType ct=ContentType());
        WebTemplate(std::string&& mv, ContentType ct=ContentType());
        WebTemplate(std::string_view k, ContentType ct=ContentType());
        
        std::string_view    data() const { return m_data; }
        
        void            execute(WebHtml&) const;
        
        ContentType         type() const { return m_type; }
        
        void        writeBitsToInfo() const;
        
    private:
        void    parse(std::string_view);
        friend class WebHtml;
    
        std::string                 m_data;
        Vector<Token>               m_bits;
        string_view_set_t           m_vars;
        ContentType                 m_type;
    };
    
}
