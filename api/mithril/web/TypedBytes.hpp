////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq-toolbox/basic/Http.hpp>
#include <yq-toolbox/container/ByteArray.hpp>
#include <yq-toolbox/basic/Ref.hpp>

namespace yq::mithril {
    struct TypedBytes : public RefCount {
    
        std::shared_ptr<ByteArray>  data;
        ContentType                 type;
        size_t                      size            = 0;
        std::filesystem::path       path;
        HttpStatus                  status;
        char                        modified[256]   = "";

        TypedBytes() {}
        TypedBytes(const std::filesystem::path _p) : path(_p) {}
    
        bool    do_info();
        bool    do_read();

        //!  Reads the file info
        static Ref<TypedBytes> info(const std::filesystem::path&);

        //!  Loads the file info
        static Ref<TypedBytes> load(const std::filesystem::path&);
        
    };
    
}
