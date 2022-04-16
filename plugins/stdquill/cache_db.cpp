////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "stdquill.hpp"

#include <yq/sql/SqlLite.hpp>
#include <yq/sql/SqlQuery.hpp>

using namespace yq;

namespace {
    void page_tables(WebHtml& out)
    {
        auto list = out.numbers();
        for(auto& s : wksp::db().tables())
            out.li() << "<a href=\"table?table=" << web_encode(s) << "\">" << s << "</a>\n";
    }

    void page_table(WebHtml& out)
    {
        std::string table = out.context().find_query("table");
        if(table.empty())
            throw HttpStatus::NotAcceptable;
        
        auto& db    = wksp::db();
        if(!db.has_table(table))
            throw HttpStatus::NotFound;

        std::string cmd = "SELECT * FROM " + table;
        SqlQuery    sql(db, cmd);
        if(!sql.valid())
            throw HttpStatus::InternalError;

        out.title() << "Sql Table: " << table;
        
//        bool    first   = false;
        
        auto _table = out.table();
    
    }
}

YQ_INVOKE(
    reg_webpage<page_table>("/dev/sql/table").local().argument("table", "SQL Table Name");
    reg_webpage<page_tables>("/dev/sql/tables", "SQL Tables").local();
)
