////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/document/Document.hpp>

namespace yq::mithril {

    /*! \brief Style is a physical/graphical presentation
    
        Style is the physical/graphical/textual presentation on the screen.  
    */ 
    struct Style {
        static constexpr const IdTypeId ID          = 26;
        static constexpr const IdTypes  PARENTS     = Document::ID;
        static constexpr const char*    EXTENSION   = "sty";
        struct Info;
        struct Data;
        struct Diff;
        struct File;
        using SharedData = std::shared_ptr<Data>;
        using SharedFile = std::shared_ptr<File>;
        
        using Notify = Notifier<const Diff&>;
        
        uint64_t  id  = 0ULL;
        constexpr auto    operator<=>(const Style&rhs) const noexcept = default;
        constexpr operator uint64_t () const noexcept { return id; }
        constexpr bool valid() const { return static_cast<bool>(id); }
    };
}

YQ_TYPE_DECLARE(yq::mithril::Style)
YQ_TYPE_DECLARE(yq::mithril::StyleSet)
YQ_TYPE_DECLARE(yq::mithril::StyleVector)
