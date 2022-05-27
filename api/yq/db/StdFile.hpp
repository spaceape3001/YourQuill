////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/io/AbstractFile.hpp>
#include <basic/preamble.hpp>

namespace yq {
    class Stream;
    class KVTree;
    class ByteArray;

    class StdFile : public AbstractFile {
    public:

        //! Enable if attributes can recurse
        virtual bool        recursive_attributes() const { return true; }
        
        //! Enable if we're expecting body (ie, context)
        virtual bool        has_body() const { return false; }
        
        //! Enable if we tolerate XML loading (not saving)
        virtual bool        xml_read() const { return false; }

    protected:
        //! Reading (from abstract file)
        virtual bool        read(ByteArray&&buffer, std::string_view fname) override;
        //! Writing (from abstract file)
        virtual bool        write(Stream&) const override;
        
        //  override depending on has_body() above
        
        //! Override to read with body
        virtual bool        read(KVTree&&, std::string_view body, std::string_view fname) { return true; }
        
        //! Override to write with body
        virtual bool        write(KVTree&, Stream&) const { return true; }

        //! Override to read without body
        virtual bool        read(KVTree&&, std::string_view fname) { return true; }
        
        //! Override to write without body
        virtual bool        write(KVTree&) const { return true; }
        
        //! Override to read from XML instead
        virtual bool        read(const XmlDocument&, std::string_view fname) { return true; }
    };
}
