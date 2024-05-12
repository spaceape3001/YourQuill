////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventArg.hpp"

#include <0/basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/event/EventCDB.hpp>

namespace yq::mithril::arg {
    Event event(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Event{};
            
        Event t   = cdb::event( arg_string);
        if(t)
            return t;
        return event_id(arg_string);
    }
    
    Event event(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return event_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return event_key(k);
        
        k       = ctx.find_query("event");
        if(!k.empty())
            return event(k);
        return Event{};
    }
    
    
    Event event(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return event(arg_string);
    }
    
    Event event(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return event(arg_string);
    }

    Event event_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Event{};
        if(!cdb::exists_event(*vv))
            return Event{};
        return Event{*vv};
    }

    Event event_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return event_id(arg_string);
    }
    
    Event event_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return event_id(arg_string);
    }
    
    Event event_key(std::string_view arg_string)
    {
        return cdb::event(trimmed(arg_string));
    }
    
    Event event_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return event_key(arg_string);
    }
    
    Event event_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return event_key(arg_string);
    }
}
