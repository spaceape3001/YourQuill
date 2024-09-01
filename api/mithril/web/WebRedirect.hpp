////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/basic/Http.hpp>
#include <yq/basic/Url.hpp>
    
namespace yq::mithril {

    struct WebRedirect {
        Url         where;
        HttpStatus  why;
    };
    
    namespace redirect {
        WebRedirect     found(const Url&);
        WebRedirect     found(const UrlView&);
        WebRedirect     found(std::string_view);
        WebRedirect     permanent(const Url&);
        WebRedirect     permanent(const UrlView&);
        WebRedirect     permanent(std::string_view);
        WebRedirect     posted(const Url&);
        WebRedirect     posted(const UrlView&);
        WebRedirect     posted(std::string_view);
        WebRedirect     see_other(const Url&);
        WebRedirect     see_other(const UrlView&);
        WebRedirect     see_other(std::string_view);
        WebRedirect     temporary(const Url&);
        WebRedirect     temporary(const UrlView&);
        WebRedirect     temporary(std::string_view);
    };
    
}
