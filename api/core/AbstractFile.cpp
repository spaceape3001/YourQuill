////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AbstractFile.hpp"
#include "Logging.hpp"
#include "Utilities.hpp"
#include "Variant.hpp"
#include "Vector.hpp"
#include "XmlFile.hpp"
#include "XmlUtils.hpp"

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

#include <QByteArray>
#include <QString>

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>

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


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

AbstractFile::AbstractFile()
{
}

AbstractFile::~AbstractFile()
{
}

void            AbstractFile::clear()
{
    reset();
    m_file.clear();
}

bool            AbstractFile::load(const std::filesystem::path& ipath)
{
    if(!read_enabled())
        return false;
    
    //if(load_it(ipath)){
        //m_file  = ipath;
        //return true;
    //} else
        //return false;

    std::ios_base::openmode om  = std::ios_base::in;
    if(is_binary())
        om |= std::ios_base::binary;
    std::ifstream   fin(ipath, om);
    //u8_ifstream   fin(ipath, om);
    if(!fin)
        return false;
    return load(fin, ipath);
}

bool            AbstractFile::load(const QString& s)
{
    return load(std::filesystem::path(s.toStdString()));
}


//bool            AbstractFile::load(u8_istream&fin, const std::filesystem::path& fp)
bool            AbstractFile::load(std::istream&fin, const std::filesystem::path& fp)
{
    if(!read_enabled())
        return false;

    Vector<char>     data;
    
    fin.seekg( 0, std::ios::end);
    bool    ok  = !fin.fail();
    size_t  sz  = fin.tellg();
    fin.seekg( 0, std::ios::beg);
    
    if(ok){
        data.resize(sz+1);
        fin.read(data.data(), sz);
        data[sz]    = u8'\0';
    } else {
        char     buffer[64<<10];
        while(!fin.eof() && !fin.fail()){
            fin.read(buffer, sizeof(buffer));
            size_t sz  = fin.gcount();
            data.append(buffer, sz);
        }
        data << u8'\0';
    }
    
    return load(data, fp);
}

bool            AbstractFile::load(Vector<char>&data, const std::filesystem::path& fp)
{
    if(!read_enabled())
        return false;
    reset();
    bool ok = read(data, fp.string());
    if(ok)
        m_file  = fp;
    return ok;
}


bool            AbstractFile::reload()
{
    if(m_file.empty())
        return false;
    return load(m_file);
}


bool            AbstractFile::save()
{
    if(m_file.empty())
        return false;
    return save_to(m_file);
}

bool            AbstractFile::save_as(const std::filesystem::path& oPath)
{
    if(save_to(oPath)){
        m_file  = oPath;
        return true;
    } else
        return false;
}

bool            AbstractFile::save_as(const QString& p)
{
    return save_as(std::filesystem::path(p.toStdString()));
}


bool            AbstractFile::save_to(const std::filesystem::path& oPath)
{
    if(!write_enabled())
        return false;
        
    Vector<char>    data;
    if(!write(data))
        return false;
        
    std::filesystem::path   tmp  = oPath.string() + ".tmp";
    int         n   = 0;
    while(file_exists(tmp.c_str()))
        tmp = oPath.string() + ".tmp" + std::to_string(++n);
    std::ios_base::openmode om  = std::ios_base::out | std::ios_base::trunc;
    if(is_binary())
        om |= std::ios_base::binary;
    bool    f   = false;
    {
        std::ofstream fout(tmp, om);
        if(!fout)
            return false;
        fout.write(data.data(), data.size());
        f   = !fout.fail();
        fout.close();
    }
    if(f){
        file_backup(oPath.c_str(), "bck");
        rename(tmp.c_str(), oPath.c_str());
    } else {
        remove(tmp.c_str());
    }
    return f;
}

bool            AbstractFile::save_to(const QString&p)
{
    return save_to(std::filesystem::path(p.toStdString()));
}

bool            AbstractFile::save_to(std::ostream&out)
{   
    if(!write_enabled())
        return false;
    Vector<char>    data;
    if(!write(data))
        return false;
    out.write(data.data(), data.size());
    return true;
}

bool            AbstractFile::save_to(Vector<char>&data)
{
    if(!write_enabled())
        return false;
    return write(data);
}

void            AbstractFile::set_file(const std::filesystem::path&p)
{
    if(can_change_to(p))
        m_file = p;
}


void            AbstractFile::set_file(const QString& s)
{
    set_file(std::filesystem::path(s.toStdString()));
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

XmlFile::XmlFile()
{
}

XmlFile::~XmlFile()
{
}

bool    XmlFile::read(Vector<char>&buffer, const std::string& fname )
{
    buffer << '\0';
    XmlDocument     doc;
    try {
        doc.parse<0>(buffer.data());
    } catch(const rapidxml::parse_error& pe){
        size_t  pt  = pe.where<char>() - buffer.data();
        yError() << "Xml parse error: " << pe.what() << " (at byte " << pt << ") : " << fname;
        return false;
    }
    
    return read(doc,fname);
}

bool    XmlFile::write(Vector<char>& s)  
{
    XmlDocument     doc;
    XmlNode*        x   = doc.allocate_node(rapidxml::node_pi, "xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.append_node(x);
    write(doc);
    rapidxml::print(std::back_inserter(s), doc, 0);
    return true;
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


boolean_r            x_boolean(const XmlBase*xb)
{
    return to_boolean(xb->value(), xb->value_size());
}

double_r             x_double(const XmlBase* xb)
{
    return to_double(xb->value(), xb->value_size());
}

float_r              x_float(const XmlBase* xb)
{
    return to_float(xb->value(), xb->value_size());
}

unsigned_r           x_hex(const XmlBase*xb)
{
    return to_hex(xb->value(), xb->value_size());
}

uint8_r              x_hex8(const XmlBase*xb)
{
    return to_hex8(xb->value(), xb->value_size());
}

uint16_r             x_hex16(const XmlBase*xb)
{
    return to_hex16(xb->value(), xb->value_size());
}

uint32_r             x_hex32(const XmlBase*xb)
{
    return to_hex32(xb->value(), xb->value_size());
}

uint64_r             x_hex64(const XmlBase*xb)
{
    return to_hex64(xb->value(), xb->value_size());
}

int_r                x_int(const XmlBase*xb)
{
    return to_int(xb->value(), xb->value_size());
}

int_r                x_integer(const XmlBase*xb)
{
    return to_integer(xb->value(), xb->value_size());
}

int8_r               x_int8(const XmlBase*xb)
{
    return to_int8(xb->value(), xb->value_size());
}

int16_r              x_int16(const XmlBase*xb)
{
    return to_int16(xb->value(), xb->value_size());
}

int32_r              x_int32(const XmlBase*xb)
{
    return to_int32(xb->value(), xb->value_size());
}

int64_r              x_int64(const XmlBase*xb)
{
    return to_int64(xb->value(), xb->value_size());
}

short_r              x_short(const XmlBase*xb)
{
    return to_short(xb->value(), xb->value_size());
}
    
std::string_view    x_string(const XmlBase*xb)
{
    return std::string_view(xb->value(), xb->value_size());
}

unsigned_r           x_uint(const XmlBase*xb)
{
    return to_uint(xb->value(), xb->value_size());
}

uint8_r              x_uint8(const XmlBase*xb)
{
    return to_uint8(xb->value(), xb->value_size());
}

uint16_r             x_uint16(const XmlBase*xb)
{
    return to_uint16(xb->value(), xb->value_size());
}

uint32_r             x_uint32(const XmlBase*xb)
{
    return to_uint32(xb->value(), xb->value_size());
}

uint64_r             x_uint64(const XmlBase*xb)
{
    return to_uint64(xb->value(), xb->value_size());
}

unsigned_r           x_uinteger(const XmlBase*xb)
{
    return to_uinteger(xb->value(), xb->value_size());
}

unsigned_r           x_unsigned(const XmlBase*xb)
{
    return to_unsigned(xb->value(), xb->value_size());
}

ushort_r             x_ushort(const XmlBase*xb)
{
    return to_ushort(xb->value(), xb->value_size());
}


//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

XmlDocument*         document_for(XmlBase* xb)
{
    XmlNode*    xn  = xb -> parent();
    if(xn)
        return xn -> document();
    return nullptr;
}


void                 write_hex(XmlBase* xb, uint8_t v)
{
    write_x(xb, String::hex(v));
}

void                 write_hex(XmlBase* xb, uint16_t v)
{
    write_x(xb, String::hex(v));
}

void                 write_hex(XmlBase* xb, uint32_t v) 
{
    write_x(xb, String::hex(v));
}

void                 write_hex(XmlBase* xb, uint64_t v)
{
    write_x(xb, String::hex(v));
}


void                 write_x(XmlBase* xb, bool f)
{
    xb->value( f ? "true" : "false");
}

void                 write_x(XmlBase* xb, double v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, float v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, int8_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, int16_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, int32_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, int64_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, uint8_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, uint16_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, uint32_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, uint64_t v)
{
    write_x(xb, String::number(v));
}

void                 write_x(XmlBase* xb, const std::string&v)
{
    XmlDocument*doc = document_for(xb);
    if(doc && !v.empty())
        xb -> value(doc -> allocate_string(v.c_str(), v.size()), v.size());
}

void                 write_x(XmlBase* xb, const String&v)
{
    XmlDocument*doc = document_for(xb);
    if(doc && !v.empty())
        xb -> value(doc -> allocate_string(v.c_str(), v.size()), v.size());
}

