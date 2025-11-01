////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "CharacterArg.hpp"

#include <mithril/web/WebContext.hpp>
#include <mithril/character/CharacterCDB.hpp>
#include <yq/text/parse.hpp>
#include <yq/text/transform.hpp>

namespace yq::mithril::arg {
    Character character(std::string_view  arg_string)
    {
        arg_string   = trimmed(arg_string);
        if(arg_string.empty())
            return Character{};
            
        Character t   = cdb::character( arg_string);
        if(t)
            return t;
        return character_id(arg_string);
    }
    
    Character character(const WebContext&ctx, bool *detected)
    {
        std::string    k    = ctx.find_query("id");
        if(!k.empty())
            return character_id(k);
        
        k       = ctx.find_query("key");
        if(!k.empty())
            return character_key(k);
        
        k       = ctx.find_query("character");
        if(!k.empty())
            return character(k);
        return Character{};
    }
    
    
    Character character(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return character(arg_string);
    }
    
    Character character(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return character(arg_string);
    }

    Character character_id(std::string_view arg_string)
    {
        auto vv = to_uint64(arg_string);
        if(!vv)
            return Character{};
        if(!cdb::exists_character(*vv))
            return Character{};
        return Character{*vv};
    }

    Character character_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return character_id(arg_string);
    }
    
    Character character_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return character_id(arg_string);
    }
    
    Character character_key(std::string_view arg_string)
    {
        return cdb::character(trimmed(arg_string));
    }
    
    Character character_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_name);
        if(detected)
            *detected   = !arg_string.empty();
        return character_key(arg_string);
    }
    
    Character character_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
    {
        std::string     arg_string = ctx.find_query(arg_names);
        if(detected)
            *detected   = !arg_string.empty();
        return character_key(arg_string);
    }
}
