////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/core/Ref.hpp>
#include <yq/container/Set.hpp>

#include <mithril/bit/Context.hpp>
#include <mithril/enum/Permission.hpp>
#include <mithril/auth/Authentication.hpp>
#include <mithril/user/User.hpp>

#include <memory>
#include <vector>

#pragma once

namespace yq::mithril {
    struct User::Data {
        std::string             name;
        Ref<Authentication>     authentication;
        std::vector<Context>    bio;
        Permissions             permissions;
        std::string             brief;

        /*! \brief Merges two user  documents
        
            \param[in] fOverride  TRUE to allo for contents of new to clobber old; otherwise, won't clobber non-empty fields.
        */
        Data&       merge(const Data&, bool fOverride=false);
        void        reset();
    };
}

