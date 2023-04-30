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

#include <mithril/html/HtmlLayout.hpp>

#include <mithril/web/WebAdapters.hpp>
#include <mithril/web/WebContext.hpp>
#include <mithril/web/WebHtml.hpp>
#include <mithril/web/WebImage.hpp>
#include <mithril/web/WebPage.hpp>
#include <mithril/web/WebRedirect.hpp>
#include <mithril/web/WebTemplate.hpp>
#include <mithril/web/Template.hpp>

#include <mithril/character/CharacterCDB.hpp>
#include <mithril/character/CharacterHtml.hpp>

using namespace yq;
//using namespace yq::arg;
using namespace yq::mithril;
using namespace yq::mithril::cdb;
using namespace yq::mithril::html;

namespace {
    void    p_characters(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Character>  chars   = all_characters(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Characters";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, all_characters(Sorted::YES));
    }

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
        reg_webpage<p_characters>("/characters");

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

