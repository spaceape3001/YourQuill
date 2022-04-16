////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UserData.hpp"
#include <yq/file/XmlFile.hpp>

namespace yq {
    class User::File : public XmlFile, public User::Data {
    public:
    
        //  Set this before load
        bool            skip_bio    = false;
    
    protected:
        virtual void    reset() override;
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        virtual bool    write(XmlDocument&) const override;
    };
}
