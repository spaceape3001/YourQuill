////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/InfoBinder.hpp>
#include <yq-toolbox/basic/Enum.hpp>
#include <yq-toolbox/basic/Flag.hpp>

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
