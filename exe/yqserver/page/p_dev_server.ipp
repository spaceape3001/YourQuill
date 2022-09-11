////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void page_dev_server(WebHtml&h)
    {
        h.title("Server Information");
        auto ta  = h.table();
        
        h.kvrow("Abbreviation") << wksp::abbreviation();
        h.kvrow("Author") << wksp::author();
        h.kvrow("Auxillary Ports") << join(wksp::aux_ports(), ", ");
        h.kvrow("Cache") << wksp::cache();
        h.kvrow("Dot") << wksp::dot();
        h.kvrow("GIT") << wksp::git();
        h.kvrow("Home") << wksp::home();
        h.kvrow("Host") << wksp::host();
        h.kvrow("Local User") << wksp::local_user();
        h.kvrow("Log Directory") << wksp::log_dir();
        h.kvrow("Markdown") << wksp::markdown();
        h.kvrow("Name") << wksp::name();
        h.kvrow("Perl") << wksp::perl();
        h.kvrow("Port") << wksp::port();
        h.kvrow("Quill Key") << wksp::quill_key();
        h.kvrow("Quill Path") << wksp::quill_file();
        h.kvrow("Read Timeout") << wksp::read_timeout();
        
        {
            auto r = h.kvrow("Roots");
            r << "<OL>";
            for(const Root* rt : wksp::roots())
                r << "<LI>[" << rt -> key << "]: " << rt->path;
            r << "</OL>";
        }
        
        h.kvrow("Smartypants") << wksp::smartypants();
        h.kvrow("Start") << wksp::start();
        
        h.kvrow("Temp Path") << wksp::temp_dir();
        
        {
            auto r = h.kvrow("Templates");
            r << "<OL>";
            for(const std::string& t : wksp::templates())
                r << "<LI>" << t;
            r << "</OL>";
        }
    }

    void    reg_dev_server()
    {
        reg_webpage<page_dev_server>("/dev/server").local();
    }
}
