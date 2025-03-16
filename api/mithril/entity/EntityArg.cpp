////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "EntityArg.hpp"

#include <mithril/web/WebContext.hpp>
#include <mithril/class/Class.hpp>
#include <mithril/entity/EntityCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Entity entity(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Entity{};
            
        Entity t   = cdb::entity( arg_string);
        if(t)
            return t;
        return entity_id(arg_string);
    }
    
    Entity entity(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return entity_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return entity_key(k);
        
        k       = ctx.find_query("entity");
        if(!k.empty())
            return entity(k);
        return Entity{};
    }
    
    
    Entity entity(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return entity(arg_string);
    }
    
    Entity entity(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return entity(arg_string);
    }

    Entity entity_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Entity{};
        if(!cdb::exists_entity(*vv))
            return Entity{};
        return Entity{*vv};
    }

    Entity entity_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return entity_id(arg_string);
    }
    
    Entity entity_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return entity_id(arg_string);
    }
    
    Entity entity_key(std::string_view arg_string)
    {
        return cdb::entity(trimmed(arg_string));
    }
    
    Entity entity_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return entity_key(arg_string);
    }
    
    Entity entity_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return entity_key(arg_string);
    }
}
