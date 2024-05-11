////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#error "Do NOT include (for now)"

#pragma once

#include <mithril/doodle/DoodleData.hpp>
#include <mithril/io/StdFile.hpp>

#define TAG_XML_RESAVE  0

namespace yq::mithril {

    class Doodle::File : public StdFile, public Data {
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
        
        virtual std::error_code     read(KVTree&&, std::string_view fname) override;
        virtual std::error_code     write(KVTree&) const override;
    };
}
