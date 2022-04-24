////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "data.hpp"
#include <yq/file/XmlFile.hpp>

namespace yq {

    //! Atom classes (including fields)
    class Class::File : public Data, public XmlFile {
    public:

    protected:
        virtual void    reset() override;

        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        virtual bool    write(XmlDocument&) const override;
    };

}
