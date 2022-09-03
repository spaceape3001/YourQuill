////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/atom/FieldData.hpp>
#include <kernel/io/StdFile.hpp>

#define FIELD_XML_RESAVE 0

namespace yq {
    class Field::File : public StdFile, public Data {
    public:

        virtual bool    recursive_attributes() const override { return false; }
        virtual bool    has_body() const override { return false; }
        
        #if FIELD_XML_RESAVE
        virtual bool    xml_read() const override { return true; }
        #else
        virtual bool    xml_read() const override { return false; }
        #endif


    protected:
        void reset() override;

        #if FIELD_XML_RESAVE
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        #endif
        
        virtual bool    read(KVTree&&, std::string_view fname) override;
        virtual bool    write(KVTree&) const override;
   
    };
}
