////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ImageArg.hpp"

#include <basic/TextUtils.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/image/ImageCDB.hpp>

namespace yq::mithril::arg {
    Image image(std::string_view  arg_string)
    {
        auto vv = to_uint64( arg_string);
        if(!vv)
            return Image();
        if(!cdb::exists_image(*vv))
            return Image();
        return Image(*vv);
    }
    
    Image image(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return image(k);
        k       = ctx.find_query("image");
        if(!k.empty())
            return image(k);
        return Image();
    }
    
    
    Image image(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return image(arg_string);
    }
    
    Image image(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return image(arg_string);
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


    SizeDesc    size_desc(std::string_view k)
    {
        return SizeDesc(k);
    }
    
    SizeDesc    size_desc(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return size_desc(arg_string);
    }
    
    SizeDesc    size_desc(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return size_desc(arg_string);
    }

}
