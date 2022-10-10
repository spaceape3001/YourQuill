////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <kernel/db/SQ.hpp>
#include <kernel/file/Root.hpp>
#include <kernel/image/Image.hpp>

namespace yq {
    namespace update {
        void    set_root_icon(const Root*rt, Image img)
        {
            if(!rt)
                return ;
                
            static thread_local cdb::SQ r("REPLACE INTO RootIcons (root, icon) VALUES (?, ?)");
            auto r_af = r.af();
            r.bind(1, rt->id);
            r.bind(2, img.id);
            r.step();
        }
    }
}
