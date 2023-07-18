////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/leaf/LeafData.hpp>

namespace yq::mithril::update {
    struct ULeaf : public U<Leaf> {
        static ULeaf       get(Leaf);
        
        const Document      doc;
        Leaf::SharedData    def;

        ULeaf(Leaf);
        //void                flash(FF);
    };
}
