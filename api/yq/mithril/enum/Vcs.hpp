////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/core/Flags.hpp>
#include <yq/meta/MetaBinder.hpp>

namespace yq::mithril {

    enum class Vcs {
        None,       //!< No VCS
        RCS,        //!< RCS
        SVN,        //!< Subversion
        GIT         //!< GIT
    };

    using VcsFlags      = Flags<Vcs,uint8_t>;
}

YQ_TYPE_DECLARE(yq::mithril::Vcs)
YQ_TYPE_DECLARE(yq::mithril::VcsFlags)
