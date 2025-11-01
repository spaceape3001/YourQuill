////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "NovelArg.hpp"
#include <yq/mithril/web/WebContext.hpp>
#include <yq/mithril/novel/NovelCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Novel novel(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Novel();
            
        Novel t   = cdb::novel( arg_string);
        if(t)
            return t;
        t       = cdb::novel_by_title(arg_string);
        if(t)
            return t;
            
        return novel_id(arg_string);
    }
    
    Novel novel(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return novel_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return novel_key(k);
        
        k       = ctx.find_query("novel");
        if(!k.empty())
            return novel(k);
        return Novel();
    }
    
    
    Novel novel(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return novel(arg_string);
    }
    
    Novel novel(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return novel(arg_string);
    }

    Novel novel_id(std::string_view arg_string)
    {
        auto vv   = to_uint64(arg_string);
        if(!vv)
            return Novel();
        if(!cdb::exists_novel(*vv))
            return Novel();
        return Novel(*vv);
    }

    Novel novel_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return novel_id(arg_string);
    }
    
    Novel novel_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return novel_id(arg_string);
    }
    
    Novel novel_key(std::string_view arg_string)
    {
        return cdb::novel(trimmed(arg_string));
    }
    
    Novel novel_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return novel_key(arg_string);
    }
    
    Novel novel_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return novel_key(arg_string);
    }

    Novel novel_title(std::string_view arg_string)
    {
        return cdb::novel_by_title(trimmed(arg_string));
    }
    
    Novel novel_title(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return novel_title(arg_string);
    }
    
    Novel novel_title(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return novel_title(arg_string);
    }
}
