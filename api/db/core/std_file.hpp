////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/file/AbstractFile.hpp>

namespace yq {
    class Stream;
    class KVTree;
    class ByteArray;

    class StdFile : public AbstractFile {
    public:

        virtual bool        recursive_attributes() const { return true; }
        virtual bool        has_body() const { return false; }

    protected:
        virtual bool        read(ByteArray&&buffer, std::string_view fname) override;
        virtual bool        write(Stream&) const override;
        
        //  override depending on has_body() above
        
        virtual bool        read(KVTree&&, std::string&&) { return true; }
        virtual bool        write(KVTree&, Stream&) const { return true; }

        virtual bool        read(KVTree&&) { return true; }
        virtual bool        write(KVTree&) const { return true; }
    };
}
