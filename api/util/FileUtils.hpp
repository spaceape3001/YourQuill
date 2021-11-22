////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "UInt128.hpp"
#include "SetFwd.hpp"
#include "VectorFwd.hpp"

#include <stdint.h>
#include <iosfwd>
#include <filesystem>

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

//  Make the path (including the specified filename)
bool            make_path(const std::filesystem::path&, mode_t dirMode=0755);

//  Make the path (not including the specified filename)
bool            make_parent_path(const std::filesystem::path&, mode_t dirMode=0755);


namespace yqdir {
    
    enum Options {
        //  No extensions
        NO_EXT      = 0x1,
        
        //  Allow hidden
        HIDDEN      = 0x2,
        
        //  Allow dot & dot-dot
        DOT_OR_DDOT = 0x4,
        
        NO_DIRS     = 0x8,
        NO_FILES    = 0x10
    };

    using   path        = std::filesystem::path;
    using   path_vec    = Vector<path>;
    
    path_vec        all_children(const path& dir, const std::string_view& name);
    path_vec        all_children(const std::vector<path>& dir, const std::string_view& name);
    path            first_child(const path& dir, const std::string_view& name);
    path            first_child(const std::vector<path>&, const std::string_view& name);
    
    //  Finds all chlidren (not hidden)
    Vector<String>  subdirectory_names(const path&, unsigned options=0) ;
    Vector<String>  subdirectory_names(const std::vector<path>&, unsigned options=0) ;

    //  Finds all chlidren (not hidden)
    StringSet       subdirectory_names_set(const path&, unsigned options=0) ;
    StringSet       subdirectory_names_set(const std::vector<path>&, unsigned options=0) ;

    //  Finds all chlidren (not hidden)
    path_vec        subdirectories(const path&, unsigned options=0);
    path_vec        subdirectories(const std::vector<path>&, unsigned options=0);
    
    bool            child_exists(const path&, const std::string_view& name);
    bool            child_exists(const std::vector<path>&, const std::string_view& name);
    
    template <typename P>
    auto    for_all_children(const std::filesystem::path& dir, unsigned options, P pred)
    {
        using result_t  = decltype(pred({}));
        for(auto const& de : std::filesystem::directory_iterator(dir)){
            auto p  = de.path();
            if((options & yqdir::NO_EXT) && !p.extension().empty())
                continue;

            auto f  = p.filename().string();
            if((!(options & yqdir::DOT_OR_DDOT)) && ((f == ".") || (f == "..")))
                continue;

            if((!(options & yqdir::HIDDEN)) && f.c_str()[0] == '.')
                continue;

            if((options & yqdir::NO_DIRS) && std::filesystem::is_directory(p))
                continue;

            if((options & yqdir::NO_FILES) && !std::filesystem::is_directory(p))
                continue;
            
            if constexpr (std::is_same_v<result_t,void>){
                pred(p);
            } else {
                result_t    r = pred(p);
                if(r != result_t{})
                    return r;
            }
        }
        
       
        if constexpr (!std::is_same_v<result_t,void>)
            return result_t{};
        else
            return;
    }

    template <typename P>
    auto    for_all_children(const std::vector<std::filesystem::path>& dirs, unsigned options, P pred)
    {
        using result_t  = decltype(pred({}));
        for(const std::filesystem::path& dir : dirs){
            if constexpr (std::is_same_v<result_t,void>){
                for_all_children(dir, options, pred);
            } else {
                result_t    r   = for_all_children(dir, options, pred);
                if(r != result_t{})
                    return r;
            }
        }
       
        if constexpr (!std::is_same_v<result_t,void>)
            return result_t{};
        else
            return;
    }
}
