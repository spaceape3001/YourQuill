////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <mithril/atom/Atom.hpp>
#include <mithril/atom/AtomCDB.hpp>

#include <yq/file/FileUtils.hpp>
#include <yq/text/transform.hpp>

#include <mithril/document/Document.hpp>
#include <mithril/document/DocumentCDB.hpp>

#include <yq/meta/TypeInfoWriter.hpp>

YQ_TYPE_IMPLEMENT(yq::mithril::Atom)
YQ_TYPE_IMPLEMENT(yq::mithril::AtomSet)
YQ_TYPE_IMPLEMENT(yq::mithril::AtomVector)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::Property)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::PropertySet)
YQ_TYPE_IMPLEMENT(yq::mithril::Atom::PropertyVector)

namespace yq::mithril {
    std::string         Atom::full_key(Document doc, std::string_view ck)
    {
        std::string     kk  = cdb::key(doc);
        std::string     dk = std::string(strip_extension(kk));
        if(dk.empty())    
            return std::string();

        if(!ck.empty()){
            dk += '#';
            dk += ck;
        }
        return dk;
    }

    std::string         Atom::full_key(Atom at, std::string_view ck)
    {
        std::string     kk  = cdb::key(at);
        if(!ck.empty()){
            if(cdb::is_document(at)){
                kk += '#';
            } else {
                kk += '.';
            }
            kk += ck;
        }
        return kk;
    }

    std::string  Atom::sub_key(Atom at, std::string_view ck)
    {
        std::string     kk  = cdb::skey(at);
        if(!kk.empty())
            kk += '.';
        kk += ck;
        return kk;
    }

    void    reg_atom_meta()
    {
        {
            auto w  = writer<Atom>();
            w.property("children", (AtomVector(*)(Atom)) cdb::children);
            w.property("document", (Document(*)(Atom)) cdb::document);
            w.property("icon", (Image(*)(Atom)) cdb::icon);
            w.property("id", READ_ONLY, &Atom::id);
            w.property("key", (std::string(*)(Atom)) cdb::key);
            w.property("label", (std::string(*)(Atom)) cdb::label);
            w.property("leaf", (Leaf(*)(Atom)) cdb::leaf);
            w.property("name", (std::string(*)(Atom)) cdb::name);
            w.property("parent", (Atom(*)(Atom)) cdb::parent);
            w.property("properties", (Atom::PropertyVector(*)(Atom)) cdb::properties);
            w.property("skey", (std::string(*)(Atom)) cdb::skey);
            w.property("tags", (TagVector(*)(Atom)) cdb::tags);
            w.property("title", (std::string(*)(Atom)) cdb::title);
        }
        
        {
            auto w = writer<Atom::Property>();
            w.property("atom", (Atom(*)(Atom::Property)) cdb::atom);
            w.property("id", READ_ONLY, &Atom::Property::id);
            w.property("key", (std::string(*)(Atom::Property)) cdb::key);
            w.property("name", (std::string(*)(Atom::Property)) cdb::name);
            w.property("source", (Atom(*)(Atom::Property)) cdb::source);
            w.property("target", (Atom(*)(Atom::Property)) cdb::target);
        }
    }
    
    YQ_INVOKE([](){reg_atom_meta();})
}

