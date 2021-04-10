////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once


Vector<String>  dir_files(const char* iDir)
{
    Vector<String>  ret;
    DIR*            d   = opendir(iDir);
    if(d){
        while(struct dirent* r = readdir(d)){
            if(r -> d_type != DT_REG)
                continue;
            ret << String(r->d_name);
        }
        closedir(d);
    }
    return ret;
}

Vector<String>  dir_directories(const char* iDir)
{
    Vector<String>  ret;
    DIR*            d   = opendir(iDir);
    if(d){
        while(struct dirent* r = readdir(d)){
            if(r -> d_type != DT_DIR)
                continue;
            if(r->d_name[0] == '.'){
                if(r->d_name[1] == '.' || r->d_name[1] == '\0')
                    continue;
            }
            ret << String(r->d_name);
        }
        closedir(d);
    }
    return ret;
}


bool        file_backup(const char* iFile, const char* suffix)
{
    if(!file_exists(iFile))
        return true;
    std::string basis  = std::string(iFile) + "." + file_modified(iFile);
    unsigned int        i   = 0;
    std::string check(basis + '.' + suffix);
    while(file_exists(check.c_str()))
        check   = basis + '+' + std::to_string(++i) + '.' + suffix;
    return !rename(iFile, check.c_str());
}

QByteArray      file_bytes(const char*z)
{
    Vector<uint8_t>     bytes   = file_load_all(z);
    return QByteArray((const char*) bytes.data(), bytes.size());
}

QByteArray      file_bytes(const std::string&z)
{
    return file_bytes(z.c_str());
}

QByteArray      file_bytes(const QString&z)
{   
    return file_bytes(z.toStdString());
}

bool        file_exists(const char* iFile)
{
    return ::access(iFile, F_OK) == 0;
}



Vector<uint8_t> file_load_all(const char*iFile)
{
    struct stat buf;
    memset(&buf, 0, sizeof(buf));
    if(::stat(iFile, &buf) == -1)
        return Vector<uint8_t>();
    size_t      sz  = buf.st_size;
    int fd  = open(iFile, O_RDONLY);
    if(fd == -1)
        return Vector<uint8_t>();
    Vector<uint8_t> ret;
    try {
        ret.resize(sz);
    } catch(const std::bad_alloc&){
        return Vector<uint8_t>();
    }
    ssize_t sz2 = read(fd, ret.data(), sz);
    close(fd);
    if(sz2 < 0)
        return Vector<uint8_t>();
    if(sz2 < (ssize_t) sz)
        ret.resize(sz2);
    return ret;
}

Vector<uint8_t> file_load_all(const std::string&s)
{
    return file_load_all(s.c_str());
}

Vector<uint8_t> file_load_all(const QString&s)
{
    return file_load_all(s.toStdString());
}

Vector<char> file_load_char(const char*iFile)
{
    struct stat buf;
    memset(&buf, 0, sizeof(buf));
    if(::stat(iFile, &buf) == -1)
        return Vector<char>();
    size_t      sz  = buf.st_size;
    int fd  = open(iFile, O_RDONLY);
    if(fd == -1)
        return Vector<char>();
    Vector<char> ret;
    try {
        ret.resize(sz);
    } catch(const std::bad_alloc&){
        return Vector<char>();
    }
    ssize_t sz2 = read(fd, ret.data(), sz);
    close(fd);
    if(sz2 < 0)
        return Vector<char>();
    if(sz2 < (ssize_t) sz)
        ret.resize(sz2,'\0');
    return ret;
}

Vector<char> file_load_char(const std::string&s)
{
    return file_load_char(s.c_str());
}

Vector<char> file_load_char(const QString&s)
{
    return file_load_char(s.toStdString());
}

String  file_modified(const char* iFile)
{
    struct stat buf;
    if(::stat(iFile, &buf) != 0)
        return String();
        
    struct tm  mt;
    localtime_r(&buf.st_mtim.tv_sec, &mt);
    char        thetime[256];
    sprintf(thetime, "%04d%02d%02d-%02d%02d%02d.%09ld", 
        1900+mt.tm_year, 1+mt.tm_mon, 1+mt.tm_mday, 
        mt.tm_hour, mt.tm_min, mt.tm_sec, buf.st_mtim.tv_nsec
    );
    return thetime;
}


bool        file_readable(const char*iFile)
{
    return ::access(iFile, F_OK | R_OK) != 0;
}


size_t      file_size(const char* iFile)
{
    struct stat buf;
    memset(&buf, 0, sizeof(buf));
    if(::stat(iFile, &buf) == -1)
        return size_r();
    return buf.st_size;
}

SizeTimestamp file_size_and_timestamp(const char* iFile)
{
    struct stat buf;
    if(::stat(iFile, &buf) != 0)
        return SizeTimestamp();
    return SizeTimestamp( buf.st_size, buf.st_mtim.tv_sec, buf.st_mtim.tv_nsec );
}

SizeTimestamp   file_size_and_timestamp(const std::string&z)
{
    return file_size_and_timestamp(z.c_str());
}

SizeTimestamp   file_size_and_timestamp(const QString&z)
{
    return file_size_and_timestamp(z.toStdString());
}

