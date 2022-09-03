////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/meta/InfoBinder.hpp>
#include <basic/Enum.hpp>
#include <basic/Flag.hpp>

namespace yq {

    YQ_ENUM(Vcs, , 
        None,       //!< No VCS
        RCS,        //!< RCS
        SVN,        //!< Subversion
        GIT         //!< GIT
    )

    using VcsFlags      = Flag<Vcs>;
}

YQ_TYPE_DECLARE(yq::Vcs)
YQ_TYPE_DECLARE(yq::VcsFlags)
