#pragma once

#include "CoreFwd.hpp"
#include <cstdint>
#include <iosfwd>

class QChar;
class QSqlDatabase;
class QRect;
class QIcon;
class QImage;
class QString;

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
bool            db_run_script_file(const char*zFile, QSqlDatabase db);

/*! \brief Runs the specified script from the given filename

    See db_run_script for syntax.  This loads up the file, runs that.
    
    \param[in]  zFile   The file to load
    \param[in]  db      The database
    
    \return TRUE if the file exists and all statements executed successfully

*/
bool            db_run_script_file(const QString&zFile, QSqlDatabase db);

/*! \brief Checks to see if the specified table exists 

    \param[in]  zTable  The table to check for
    \param[in]  db      The database
    
    \return TRUE if the table exists in the specified database
*/
bool            db_table_exists(const String& zTable, QSqlDatabase db);

bool            file_backup(const char*, const char* suffix="bck");
QByteArray      file_bytes(const char*);
QByteArray      file_bytes(const std::string&);
QByteArray      file_bytes(const QString&);
bool            file_exists(const char*);
String          file_modified(const char*);
bool            file_readable(const char*);
size_t          file_size(const char*);

struct SizeTimestamp {
    size_t          size;
    uint64_t        time;
    uint64_t        nano;
    bool            exists;
    
    SizeTimestamp() : size{}, time{}, nano{}, exists(false) {}
    SizeTimestamp(size_t s, uint64_t t, uint64_t n) : size(s), time(t), nano(n), exists(true) {}
    uint64_t        nanoseconds() const { return 1'000'000'000*time + nano; }
};

/*! \brief Size and time for last modification

    This queries and returns the file size and timestamp to the last update.  Nothing's guaranteed to uint128_t except
    that it changes once a nanosecond.
*/
SizeTimestamp   file_size_and_timestamp(const char*);
SizeTimestamp   file_size_and_timestamp(const std::string&);
SizeTimestamp   file_size_and_timestamp(const QString&);
uint128_t       file_timestamp(const char*);

Vector<String>  dir_files(const char*);
Vector<String>  dir_directories(const char*);

/*! \brief Convience function for reading a file into a simple buffer

    \note This will return an empty vector if an error occurs or the file is zero size.
    \return File contents
*/
Vector<uint8_t> file_load_all(const char*);
Vector<uint8_t> file_load_all(const std::string&);
Vector<uint8_t> file_load_all(const QString&);

Vector<char> file_load_char(const char*);
Vector<char> file_load_char(const std::string&);
Vector<char> file_load_char(const QString&);


uint64_t        randU64();

/*! \brief Returns desktop parameters.

    \note, there can be GAPS due to relative screen geometry
*/
QRect           desktop_screen_box();

/*!
    Gets an icon from a filename.  Any %1 notation will be replaced by 
    common icon sizes and tested.
    
    \note Qt restriction requires this to run on the MAIN THREAD.  Any
    other thread will simply return an empty icon
*/
QIcon           fetch_icon(const QString&);


bool            is_main_thread();


QString         join(const Set<uint16_t>& values, const QString& sep);

QString         join(const QStringSet&, QChar);
QString         join(const QStringSet&, const QString&);

QByteArray      join(const Vector<QByteArray>&, char);
QByteArray      join(const Vector<QByteArray>&, const QByteArray&);

QString         join(const Vector<QString>&, char);
QString         join(const Vector<QString>&, const QString&);

String          join(const Vector<String>&, char);
String          join(const Vector<String>&, const String&);

QByteArray      make_png(const QImage&);

QString         sanitize_path(const QString&);
QStringList     sanitize_path(const QStringList&);

QString         strip_extension(const QString&);

QString         utf8(const QByteArray&);
QByteArray      utf8(const QString&);

std::ostream& operator<<(std::ostream&, const QString&);
