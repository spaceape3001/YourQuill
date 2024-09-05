////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq-toolbox/meta/TypeInfoWriter.hpp>
#include "AtomCDB.hpp"
#include "AtomHtml.hpp"

namespace  {
    void    reg_atom_meta()
    {
        using namespace yq;
        using namespace yq::mithril;

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
}
