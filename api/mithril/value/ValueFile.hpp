////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/value/ValueData.hpp>
#include <yq-toolbox/io/XmlFile.hpp>

namespace yq::mithril {

    class Value::ListFile : public ListData, public XmlFile {
    public:
    
    protected:
        virtual std::error_code    read(const XmlDocument&, std::string_view fname) override = 0;
        virtual std::error_code    write(XmlDocument&) const override = 0;
    };

}
