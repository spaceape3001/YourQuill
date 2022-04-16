////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserGroupData.hpp"
#include <yq/file/XmlFile.hpp>

namespace yq {
    class UserGroup::File : public XmlFile, public UserGroup::Data {
    public:
    
        //  Set this before load
        bool            skip_bio    = false;

    protected:
        virtual void    reset() override;
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        virtual bool    write(XmlDocument&) const override;
    };
}
