////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EventPost.hpp"

#include <yq/mithril/event/EventArg.hpp>
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/event/Event.hpp>
#include <yq/text/copy.hpp>

namespace yq::mithril::post {
    Event event(WebContext&ctx, bool *detected)
    {
        ctx.decode_post();
        
        if(detected)
            *detected   = false;
            
        std::string    k    = ctx.rx_post.first("event");
        if(!k.empty()){
            if(detected)
                *detected   = true;
            return arg::event_id(k);
        }
        return Event();
    }
    
    Event event(WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_name));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::event_id(arg_string);
    }
    
    Event event(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.rx_post.first(copy(arg_names));
        if(detected)
            *detected   = !arg_string.empty();
        return arg::event_id(arg_string);
    }
}
