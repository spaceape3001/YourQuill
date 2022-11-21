////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/ClassData.hpp>
#include <kernel/StdFile.hpp>

#define CLASS_XML_RESAVE 0

namespace yq {

    //! Atom classes (including fields)
    class Class::File : public StdFile, public Data {
    public:

        virtual bool    recursive_attributes() const override { return false; }
        virtual bool    has_body() const override { return false; }
        
        #if CLASS_XML_RESAVE
        virtual bool    xml_read() const override { return true; }
        #else
        virtual bool    xml_read() const override { return false; }
        #endif

    protected:
        virtual void    reset() override;

        #if CLASS_XML_RESAVE
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        #endif
        
        virtual bool    read(KVTree&&, std::string_view fname) override;
        virtual bool    write(KVTree&) const override;
    };

}
