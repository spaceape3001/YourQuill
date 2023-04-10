////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/kernel/enum/Access.hpp>
#include <mithril/kernel/enum/DataRole.hpp>

namespace yq::mithril {

    using PolicyMap     = EnumMap<DataRole,Access>;

    Expect<Access>      decode_access(std::string_view arg);

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
