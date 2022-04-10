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
        auto list = html::numbers(out);
        for(auto& s : wksp::db().tables()){
            out << "<li><a href=\"table?table=" << s << "\">" << s << "</a>\n";
        }
    }

    void page_table(WebHtml& out)
    {
        StringMultiMap  args    = out.context().decode_query();
        std::string table  = args.first("table");
        if(table.empty())
            throw HttpStatus::NotAcceptable;
        
        auto& db    = wksp::db();
        if(!db.has_table(table))
            throw HttpStatus::NotFound;

        std::string cmd = "SELECT * FROM " + table;
        SqlQuery    sql(db, cmd);
        if(!sql.valid())
            throw HttpStatus::InternalError;

        std::string title   = "Sql Table: " + table;
        out.title(table);
        
//        bool    first   = false;
        
        auto _table = html::table(out);
    
    }
}

YQ_INVOKE(
    reg_webpage<page_table>("/dev/sql/table").local().argument("table", "SQL Table Name");
    reg_webpage<page_tables>("/dev/sql/tables", "SQL Tables").local();
)
