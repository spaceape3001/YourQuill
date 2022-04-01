////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/type/Ref.hpp>

namespace yq {
    class WebHtml;

    class WebTemplate : public RefCount {
    public:
        struct Token {
            std::string_view        token;
            bool                    variable;
        };

        WebTemplate();
        WebTemplate(std::string&& mv);
        WebTemplate(std::string_view k);
        
        std::string_view    data() const { return m_data; }
        
    private:
        void    parse(std::string_view);
        friend class WebHtml;
    
        std::string                 m_data;
        Vector<Token>               m_bits;
        string_view_set_t           m_vars;
    };
    
}
