////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/app/DelayInit.hpp>
#include <yq/atom/Atom.hpp>
#include <yq/atom/Class.hpp>
#include <yq/atom/Field.hpp>
#include <yq/bit/Copyright.hpp>
#include <yq/enum/SubmitLabel.hpp>
#include <yq/file/file_cdb.hpp>
#include <yq/image/Image.hpp>
#include <yq/leaf/Leaf.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <db/tag.hpp>
#include <yq/text/text_utils.hpp>
#include <yq/type/Guarded.hpp>
#include <db/user.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebTemplate.hpp>
#include <yq/wksp/Root.hpp>
#include <yq/wksp/Workspace.hpp>


using namespace yq;



extern Guarded<std::string>     gTextColor, gBkColor;

template <typename T>
struct Dev {
    T       data;
};

template <typename T>
Dev<T> dev(T d) 
{ 
    return Dev<T>{ d }; 
}


template <typename T>
struct DevID {
    T       data;
};

template <typename T>
DevID<T> dev_id(T d) 
{ 
    return DevID<T>{ d }; 
}

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

WebHtml&    operator<<(WebHtml&, const Dev<Atom>&);
WebHtml&    operator<<(WebHtml&, const Dev<Class>&);
WebHtml&    operator<<(WebHtml&, const Dev<Directory>&);
WebHtml&    operator<<(WebHtml&, const Dev<Document>&);
WebHtml&    operator<<(WebHtml&, const Dev<Field>&);
WebHtml&    operator<<(WebHtml&, const Dev<Folder>&);
WebHtml&    operator<<(WebHtml&, const Dev<Fragment>&);
WebHtml&    operator<<(WebHtml&, const Dev<Leaf>&);
WebHtml&    operator<<(WebHtml&, const Dev<Tag>&);
WebHtml&    operator<<(WebHtml&, const Dev<User>&);
WebHtml&    operator<<(WebHtml&, const Dev<const Root*>&);

WebHtml&    operator<<(WebHtml&, const Atom&);
WebHtml&    operator<<(WebHtml&, const Class&);
WebHtml&    operator<<(WebHtml&, const Document&);
WebHtml&    operator<<(WebHtml&, const Folder&);
WebHtml&    operator<<(WebHtml&, const Leaf&);
WebHtml&    operator<<(WebHtml&, const Tag&);
WebHtml&    operator<<(WebHtml&, const User&);

WebHtml&    operator<<(WebHtml&, const DevID<Atom>&);
WebHtml&    operator<<(WebHtml&, const DevID<Class>&);
WebHtml&    operator<<(WebHtml&, const DevID<Directory>&);
WebHtml&    operator<<(WebHtml&, const DevID<Document>&);
WebHtml&    operator<<(WebHtml&, const DevID<Field>&);
WebHtml&    operator<<(WebHtml&, const DevID<Folder>&);
WebHtml&    operator<<(WebHtml&, const DevID<Fragment>&);
WebHtml&    operator<<(WebHtml&, const DevID<Leaf>&);
WebHtml&    operator<<(WebHtml&, const DevID<Tag>&);
WebHtml&    operator<<(WebHtml&, const DevID<User>&);
WebHtml&    operator<<(WebHtml&, const DevID<const Root*>&);

void        dev_table(WebHtml&, const std::vector<Atom>&);
void        dev_table(WebHtml&, const std::vector<Class>&);
void        dev_table(WebHtml&, const std::vector<Directory>&);
void        dev_table(WebHtml&, const std::vector<Document>&);
void        dev_table(WebHtml&, const std::vector<Field>&);
void        dev_table(WebHtml&, const std::vector<Folder>&);
void        dev_table(WebHtml&, const std::vector<Fragment>&);
void        dev_table(WebHtml&, const std::vector<Leaf>&);
void        dev_table(WebHtml&, const std::vector<Tag>&);
void        dev_table(WebHtml&, const std::vector<User>&);
void        dev_table(WebHtml&, const std::vector<const Root*>&);



enum {
    s3_tag1     = 0,
    s3_leaf,
    s3_user1,
    s3_user2,
    s3_tag2
};



