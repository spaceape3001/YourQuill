////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/atom/ValueData.hpp>
#include <yq/io/XmlFile.hpp>

namespace yq {

    class Value::ListFile : public ListData, public XmlFile {
    public:
    
    protected:
        virtual bool    read(const XmlDocument&, std::string_view fname) = 0;
        virtual bool    write(XmlDocument&) const = 0;
    };

}
