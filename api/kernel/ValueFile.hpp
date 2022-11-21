////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/ValueData.hpp>
#include <basic/XmlFile.hpp>

namespace yq {

    class Value::ListFile : public ListData, public XmlFile {
    public:
    
    protected:
        virtual bool    read(const XmlDocument&, std::string_view fname) = 0;
        virtual bool    write(XmlDocument&) const = 0;
    };

}
