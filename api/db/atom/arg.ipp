////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "arg.hpp"
#include "cdb.hpp"

namespace yq {

    namespace arg {
        Atom atom(std::string_view  arg_string)
        {
            arg_string   = trimmed(arg_string);
            if(arg_string.empty())
                return Atom{};
                
            Atom t   = cdb::atom( arg_string);
            if(t)
                return t;
            uint64_t    i = to_uint64( arg_string).value;
            if(cdb::exists_atom(i))
                return Atom{i};
            return Atom{};
        }
        
        Atom atom(const WebContext&ctx, bool *detected)
        {
            std::string    k    = ctx.find_query("id");
            if(!k.empty())
                return atom_id(k);
            
            k       = ctx.find_query("key");
            if(!k.empty())
                return atom_key(k);
            
            k       = ctx.find_query("atom");
            if(!k.empty())
                return atom(k);
            return Atom{};
        }
        
        
        Atom atom(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom(arg_string);
        }
        
        Atom atom(const WebContext& ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom(arg_string);
        }

        Atom atom_id(std::string_view arg_string)
        {
            uint64_t    i   = to_uint64(arg_string).value;
            if(cdb::exists_atom(i))
                return Atom{i};
            return Atom{};
        }

        Atom atom_id(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_id(arg_string);
        }
        
        Atom atom_id(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_id(arg_string);
        }
        
        Atom atom_key(std::string_view arg_string)
        {
            return cdb::atom(trimmed(arg_string));
        }
        
        Atom atom_key(const WebContext&ctx, std::string_view arg_name, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_name);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_key(arg_string);
        }
        
        Atom atom_key(const WebContext&ctx, std::initializer_list<std::string_view> arg_names, bool *detected)
        {
            std::string     arg_string = ctx.find_query(arg_names);
            if(detected)
                *detected   = !arg_string.empty();
            return atom_key(arg_string);
        }
    }
}
