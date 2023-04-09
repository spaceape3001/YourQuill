////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <basic/Ref.hpp>
#include <basic/Set.hpp>

#include <mithril/kernel/bit/Context.hpp>
#include <mithril/kernel/enum/Permission.hpp>
#include <mithril/kernel/user/Authentication.hpp>
#include <mithril/kernel/user/User.hpp>

#include <memory>
#include <vector>

#pragma once

namespace yq {

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

