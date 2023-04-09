////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafArg.hpp"

#include <basic/TextUtils.hpp>
#include <aether/http/web/WebContext.hpp>
#include <aether/kernel/leaf/LeafCDB.hpp>

namespace yq {

    namespace arg {
        Leaf leaf(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Leaf();
                
            Leaf t   = cdb::leaf( arg_string);
            if(t)
                return t;
            t       = cdb::leaf_by_title(arg_string);
            if(t)
                return t;
                
            return leaf_id(arg_string);
        }
        
        Leaf leaf(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return leaf_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return leaf_key(k);
            
            k       = ctx.find_query("leaf");
            if(!k.empty())
                return leaf(k);
            return Leaf();
        }
        
        
        Leaf leaf(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf(arg_string);
        }
        
        Leaf leaf(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf(arg_string);
        }

        Leaf leaf_id(std::string_view arg_string)
        {
            auto vv   = to_uint64(arg_string);
            if(!vv)
                return Leaf();
            if(!cdb::exists_leaf(*vv))
                return Leaf();
            return Leaf(*vv);
        }

        Leaf leaf_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_id(arg_string);
        }
        
        Leaf leaf_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_id(arg_string);
        }
        
        Leaf leaf_key(std::string_view arg_string)
        {
            return cdb::leaf(trimmed(arg_string));
        }
        
        Leaf leaf_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_key(arg_string);
        }
        
        Leaf leaf_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_key(arg_string);
        }

        Leaf leaf_title(std::string_view arg_string)
        {
            return cdb::leaf_by_title(trimmed(arg_string));
        }
        
        Leaf leaf_title(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_title(arg_string);
        }
        
        Leaf leaf_title(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return leaf_title(arg_string);
        }
    }
}
