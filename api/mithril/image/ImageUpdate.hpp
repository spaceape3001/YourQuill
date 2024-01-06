////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/update/U.hpp>
#include <mithril/update/preamble.hpp>
#include <mithril/fragment/Fragment.hpp>
#include <mithril/image/Image.hpp>

namespace yq::mithril::update {
    class UImage : public U<Image> {
    public:
        static UImage   get(Image);
        static UImage   get(Fragment);
        
        static void     s2();
        static void     s3(Document);
        static void     notify(Fragment,Change);
    
        UImage(Image);

        const Fragment  frag;
        
        void    u_thumbnail(FF flags={});
        void    x_erase();
    };
}
