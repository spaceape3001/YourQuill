////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <compare>
#include <cstdint>

namespace yq {

    /*! \brief Field of the class

        A field is a pre-declared attribute for any atom using the specified class.  It's recommended to keep fields
        unique in purpose based on the name.
        
        A field may be bound to a single class, the defining file will have a class.field.fld format to its file name.
        A field may be bound to one or more classes, at which point, the defining file will have a field.fld format to 
        it's file name and it's class id in the cdb will be 0.  
    */
    struct Field {
        static constexpr const char*    szExtension = "fld";
        struct Info;
        struct Data;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Field&rhs) const = default;
        constexpr operator bool() const { return id != 0ULL; }
    };
}
