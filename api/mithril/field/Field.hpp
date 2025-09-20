////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>

namespace yq::mithril {

    /*! \brief Field of the class

        A field is a pre-declared attribute for any atom using the specified class.  It's recommended to keep fields
        unique in purpose based on the name.
        
        A field may be bound to a single class, the defining file will have a class.field.fld format to its file name.
        A field may be bound to one or more classes, at which point, the defining file will have a field.fld format to 
        it's file name and it's class id in the cdb will be 0.  
    */
    struct Field {
        static constexpr const IdTypeId ID          = 13;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "fld";
        struct Info;
        struct Data;
        struct Rank;
        class File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        struct Diff;
        
        using Notify = Notifier<const Diff&>;

        static bool less_key(Field a, Field b);
        static bool less_label(Field a, Field b);
        static bool less_name(Field a, Field b);

        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Field&rhs) const noexcept = default;
        constexpr operator uint64_t() const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };

    using FieldFragDoc      = std::pair<Fragment, Field::SharedFile>;
}

YQ_TYPE_DECLARE(yq::mithril::Field)
YQ_TYPE_DECLARE(yq::mithril::FieldSet)
YQ_TYPE_DECLARE(yq::mithril::FieldVector)
