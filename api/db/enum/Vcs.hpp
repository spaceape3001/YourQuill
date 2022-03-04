////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <util/meta/Binder.hpp>
#include <util/type/Enum.hpp>
#include <util/type/Flag.hpp>

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
