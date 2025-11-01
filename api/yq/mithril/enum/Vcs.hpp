////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/MetaBinder.hpp>
#include <yq/core/Enum.hpp>
#include <yq/core/Flag.hpp>

namespace yq {
    namespace mithril {

        YQ_ENUM(Vcs, , 
            None,       //!< No VCS
            RCS,        //!< RCS
            SVN,        //!< Subversion
            GIT         //!< GIT
        )

        using VcsFlags      = Flag<Vcs>;
    }
}

YQ_TYPE_DECLARE(yq::mithril::Vcs)
YQ_TYPE_DECLARE(yq::mithril::VcsFlags)
