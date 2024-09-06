////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/basic/Http.hpp>
#include <yq-toolbox/basic/Ref.hpp>
#include <yq-toolbox/typedef/string_sets.hpp>


namespace yq::mithril {
    class WebHtml;
    struct WebContext;

    class Template : public RefCount {
    public:
        struct Token {
            std::string_view        token;
            bool                    variable;
        };

        Template(ContentType ct=ContentType());
        Template(std::string&& mv, ContentType ct=ContentType());
        Template(std::string_view k, ContentType ct=ContentType());
        
        std::string_view    data() const { return m_data; }
        
        void            execute(WebHtml&) const;
        
        ContentType         type() const { return m_type; }
        
        void        writeBitsToInfo() const;
        
        bool        empty() const { return m_data.empty(); }
        
    private:
        void    parse(std::string_view);
        friend class WebHtml;
    
        std::string                 m_data;
        std::vector<Token>          m_bits;
        string_view_set_t           m_vars;
        ContentType                 m_type;
    };
    
    namespace web {
        void    set_template(Ref<Template>);
    }


}
