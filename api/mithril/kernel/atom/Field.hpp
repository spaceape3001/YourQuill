////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/preamble.hpp>

namespace yq::mithril {

    /*! \brief Field of the class

        A field is a pre-declared attribute for any atom using the specified class.  It's recommended to keep fields
        unique in purpose based on the name.
        
        A field may be bound to a single class, the defining file will have a class.field.fld format to its file name.
        A field may be bound to one or more classes, at which point, the defining file will have a field.fld format to 
        it's file name and it's class id in the cdb will be 0.  
    */
    struct Field {
        static constexpr const char*    szExtension = "field";
        struct Info;
        struct Data;
        struct Rank;
        class File;
        using Lock = IDLock<Field>;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;

        static bool less_key(Field a, Field b);
        static bool less_label(Field a, Field b);
        static bool less_name(Field a, Field b);

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Field&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
    };

    using FieldFragDoc      = std::pair<Fragment, Field::SharedFile>;
}

YQ_TYPE_DECLARE(yq::mithril::Field)
