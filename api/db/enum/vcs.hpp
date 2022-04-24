////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/meta/InfoBinder.hpp>
#include <yq/type/Enum.hpp>
#include <yq/type/Flag.hpp>

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
