////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/org/TagData.hpp>
#include <kernel/io/StdFile.hpp>

#define TAG_XML_RESAVE  0

namespace yq {

    class Tag::File : public StdFile, public Data {
    public:

        virtual bool    recursive_attributes() const override { return false; }
        virtual bool    has_body() const override { return false; }
        
        #if TAG_XML_RESAVE
        virtual bool    xml_read() const override { return true; }
        #else
        virtual bool    xml_read() const override { return false; }
        #endif

    protected:
        virtual void    reset() override;
        
        #if TAG_XML_RESAVE
        virtual std::error_code     read(const XmlDocument&, std::string_view fname) override;
        #endif
        
        virtual std::error_code     read(KVTree&&, std::string_view fname) override;
        virtual std::error_code     write(KVTree&) const override;
    };
}
