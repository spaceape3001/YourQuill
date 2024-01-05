////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/update/preamble.hpp>
#include <mithril/update/U.hpp>
#include <mithril/category/CategoryData.hpp>
#include <mithril/document/Document.hpp>
#include <mithril/enum/Change.hpp>
#include <mithril/image/Image.hpp>

namespace yq::mithril::update {
    struct UCategory : public U<Category>{
        static UCategory&                   get(Category t);
        static std::pair<UCategory&, bool>  create(Document);
        static const FileSpec&  lookup();

        const Document          doc;
        Image                   icon;
        Category::SharedData    def;

        UCategory(Category);
        
        static void             notify(Fragment,Change);
        static void             icons(Fragment,Change);
        static void             s3(Document);
        
        void                    reload();
        
        //! Update the icon
        void                    u_icon();

        //! Update the info
        void                    u_info();

        //! Erases from the cache
        void                    x_erase();
    };
}
