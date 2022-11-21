////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/AssertDeny.hpp>
#include <basic/preamble.hpp>

namespace yq {

    struct Copyright {

        //! Text of the licence (or disclaimer)
        std::string     text;
        
        //! Year to start (0 for indefinite)
        unsigned short  from        = 0;
        
        //! Year to end (0 for today/indefinite)
        unsigned short  to          = 0;
        
        //! \brief Whether we assert copyright over, or disclaim copyright
        //!
        //! If the user *owns* the copyright, then this should be ASSERT
        //! If the user is leveraging another's copyright (ie, fanfiction/fanart),
        //! then this should be deny.
        AssertDeny      stance;
        
        auto    operator<=>(const Copyright&) const = default;

        bool    empty() const;
        bool    not_null() const;
        void    reset();
        
        void    merge(const Copyright&);
    };

    Result<Copyright>   x_copyright(const XmlNode*);
    void                write_xn(XmlNode* xb, const Copyright&);

}
