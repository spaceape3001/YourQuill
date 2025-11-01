////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/mithril/image/Image.hpp>
#include <yq/mithril/root/RootDir.hpp>
#include <yq/mithril/wksp/CacheQuery.hpp>

using namespace yq;
using namespace yq::mithril;

namespace {
#if 0
    void    u_set_root_icon(const RootDir*rt, Image img)
    {
        if(!rt)
            return ;
            
        static thread_local CacheQuery r("REPLACE INTO RootIcons (root_dir, icon) VALUES (?, ?)");
        auto r_af = r.af();
        r.bind(1, rt->id);
        r.bind(2, img.id);
        r.step();
    }
#endif
}


