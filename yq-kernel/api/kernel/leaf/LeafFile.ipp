////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "LeafFile.hpp"
#include <basic/StreamOps.hpp>


namespace yq {


    void    Leaf::File::reset() 
    {
        Data::reset();
    }
    
    bool    Leaf::File::read(KVTree&&tree, std::string_view body, std::string_view fname) 
    {
        attrs               = std::move(tree);
        read_kv(context, body);
        return true;
    }
    
    bool    Leaf::File::write(Stream& pipe) const
    {
        attrs.write(pipe);
        
        if(!context.empty()){
            pipe << '\n';
            write_kv(pipe, context);
        }
        
        return true;
    }
    
}
