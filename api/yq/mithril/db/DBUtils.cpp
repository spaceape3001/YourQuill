////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/preamble.hpp>
#include <yq/meta/TypeMeta.hpp>
#include <yq/text/basic.hpp>

namespace yq::mithril::cdb {
    std::string make_filename(std::string_view k, const char* ext)
    {
        std::string     ret;
        ret.reserve(2+k.size()+strlen(ext));
        ret = k;
        ret += '.';
        ret += ext;
        return ret;
    }

    std::string_view         base_key(std::string_view key)
    {
        size_t i   = key.find_last_of('/');
        if(i == std::string_view::npos){
            size_t j   = key.find_first_of('.');
            if(j == std::string_view::npos)
                return key;
            return key.substr(0,j);
        } else {
            size_t j   = key.find_first_of('.', i);
            if(j == std::string_view::npos)
                return key.substr(i+1);
            return key.substr(i+1,j-i-1);
        }
    }
    
}

