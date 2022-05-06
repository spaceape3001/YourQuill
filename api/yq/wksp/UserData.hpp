////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/bit/Context.hpp>
#include <yq/collection/Set.hpp>
#include <yq/enum/Permission.hpp>
#include <yq/type/Ref.hpp>
#include <yq/wksp/Authentication.hpp>
#include <yq/wksp/User.hpp>

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

