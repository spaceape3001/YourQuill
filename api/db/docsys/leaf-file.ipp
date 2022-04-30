////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/stream/Text.hpp>

namespace yq {

    void    Leaf::File::reset() 
    {
        Data::reset();
    }
    
    bool    Leaf::File::read(KVTree&&tree, std::string&& body) 
    {
        attrs               = std::move(tree);
        
        Context             cur;
        stream::Text        pipe(cur.data);
        
        vsplit(body, '\n', [&](std::string_view bit){
            if(starts(bit, "#!")){
                bit = trimmed(bit.substr(2));
                std::string_view    cmd, arg;

                size_t  i   = bit.find_first_of(" \t");
                if(i != std::string_view::npos){
                    cmd         = trimmed(bit.substr(0, i));
                    arg         = trimmed(bit.substr(i+1));
                } else 
                    cmd = bit;
                    
                if(is_similar(cmd, szContext)){
                    if(cur != Context()){
                        context.push_back(cur);
                        cur = Context();
                    }
                } else if(is_similar(cmd, szTitle)){
                    cur.title       = std::string(arg);
                } else if(is_similar(cmd, szFormat)){
                    cur.format      = Format(arg);
                } else if(is_similar(cmd, szOrder)){
                    cur.order       = to_uint(arg).value;
                } else if(is_similar(cmd, szIcon)){
                    cur.icon        = std::string(arg);
                } 
                
                //  No else, assumed to be an ill-mannered command
            } else {
                if(cur.data.empty() && (is_space(bit) || bit.empty()))
                    return ;
                if(is_space(bit))
                    pipe << '\n';
                else
                    pipe << bit << '\n';
            }
        });
        
        if(cur != Context())
            context.push_back(cur);
        return true;
    }
    
    bool    Leaf::File::write(Stream& pipe) const
    {
        attrs.write(pipe);
        bool    first   = true;
        
        for(const Context& ctx : context){
            pipe << '\n';
            
            if(first){
                first   = false;
            } else {
                pipe << "#! " << szContext << '\n';
            }
            
            if(!ctx.title.empty())
                pipe << "#! " << szTitle << '\t' << ctx.title << '\n';
            if(!ctx.icon.empty())
                pipe << "#! " << szIcon << '\t' << ctx.icon << '\n';
            if(ctx.order)
                pipe << "#! " << szOrder << '\t' << ctx.order << '\n';
            if(ctx.format != Format())
                pipe << "#! " << szFormat << '\t' << ctx.format.key() << '\n';
        }
        
        return true;
    }
    

}
