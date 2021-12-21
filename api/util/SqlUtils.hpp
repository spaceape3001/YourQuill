////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

class String;
class QString;
class QSqlDatabase;

/*! \brief Runs the specified script against the database

    This routine runs the specified script of semi-colon separated SQL statements.  
    Comments start on newlines with "--".

    \param[in] script   SQL script
    \param[in] db       the database
    
    \return TRUE if all statements executed successfully
*/
bool            db_run_script(const String&script, QSqlDatabase db);

/*! \brief Runs the specified script from the given filename

    See db_run_script for syntax.  This loads up the file, runs that.
    
    \param[in]  zFile   The file to load
    \param[in]  db      The database
    
    \return TRUE if the file exists and all statements executed successfully

*/
bool            db_run_script_file(const std::filesystem::path& file, QSqlDatabase db);


/*! \brief Checks to see if the specified table exists 

    \param[in]  zTable  The table to check for
    \param[in]  db      The database
    
    \return TRUE if the table exists in the specified database
*/
bool            db_table_exists(const String& zTable, QSqlDatabase db);
