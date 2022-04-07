////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <yq/app/DelayInit.hpp>
#include <yq/bit/Copyright.hpp>
#include <yq/file/cdb.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/Utils.hpp>
#include <yq/type/Guarded.hpp>
#include <yq/web/HttpDataStream.hpp>
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


WebHtml&    operator<<(WebHtml&, const Dev<Folder>&);
WebHtml&    operator<<(WebHtml&, const Dev<Fragment>&);
WebHtml&    operator<<(WebHtml&, const Dev<Document>&);
WebHtml&    operator<<(WebHtml&, const Dev<Directory>&);
WebHtml&    operator<<(WebHtml&, const Dev<const Root*>&);

WebHtml&    operator<<(WebHtml&, const DevID<Folder>&);
WebHtml&    operator<<(WebHtml&, const DevID<Fragment>&);
WebHtml&    operator<<(WebHtml&, const DevID<Document>&);
WebHtml&    operator<<(WebHtml&, const DevID<Directory>&);
WebHtml&    operator<<(WebHtml&, const DevID<const Root*>&);
