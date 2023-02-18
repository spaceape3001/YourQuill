////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/ValueData.hpp>
#include <basic/XmlFile.hpp>

namespace yq {

    class Value::ListFile : public ListData, public XmlFile {
    public:
    
    protected:
        virtual std::error_code    read(const XmlDocument&, std::string_view fname) override = 0;
        virtual std::error_code    write(XmlDocument&) const override = 0;
    };

}
