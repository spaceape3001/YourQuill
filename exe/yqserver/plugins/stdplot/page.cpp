////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include <basic/CollectionUtils.hpp>
#include <basic/DelayInit.hpp>
#include <basic/DirUtils.hpp>
#include <basic/FileUtils.hpp>
#include <basic/Logging.hpp>
#include <basic/StreamOps.hpp>
#include <basic/TextUtils.hpp>

#include <basic/stream/Text.hpp>

#include <http/HtmlLayout.hpp>

#include <http/web/WebAdapters.hpp>
#include <http/web/WebContext.hpp>
#include <http/web/WebHtml.hpp>
#include <http/web/WebImage.hpp>
#include <http/web/WebPage.hpp>
#include <http/web/WebRedirect.hpp>
#include <http/web/WebTemplate.hpp>
#include <http/web/Template.hpp>

using namespace yq;

namespace {
    void    p_dev_books(WebHtml& h)
    {
        h.title() << "All Books";
        h << "(still under construction)";
    }
    
    void    p_dev_characters(WebHtml& h)
    {
        h.title() << "All Characters";
        h << "(still under construction)";
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

