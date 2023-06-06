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

#include <mithril/book.hpp>
#include <mithril/character.hpp>
#include <mithril/group.hpp>
#include <mithril/event.hpp>
#include <mithril/place.hpp>
#include <mithril/thing.hpp>

using namespace yq;
//using namespace yq::arg;
using namespace yq::mithril;
using namespace yq::mithril::cdb;
using namespace yq::mithril::html;

#include "p_dev.ipp"

namespace {
    void p_books(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Book>  books   = all_books(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Books";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, books);
    }

    void    p_characters(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Character>  chars   = all_characters(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Characters";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, chars);
    }

    void p_events(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Event>  events   = all_events(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Events";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, events);
    }

    void p_groups(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Group>  groups   = all_groups(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Groups";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, groups);
    }

    void p_places(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Place>  places   = all_places(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Places";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, places);
    }

    void p_things(WebHtml& h)
    {
        //  Search parameters go here... execute the search
        std::vector<Thing>  things   = all_things(Sorted::YES);
        
        
        //  Form the title... 
        h.title() << "Things";
        
        h << "<p><i>(Search Bar will go here)</i></p>\n";
        
        //  And results
        
        
        
        dev_table(h, things);
    }

    void    reg_me()
    {
        reg_webpage<p_books>("/books");
        reg_webpage<p_characters>("/characters");
        reg_webpage<p_events>("/events");
        reg_webpage<p_groups>("/groups");
        reg_webpage<p_places>("/places");
        reg_webpage<p_things>("/things");

        reg_dev();

    
        yInfo() << "Standard plot pages registered";
    }
    
    YQ_INVOKE(reg_me();)
}

