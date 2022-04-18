////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <db/workspace.hpp>

#include <yq/app/DelayInit.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/web/WebAdapters.hpp>
#include <yq/web/WebHtml.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebVariable.hpp>

using namespace yq;

#include "var/abbr.ipp"
#include "var/author.ipp"
#include "var/body.ipp"
#include "var/can_edit.ipp"
#include "var/footer.ipp"
#include "var/home.ipp"
#include "var/host.ipp"
#include "var/local.ipp"
#include "var/logged_in.ipp"
#include "var/name.ipp"
#include "var/port.ipp"
#include "var/scripts.ipp"
#include "var/ssid.ipp"
#include "var/summary.ipp"
#include "var/tabbar.ipp"
#include "var/time.ipp"
#include "var/title.ipp"
#include "var/user.ipp"
#include "var/year.ipp"

