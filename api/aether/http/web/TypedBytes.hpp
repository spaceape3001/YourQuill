////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <basic/Http.hpp>
#include <basic/ByteArray.hpp>
#include <basic/Ref.hpp>

namespace yq {
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