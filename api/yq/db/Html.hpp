////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/Submit.hpp>
#include <yq/enum/Reset.hpp>

namespace yq {
    class WebHtml;
    
    namespace html {
        template <typename T>
        struct Dev {
            T       data;
        };

        template <typename T>
        Dev<T> dev(T d) 
        { 
            return Dev<T>{ d }; 
        }


        template <typename T>
        struct DevID {
            T       data;
        };

        template <typename T>
        DevID<T> dev_id(T d) 
        { 
            return DevID<T>{ d }; 
        }
        
        template <typename T>
        struct Plural {
            T       data;
        };
        
        template <typename T>
        Plural<T> plural(T d)
        {
            return Plural<T>{ d };
        }
        
        struct MarkdownText {
            std::string_view    data;
        };
        
        inline MarkdownText    markdown(std::string_view k) 
        { 
            return MarkdownText{k}; 
        }

        WebHtml&    operator<<(WebHtml&, const MarkdownText&);

        WebHtml&    operator<<(WebHtml&, Reset);    
        WebHtml&    operator<<(WebHtml&, Submit);    
    }
}

