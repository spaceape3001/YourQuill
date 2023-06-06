////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    void    p_dev_books(WebHtml& h)
    {
        h.title() << "All Books";
        dev_table(h, all_books(Sorted::YES));
    }
    
    void    p_dev_characters(WebHtml& h)
    {
        h.title() << "All Characters";
        dev_table(h, all_characters(Sorted::YES));
    }

    void    p_dev_events(WebHtml& h)
    {
        h.title() << "All Events";
        dev_table(h, all_events(Sorted::YES));
    }

    void    p_dev_groups(WebHtml& h)
    {
        h.title() << "All Groups";
        dev_table(h, all_groups(Sorted::YES));
    }

    void    p_dev_organizations(WebHtml& h)
    {
        h.title() << "All Organizations";
        h << "(still under construction)";
    }

    void    p_dev_places(WebHtml& h)
    {
        h.title() << "All Places";
        dev_table(h, all_places(Sorted::YES));
    }
    
    void    p_dev_things(WebHtml& h)
    {
        h.title() << "All Things";
        dev_table(h, all_things(Sorted::YES));
    }
    
    void reg_dev()
    {
        reg_webpage<p_dev_books>("/dev/books");
        reg_webpage<p_dev_characters>("/dev/characters");
        reg_webpage<p_dev_events>("/dev/events");
        reg_webpage<p_dev_groups>("/dev/groups");
        reg_webpage<p_dev_organizations>("/dev/organizations");
        reg_webpage<p_dev_places>("/dev/places");
        reg_webpage<p_dev_things>("/dev/things");
    }
}
