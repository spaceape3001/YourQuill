////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <mithril/event/EventJson.hpp>
#include <mithril/event/EventSearch.hpp>

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

        json p_api_event(WebContext& ctx)
        {
            Event    v   = arg::event(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            return json_(v);
        }
        
        json p_api_event_key(WebContext& ctx)
        {
            Event    v   = arg::event(ctx);
            if(!v)
                throw HttpStatus::BadArgument;
            json j{
                { "key", cdb::key(v) }
            };
            return j;
        }
        
        json p_api_events(WebContext& ctx)
        {
            EventVector  ret = search(ctx, EVENT);
            return json{
                { "events", json_(ret) }
            };
        }
        
    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------
        
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

    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_events(WebHtml& h)
        {
            h.title() << "All Events";
            dev_table(h, all_events(Sorted::YES));
        }


    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_event_pages()
        {
            reg_webpage<p_api_event>("/api/event").argument("ID", "Event ID");
            reg_webpage<p_api_event_key>("/api/event/key").argument("ID", "Event ID");
            reg_webpage<p_api_events>("/api/events");

            reg_webpage<p_events>("/events");
            
            reg_webpage<p_dev_events>("/dev/events");
        }
}
