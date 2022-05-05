////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "std_file.hpp"

#include <yq/bit/KeyValue.hpp>
#include <yq/stream/Text.hpp>
#include <yq/type/ByteArray.hpp>

#include <rapidxml.hpp>

namespace yq {
    bool        StdFile::read(ByteArray&&buffer, std::string_view fname) 
    {
        std::string_view    file    = buffer.as_view();
        if(xml_read() && starts_igCase(file, "<?xml")){
            buffer << '\0';
            XmlDocument     doc;
            try {
                doc.parse<0>(buffer.data());
            } catch(const rapidxml::parse_error& pe){
                size_t  pt  = pe.where<char>() - buffer.data();
                yError() << "Xml parse error: " << pe.what() << " (at byte " << pt << ") : " << fname;
                return false;
            }
            
            if(!read(doc,fname))
                return false;
            
            //  overwrite with key-value
            save_to(fname);
            return true;
        } else {
            KVTree      tree;
            
            unsigned int    opts    = 0;
            if(has_body())
                opts |= KVTree::BODY;
            if(recursive_attributes())
                opts |= KVTree::RECURSIVE;
            
            auto ret    = tree.parse(buffer.as_view(), fname, opts);
            if(!ret.good) [[unlikely]]
                return false;
            if(has_body())
                return read(std::move(tree), ret.body, fname);
            else
                return read(std::move(tree), fname);
        }
    }

    bool        StdFile::write(yq::Stream& buffer) const
    {
        KVTree          tree;
        std::string     body;
        
        if(has_body()){
            stream::Text        text(body);
            if(!write(tree, text))
                return false;
        } else {
            if(!write(tree))
                return false;
        }
        
        tree.write(buffer);
        if(!body.empty())
            buffer << '\n' << body;
        return true;
    }
}
