////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace {
    void page_dev_sql_tables(WebHtml& out)
    {
        auto list = out.numbers();
        for(auto& s : wksp::db().tables())
            out.li() << "<a href=\"/dev/sql/table?table=" << web_encode(s) << "\">" << s << "</a>\n";
    }
    
    YQ_INVOKE( reg_webpage<page_dev_sql_tables>("/dev/sql/tables", "SQL Tables").local(); )
}

