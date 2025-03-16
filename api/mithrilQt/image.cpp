////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "image.hpp"
#include <yq/core/ThreadId.hpp>
#include <gluon/core/IgCaseQ.hpp>
#include <mithril/fragment/FragmentCDB.hpp>
#include <mithril/image/ImageCDB.hpp>
#include <QIcon>
#include <unordered_map>

using namespace yq::gluon;

namespace yq::mithril {
    namespace {
        struct ImageIcon {
            QIcon       icon;
            uint64_t    modified    = 0ULL;
        };
    }

    QIcon       qIcon(Image img)
    {
        static std::unordered_map<uint64_t, ImageIcon>    s_icons;

        if(!is_main_thread())
            return QIcon();
        if(!img)
            return QIcon();
            
        Fragment frag   = cdb::fragment(img);
        if(!frag)
            return QIcon();
            
        uint64_t    m   = cdb::modified(frag);

        auto [i,f]  = s_icons.try_emplace((uint64_t) frag, ImageIcon{ QIcon(), m });
        ImageIcon& ii = i->second;
        if(!f && (ii.modified == m))
            return ii.icon;
        
        std::filesystem::path   p   = cdb::path(frag);
        if(p.empty())
            return QIcon();
        ii.icon = QIcon(QString::fromStdString(p.string()));
        return ii.icon;
    }
}
