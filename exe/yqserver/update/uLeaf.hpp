////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <update/preamble.hpp>
#include <mithril/atom/Atom.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/LeafData.hpp>
#include <mithril/tag/Tag.hpp>

namespace yq::mithril::update {
    struct ULeaf : public U<Leaf> {
        static ULeaf&                   get(Leaf);
        static std::pair<ULeaf&,bool>   create(Document);

        static const FileSpec&  lookup();
        static void             notify(Fragment,Change);
        static void             icons(Fragment,Change);
        
        //! Create the leaf & base atom
        static void             s3(Document);

        const Document      doc;
        const Atom          atom;
        Image               icon;
        Leaf::SharedData    def;        // this will *NOT* have context (at least to start)
        TagSet              tags;

        ULeaf(Leaf);
        
        void        reload();
        
        void        u_icon();
        void        u_info();
        void        u_tags();
        void        x_erase();
        
        
        //void                flash(FF);
    };
}
