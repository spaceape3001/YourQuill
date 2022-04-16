#pragma once


#include <yq/preamble.hpp>
#include <memory>
#include "User.hpp"
#include <yq/bit/Authentication.hpp>
#include <yq/bit/Context.hpp>
#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>
#include <yq/enum/Permission.hpp>

#pragma once

namespace yq {

    struct User::Data {
        std::string         name;
        Authentication      authentication;
        Vector<Context>     bio;
        Permissions         permissions;
        std::string         brief;

        /*! \brief Merges two user  documents
        
            \param[in] fOverride  TRUE to allo for contents of new to clobber old; otherwise, won't clobber non-empty fields.
        */
        Data&       merge(const Data&, bool fOverride=false);
        void        reset();
    };
}

