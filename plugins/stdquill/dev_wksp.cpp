////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

using namespace yq;
using namespace std::literals::string_literals;

namespace {
    void    dev_table(WebHtml& h, const root_vector_t& roots)
    {
        auto t = html::table(h);
        h << "<tr><th><ID></th><th>Key</th><th>Name</th><th>Path</th></tr>\n";
        for(const Root* r : roots){
            if(!r)
                continue;
            h << "<tr><td><a href=\"/dev/root?id=" << r->id << "\">" << r->id << "</a></td><td>" << r->key 
                << "</td><td>" << r->name << "</td><td>" << r->path << "</td></tr>\n";
        }
    }

    void    wksp_roots(WebHtml&h)
    {
        h.title("Workspace Roots");
        dev_table(h, wksp::roots());
    }

   void  wksp_info(WebHtml& h)
   {
        h.title("Workspace '"s + copy(wksp::name()) + "'"s);
        
        auto t = html::table(h);
        html::kvrow(h, "Name") << wksp::name();
        html::kvrow(h, "Author") << wksp::author();
        html::kvrow(h, "Abbreviation") << wksp::abbreviation();
        html::kvrow(h, "Cache") << wksp::cache();
        //html::kvrow(h, "Config") << dev(cdb::config_folder());
        html::kvrow(h, "Copyright") << wksp::copyright().text;
        html::kvrow(h, "Key") << wksp::quill_key();
        html::kvrow(h, "Host") << wksp::host();
        html::kvrow(h, "Port") << wksp::port();
        html::kvrow(h, "Read Timeout") << wksp::read_timeout();
        html::kvrow(h, "Start") << wksp::start();
        html::kvrow(h, "Templates") << join(wksp::templates(), ", ");
   }
     

}

YQ_INVOKE(
    reg_webpage<wksp_info>("/dev/wksp/info");
    reg_webpage<wksp_roots>("/dev/wksp/roots");
)
