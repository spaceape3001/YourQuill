////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <yq/text/chars.hpp>
#include <yq/text/misc.hpp>

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
    
    //  -----------------------------------------------------------------------
    //      ADMIN
    //  -----------------------------------------------------------------------

    //  -----------------------------------------------------------------------
    //      DEV
    //  -----------------------------------------------------------------------

        void p_dev_sql_table(WebHtml& h)
        {
            int i, cc, count;
            std::string table = h.context().find_query("table");
            if(table.empty())
                throw HttpStatus::BadArgument;
            
            auto& db    = wksp::db();
            if(!db.has_table(table))
                throw HttpStatus::NotFound;

            std::string cmd = "SELECT * FROM " + table;
            SqlQuery    sql(db, cmd);
            if(!sql.valid())
                throw HttpStatus::InternalError;

            h.title() << "Sql Table: " << table;
            
    //        bool    first   = false;
            
            auto _table = h.table();

            h << "<tr>";
            cc  = sql.column_count();
            for(i=1;i<=cc;++i)
                h << "<th>" << sql.column_name(i) << "</th>";
            h << "</tr>\n";
            
            count   = 0;
            SQResult    r;
            while((r = sql.step()) == SQResult::Row){
                ++ count;
                h << "<tr>";
                for(i=1;i<=cc;++i){
                    h << "<td>";
                    html_escape_write(h, sql.v_text(i));
                    h << "</td>";
                }
                h << "</tr>\n";
            }
            
            h << "</table>\n";
            h.paragraph() << "This table has " << count << " row(s).";
        }
        
        void p_dev_sql_tables(WebHtml& out)
        {
            auto list = out.numbers();
            for(auto& s : wksp::db().tables())
                out.li() << "<a href=\"/dev/sql/table?table=" << web_encode(s) << "\">" << s << "</a>\n";
        }

    //  -----------------------------------------------------------------------
    //      REGISTRATION
    //  -----------------------------------------------------------------------

        void reg_sql_pages()
        {
            reg_webpage<p_dev_sql_table>("/dev/sql/table").local().argument("table", "SQL Table Name");
            reg_webpage<p_dev_sql_tables>("/dev/sql/tables", "SQL Tables").local();
        }
}
