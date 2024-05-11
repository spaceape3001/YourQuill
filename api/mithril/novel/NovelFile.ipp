////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#error "Do NOT include (for now)"

#include "NovelFile.hpp"
#include <0/io/StreamOps.hpp>


namespace yq::mithril {


    void    Novel::File::reset() 
    {
        Data::reset();
    }
    
    #if 0
    
    std::error_code   Novel::File::read(KVTree&&tree, std::string_view body, std::string_view) 
    {
        attrs               = std::move(tree);
        return read_kv(context, body);
    }
    
    std::error_code   Novel::File::write(Stream& pipe) const
    {
        attrs.write(pipe);
        
        if(!context.empty()){
            pipe << '\n';
            write_kv(pipe, context);
        }
        
        return std::error_code();
    }
    
    #endif
    
}
