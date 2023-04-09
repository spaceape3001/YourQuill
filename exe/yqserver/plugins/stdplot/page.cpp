////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/CollectionUtils.hpp>
#include <basic/DelayInit.hpp>
#include <io/DirUtils.hpp>
#include <io/FileUtils.hpp>
#include <basic/Logging.hpp>
#include <io/StreamOps.hpp>
#include <basic/TextUtils.hpp>

#include <io/stream/Text.hpp>

#include <mithril/http/HtmlLayout.hpp>

#include <mithril/http/web/WebAdapters.hpp>
#include <mithril/http/web/WebContext.hpp>
#include <mithril/http/web/WebHtml.hpp>
#include <mithril/http/web/WebImage.hpp>
#include <mithril/http/web/WebPage.hpp>
#include <mithril/http/web/WebRedirect.hpp>
#include <mithril/http/web/WebTemplate.hpp>
#include <mithril/http/web/Template.hpp>

#include <mithril/kernel/plot/CharacterCDB.hpp>
#include <mithril/http/plot/CharacterHtml.hpp>

using namespace yq;
//using namespace yq::arg;
using namespace yq::cdb;
using namespace yq::html;

namespace {
    void    p_dev_books(WebHtml& h)
    {
        h.title() << "All Books";
        h << "(still under construction)";
    }
    
    void    p_dev_characters(WebHtml& h)
    {
        h.title() << "All Characters";
        dev_table(h, all_characters(Sorted::YES));
    }

    void    p_dev_events(WebHtml& h)
    {
        h.title() << "All Events";
        h << "(still under construction)";
    }

    void    p_dev_organizations(WebHtml& h)
    {
        h.title() << "All Organizations";
        h << "(still under construction)";
    }

    void    p_dev_places(WebHtml& h)
    {
        h.title() << "All Places";
        h << "(still under construction)";
    }
    
    void    p_dev_things(WebHtml& h)
    {
        h.title() << "All Things";
        h << "(still under construction)";
    }

    void    reg_me()
    {
        reg_webpage<p_dev_books>("/dev/books");
        reg_webpage<p_dev_characters>("/dev/characters");
        reg_webpage<p_dev_events>("/dev/events");
        reg_webpage<p_dev_organizations>("/dev/organizations");
        reg_webpage<p_dev_places>("/dev/places");
        reg_webpage<p_dev_things>("/dev/things");
    
        yInfo() << "Standard plot pages registered";
    }
    
    YQ_INVOKE(reg_me();)
}

