////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UInt128.hpp"
#include "VectorFwd.hpp"

#include <stdint.h>
#include <iosfwd>

class QByteArray;
class QString;
class String;

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
