////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/atomsys/field-data.hpp>
#include <yq/file/XmlFile.hpp>

namespace yq {
    class Field::File : public Data, public XmlFile {
    public:
    protected:
        void reset() override;
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        virtual bool    write(XmlDocument&) const override;
   
    };
}
