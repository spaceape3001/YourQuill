////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////


#include "SqlLogging.hpp"
#include "FileUtils.hpp"
#include "SqlUtils.hpp"

#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>

bool            db_run_script(const String&data, QSqlDatabase db)
{
    Vector<String>  lines;
    for(const String& s : data.split('\n')){
        if(is_blank(s))
            continue;
        if(s.trimmed().starts_with("--"))
            continue;
        lines << s;
    }
    
    lines   = String(' ').join(lines).split(';');
    bool    success = true;
    for(String s : lines){
        s = s.trimmed();
        if(s.empty())
            continue;
        QSqlQuery q(db);
        if(!q.exec(s.qString())){
            dbError << "SQL failure: " << q.lastError().text() << "\n" << s;
            success = false;
        }
    }
    return success;
}

bool            db_run_script_file(const char*file, QSqlDatabase db)
{
    return db_run_script(String(file_load_all(file)), db);
}

bool            db_run_script_file(const QString&str, QSqlDatabase db)
{
    std::string     f   = str.toStdString();
    return db_run_script(String(file_load_all(f.c_str())), db);
}


bool            db_table_exists(const String&tn, QSqlDatabase db)
{
    QString q  = tn.qString();
    for(QString t : db.tables())
        if(is_similar(t, q))
            return true;
    return false;
}

