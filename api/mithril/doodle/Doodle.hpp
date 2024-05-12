////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>
#include <mithril/document/Document.hpp>

namespace yq::mithril {

    /*! \brief Doodle is a 3D model that the user creates
    */

    struct Doodle {
        static constexpr const IdTypeId ID          = 10;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "d";
        
        struct Info;
        //struct Data;
        //struct File;
        struct Diff;

        //using SharedData = std::shared_ptr<Data>;
        //using SharedFile = std::shared_ptr<File>;
        
        using Notify = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Doodle&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Doodle)
YQ_TYPE_DECLARE(yq::mithril::DoodleSet)
YQ_TYPE_DECLARE(yq::mithril::DoodleVector)
