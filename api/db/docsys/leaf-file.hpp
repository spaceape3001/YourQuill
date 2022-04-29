////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/docsys/leaf-data.hpp>
#include <yq/file/XmlFile.hpp>

namespace yq {

    //! Leaf, the basis of all evil :)
    class Leaf::File : public XmlFile, public Data {
    public:

        virtual void    reset() override;
        virtual bool    read(ByteArray&&buffer, std::string_view fname) override;
        virtual bool    read(const XmlDocument&, std::string_view fname) override;
        virtual bool    write(XmlDocument&) const override;
    };

    Leaf::KV    x_leaf_kv(const XmlNode* node);
    void        read_xn(Leaf::KV& ret, const XmlNode*);
    void        write_xn(XmlNode*, const Leaf::KV&);
}
