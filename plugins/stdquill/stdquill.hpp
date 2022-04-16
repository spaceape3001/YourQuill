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
#include <yq/file/cdb.hpp>
#include <yq/image/Image.hpp>
#include <yq/leaf/Leaf.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/tag/Tag.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/Guarded.hpp>
#include <yq/user/User.hpp>
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



