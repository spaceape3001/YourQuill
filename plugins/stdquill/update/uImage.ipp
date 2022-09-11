////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "uImage.hpp"

namespace yq {
    void                    update_root(const Root*rt, Image img)
    {
        if(!rt)
            return ;
            
        static thread_local SQ r("REPLACE INTO RootIcons (root, icon) VALUES (?, ?)");
        auto r_af = r.af();
        r.bind(1, rt->id);
        r.bind(2, img.id);
        r.step();
    }
}
