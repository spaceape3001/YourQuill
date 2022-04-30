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
    
    bool    Leaf::File::read(KVTree&&tree, std::string_view body) 
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
