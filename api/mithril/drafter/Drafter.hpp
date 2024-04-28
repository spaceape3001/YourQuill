////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/id/Id.hpp>
#include <mithril/document/Document.hpp>

namespace yq::mithril {

    /*! \brief Drafter is a 3D model that the user creates
    */

    struct Drafter {
        static constexpr const IdTypeId ID          = 10;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "d3x";
        
        struct Info;
        struct Data;
        struct File;
        struct Diff;

        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        using Notify = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Drafter&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Drafter)
YQ_TYPE_DECLARE(yq::mithril::DrafterSet)
YQ_TYPE_DECLARE(yq::mithril::DrafterVector)
