////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <db/filesys_cdb.hpp>
#include <db/root.hpp>
#include <db/tag.hpp>
#include <db/user.hpp>
#include <db/workspace.hpp>

#include <db/html_common.hpp>


#include <basic/DelayInit.hpp>
//#include <yq/atom/Atom.hpp>
//#include <yq/atom/Class.hpp>
//#include <yq/atom/Field.hpp>
#include <kernel/bit/Copyright.hpp>
#include <kernel/enum/SubmitLabel.hpp>
//#include <yq/leaf/Leaf.hpp>
#include <basic/Logging.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>
#include <basic/Guarded.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebTemplate.hpp>

namespace yq {
    struct Atom;
    struct Class;
    struct Field;
    struct Leaf;
}


using namespace yq;
using namespace yq::html;



extern Guarded<std::string>     gTextColor, gBkColor;



template <typename T>
struct Icon {
    T                   src;
    std::string_view    alt;
    std::string_view    style;
};

template <typename T>
Icon<T> icon(T v, std::string_view a=std::string_view(), std::string_view s=std::string_view())
{
    return Icon{v, a, s};
}

WebAutoClose    form(WebHtml&, std::string_view action, bool inspect=false);
WebAutoClose    form(WebHtml&, std::string_view action, HttpOp, bool inspect=false);

namespace input {

    struct key {};
    struct edit_req {};
    struct root {
        DataRole dr;
    };

    struct check {
        std::string_view    key, label;
        bool                checked = false;
    };

    struct hidden {
        std::string_view    key, value;
    };

    struct label {
        std::string_view    key, label;
    };

    struct line {
        std::string_view    key, value;
        int                 size = -1;
    };
    
    struct text {
        std::string_view    key, value;
        int                 rows    = 10;
        int                 cols    = 80;
    };
    
    struct control_root {
    };
};


WebHtml&    operator<<(WebHtml&, SubmitLabel);
WebHtml&    operator<<(WebHtml&, input::key);
WebHtml&    operator<<(WebHtml&, input::edit_req);
WebHtml&    operator<<(WebHtml&, input::root);
WebHtml&    operator<<(WebHtml&, const input::check&);
WebHtml&    operator<<(WebHtml&, const input::hidden&);
WebHtml&    operator<<(WebHtml&, const input::label&);
WebHtml&    operator<<(WebHtml&, const input::line&);
WebHtml&    operator<<(WebHtml&, const input::text&);
WebHtml&    operator<<(WebHtml&, input::control_root);



enum {
    s3_tag1     = 0,
    s3_leaf,
    s3_user1,
    s3_user2,
    s3_tag2
};



