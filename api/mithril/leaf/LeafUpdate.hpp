////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/atom/Atom.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/LeafData.hpp>
#include <mithril/tag/Tag.hpp>
#include <mithril/update/U.hpp>

namespace yq::mithril::update {
    class ULeaf {
    public:
        //static ULeaf&                   get(Leaf);
        //static std::pair<ULeaf&,bool>   create(Document);

        static const FileSpec&  lookup();
        static void             notify(Fragment,Change);
        static void             icons(Fragment,Change);
        
        //! Create the leaf & base atom
        static void             s3(Document);

        static void             u_icon(Leaf);
        static void             u_icon(Leaf, Image);
        static void             x_erase(Leaf);


#if 0
        const Leaf          x;
        const uint64_t      id;
        const Document      doc;
        const Atom          atom;
        Leaf::SharedData    def;        // this will *NOT* have context (at least to start)
        
        ULeaf(Leaf);
    

        void        reload();
#endif

        static void u_info(Leaf, const Leaf::Data&);
        static void u_tags(Leaf, const Leaf::Data&);
        static Atom i_atom(Leaf);
        static Leaf::SharedData reload(Leaf);


#if 0
        const Document      doc;
        const Atom          atom;
        Image               icon;
        TagSet              tags;
        
        
        void        i_atom();
        
        void        u_info();
        void        u_tags();
        void        u_atom();
#endif
        
        //void                flash(FF);
    };
}
