////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/enum/Access.hpp>
#include <yq/enum/DataRole.hpp>

namespace yq {

    using PolicyMap     = EnumMap<DataRole,Access>;

    Result<Access>      decode_access(const std::string_view& arg);

    /*! \brief Merges access levels

        This merges access levels, assuming a is superior
        
        \param[in]  Superior access
        \parma[in]  Subordinate access, modified
        \return Modified subordinate access, taking the superior into account
    */
    Access      moderate(Access a, Access b);
    Access      moderate(Access a, Access b, Access limit);

    PolicyMap   moderate(const PolicyMap& a, const PolicyMap& b);
    PolicyMap   moderate(const PolicyMap& a, const PolicyMap& b, Access limit);
    PolicyMap   moderate(Access a, const PolicyMap& b);

}