////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafArg.hpp"

#include <basic/TextUtils.hpp>
#include <http/Post.hpp>
#include <http/WebContext.hpp>
#include <kernel/LeafCDB.hpp>

namespace yq {

    namespace arg {
        Leaf leaf(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Leaf{};
                
            Leaf t   = cdb::leaf( arg_string);
            if(t)
                return t;
            t       = cdb::leaf_by_title(arg_string);
            if(t)
                return t;
                
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_leaf(i))
                return Leaf{i};
            return Leaf{};
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
            return Leaf{};
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
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_leaf(i))
                return Leaf{i};
            return Leaf{};
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

    namespace post {
        Leaf leaf(WebContext&ctx, bool *detected)
        {
            ctx.decode_post();
            
            if(detected)
                *detected   = false;
                
            std::string    k    = ctx.rx_post.first("leaf");
            if(!k.empty()){
                if(detected)
                    *detected   = true;
                return arg::leaf_id(k);
            }
            return Leaf();
        }
        
        Leaf leaf(WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_name));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
        
        Leaf leaf(WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.rx_post.first(copy(arg_names));
            if(detected)
                *detected   = !arg_string.empty();
            return arg::leaf_id(arg_string);
        }
    }
}
