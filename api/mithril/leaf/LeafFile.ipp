////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafFile.hpp"
#include <0/io/StreamOps.hpp>


namespace yq::mithril {
    void    Leaf::File::reset() 
    {
        Data::reset();
    }
    
    std::error_code   Leaf::File::read(KVTree&&tree, std::string_view body, std::string_view) 
    {
        attrs               = std::move(tree);
        return read_kv(context, body);
    }
    
    std::error_code   Leaf::File::write(Stream& pipe) const
    {
        attrs.write(pipe);
        
        if(!context.empty()){
            pipe << '\n';
            write_kv(pipe, context);
        }
        
        return std::error_code();
    }
}
