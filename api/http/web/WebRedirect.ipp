////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WebRedirect.hpp"

namespace yq {
    namespace redirect {
        WebRedirect     found(const Url& v)
        {
            return WebRedirect{ v, HttpStatus::Found };
        }
        
        WebRedirect     found(const UrlView& v)
        {
            return WebRedirect{ copy(v), HttpStatus::Found };
        }
        
        WebRedirect     found(std::string_view v)
        {
            return WebRedirect{ copy(to_url_view(v).value), HttpStatus::Found };
        }

        WebRedirect     permanent(const Url& v)
        {
            return WebRedirect{ v, HttpStatus::PermanentRedirect };
        }
        
        WebRedirect     permanent(const UrlView& v)
        {
            return WebRedirect{ copy(v), HttpStatus::PermanentRedirect };
        }
        
        WebRedirect     permanent(std::string_view v)
        {
            return WebRedirect{ copy(to_url_view(v).value), HttpStatus::PermanentRedirect };
        }

        WebRedirect     posted(const Url& v)
        {
            return WebRedirect{ v, HttpStatus::PostedRedirect };
        }
        
        WebRedirect     posted(const UrlView& v)
        {
            return WebRedirect{ copy(v), HttpStatus::PostedRedirect };
        }
        
        WebRedirect     posted(std::string_view v)
        {
            return WebRedirect{ copy(to_url_view(v).value), HttpStatus::PostedRedirect };
        }
        
        WebRedirect     see_other(const Url&v)
        {
            return WebRedirect{ v, HttpStatus::SeeOther };
        }
        
        WebRedirect     see_other(const UrlView&v)
        {
            return WebRedirect{ copy(v), HttpStatus::SeeOther };
        }
        
        WebRedirect     see_other(std::string_view v)
        {
            return WebRedirect{ copy(to_url_view(v).value), HttpStatus::SeeOther };
        }
        
        WebRedirect     temporary(const Url& v)
        {
            return WebRedirect{ v, HttpStatus::TemporaryRedirect };
        }
        
        WebRedirect     temporary(const UrlView& v)
        {
            return WebRedirect{ copy(v), HttpStatus::TemporaryRedirect };
        }
        
        WebRedirect     temporary(std::string_view v)
        {
            return WebRedirect{ copy(to_url_view(v).value), HttpStatus::TemporaryRedirect };
        }
        
    }
}
