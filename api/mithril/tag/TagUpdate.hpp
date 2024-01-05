////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/document/Document.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>
#include <mithril/leaf/Leaf.hpp>
#include <mithril/tag/TagData.hpp>
#include <mithril/update/U.hpp>

namespace yq::mithril::update {
    struct UTag : public U<Tag>{
        static UTag&                    get(Tag t);
        static std::pair<UTag&, bool>   create(Document);

        static void             icons(Fragment,Change);
        static const FileSpec&  lookup();
        static void             notify(Fragment,Change);
        static void             s3(Document);
        static void             s3_leaf(Document);

        const Document      doc;
        Tag::SharedData     def;
        Image               icon;
        Leaf                leaf;

        UTag(Tag);
        
        void        reload();
        void        u_icon();
        void        u_info();
        void        u_leaf();
        void        x_erase();
    };
}
