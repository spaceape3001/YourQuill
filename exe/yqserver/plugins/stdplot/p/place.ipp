////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {

    //  -----------------------------------------------------------------------
    //      UTILITIES
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      REST API
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      PAGES
    //  -----------------------------------------------------------------------

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
        
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void    p_dev_places(WebHtml& h)
        {
            h.title() << "All Places";
            dev_table(h, all_places(Sorted::YES));
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_place_pages()
        {
            reg_webpage<p_places>("/places");
            
            reg_webpage<p_dev_places>("/dev/places");
        }
}
