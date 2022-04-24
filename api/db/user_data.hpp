#pragma once


#include <yq/preamble.hpp>
#include <memory>
#include "user.hpp"
#include "authentication.hpp"
#include "bit_context.hpp"
#include "enum_permission.hpp"
#include <yq/collection/Set.hpp>
#include <yq/collection/Vector.hpp>

#pragma once

namespace yq {

    struct User::Data {
        std::string         name;
        Ref<Authentication> authentication;
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

