////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void  page_dev_wksp(WebHtml& h)
    {
        h.title() << "Workspace '" << wksp::name() << "'";
        
        auto t = h.table();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Color (Background)") << gBkColor.get();
        h.kvrow("Color (Text)") << gTextColor.get();
        //h.kvrow("Config") << dev(cdb::config_folder());
        h.kvrow("Copyright") << wksp::copyright().text;
        h.kvrow("Key") << wksp::quill_key();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        h.kvrow("Start") << wksp::start();
        h.kvrow("Templates") << join(wksp::templates(), ", ");
    }

    void    page_dev_wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::roots());
    }

    void    reg_dev_wksp()
    {
        reg_webgroup({
            reg_webpage<page_dev_wksp>("/dev/wksp").label("Info"),
            reg_webpage<page_dev_wksp_roots>("/dev/wksp/roots").label("Roots")
        });
    }
}
