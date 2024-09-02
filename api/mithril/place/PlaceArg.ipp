////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PlaceArg.hpp"


#include <mithril/web/WebContext.hpp>
#include <mithril/place/PlaceCDB.hpp>

namespace yq::mithril::arg {
    Place place(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Place{};
            
        Place t   = cdb::place( arg_string);
        if(t)
            return t;
        return place_id(arg_string);
    }
    
    Place place(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return place_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return place_key(k);
        
        k       = ctx.find_query("place");
        if(!k.empty())
            return place(k);
        return Place{};
    }
    
    
    Place place(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return place(arg_string);
    }
    
    Place place(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return place(arg_string);
    }

    Place place_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Place{};
        if(!cdb::exists_place(*vv))
            return Place{};
        return Place{*vv};
    }

    Place place_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return place_id(arg_string);
    }
    
    Place place_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return place_id(arg_string);
    }
    
    Place place_key(std::string_view arg_string)
    {
        return cdb::place(trimmed(arg_string));
    }
    
    Place place_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return place_key(arg_string);
    }
    
    Place place_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return place_key(arg_string);
    }
}
