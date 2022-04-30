////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "std_file.hpp"

#include <db/bit/key_value.hpp>
#include <yq/stream/Text.hpp>
#include <yq/type/ByteArray.hpp>

namespace yq {
    bool        StdFile::read(ByteArray&&buffer, std::string_view fname) 
    {
        KVTree      tree;
        std::string body;
        
        if(!tree.parse(buffer, (has_body()?&body:nullptr), recursive_attributes(), fname))
            return false;
        if(has_body())
            return read(std::move(tree), std::move(body));
        else
            return read(std::move(tree));
    }

    bool        StdFile::write(yq::Stream& buffer) const
    {
        KVTree          tree;
        std::string     body;
        
        if(has_body()){
            stream::Text        text(body);
            if(!write(tree, text))
                return false;
        } else {
            if(!write(tree))
                return false;
        }
        
        tree.write(buffer);
        if(!body.empty())
            buffer << '\n' << body;
        return true;
    }
}
