////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <0/meta/InfoBinder.hpp>
#include <0/basic/Enum.hpp>
#include <0/basic/Flag.hpp>

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
