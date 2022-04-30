////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

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
    }
}
