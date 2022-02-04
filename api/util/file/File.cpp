////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "AbstractFile.hpp"
#include "FileUtils.hpp"
#include "XmlFile.hpp"
#include "XmlUtils.hpp"

#include <util/algo/Compare.hpp>
#include <util/collection/Set.hpp>
#include <util/log/Logging.hpp>
#include <util/stream/Bytes.hpp>
#include <util/text/ByteArray.hpp>
#include <util/text/Strings.hpp>

#include <filesystem>
#include <fstream>
#include <cstring>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"


namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ABSTRACT FILE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


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


        //bool            AbstractFile::load(u8_istream&fin, const std::filesystem::path& fp)
        bool            AbstractFile::load(std::istream&fin, const std::filesystem::path& fp)
        {
            if(!read_enabled())
                return false;

            ByteArray       data;
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
            
            return load(std::move(data), fp);
        }

        bool            AbstractFile::load(ByteArray&&data, const std::filesystem::path& fp)
        {
            if(!read_enabled())
                return false;
            reset();
            bool ok = read(std::move(data), fp.string());
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


        bool            AbstractFile::save_to(const std::filesystem::path& oPath) const
        {
            if(!write_enabled())
                return false;
            
            ByteArray               data;
            yq::stream::Bytes       output(data);
            if(!write(output))
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


        bool            AbstractFile::save_to(yq::Stream&out) const
        {   
            if(!write_enabled())
                return false;
            if(!write(out))
                return false;
            return true;
        }

        void            AbstractFile::set_file(const std::filesystem::path&p)
        {
            if(can_change_to(p))
                m_file = p;
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  FILE UTILS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


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

        ByteArray       file_bytes(const std::filesystem::path& iFile)
        {
            struct stat buf;
            memset(&buf, 0, sizeof(buf));
            if(::stat(iFile.c_str(), &buf) == -1)
                return ByteArray();
            size_t      sz  = buf.st_size;
            int fd  = open(iFile.c_str(), O_RDONLY);
            if(fd == -1)
                return ByteArray();
            
            ByteArray   ret;
            try {
                ret.resize(sz);
            } catch(const std::bad_alloc&){
                return ByteArray();
            }
            ssize_t sz2 = read(fd, ret.data(), sz);
            close(fd);
            if(sz2 < 0)
                return ByteArray();
            if(sz2 < (ssize_t) sz)
                ret.resize(sz2);
            return ret;
        }

        String          file_string(const std::filesystem::path&iFile)
        {
            struct stat buf;
            memset(&buf, 0, sizeof(buf));
            if(::stat(iFile.c_str(), &buf) == -1)
                return String();
            size_t      sz  = buf.st_size;
            int fd  = open(iFile.c_str(), O_RDONLY);
            if(fd == -1)
                return String();
            
            String   ret;
            try {
                ret.resize(sz);
            } catch(const std::bad_alloc&){
                return String();
            }
            ssize_t sz2 = read(fd, ret.data(), sz);
            close(fd);
            if(sz2 < 0)
                return String();
            if(sz2 < (ssize_t) sz)
                ret.resize(sz2);
            return ret;
        }



        bool        file_exists(const char* iFile)
        {
            return ::access(iFile, F_OK) == 0;
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

        SizeTimestamp file_size_and_timestamp(const std::filesystem::path& iFile)
        {
            struct stat buf;
            if(::stat(iFile.c_str(), &buf) != 0)
                return SizeTimestamp();
            return SizeTimestamp( buf.st_size, buf.st_mtim.tv_sec, buf.st_mtim.tv_nsec );
        }

        //std::filesystem::path   find_first_file(const std::filesystem::path& dir, const std::string_view& name)
        //{
            //for(auto const& de : std::filesystem::directory_iterator(dname)){
                //auto p = de.path();
                //if(is_similar(p.filename().c_str(), sv))
                    //return p;
            //}
            //return std::filesystem::path();
        //}



        //  Make the path (including the specified filename)
        bool            make_path(const std::filesystem::path& p, mode_t dirMode)
        {
            if(::access(p.c_str(), X_OK) == 0)  // it exists
                return true;
            std::filesystem::path   pp  = p.parent_path();
            if((pp != p) && !make_path(pp, dirMode))
                return false;
            return !mkdir(p.c_str(), dirMode);
        }

        //  Make the path (not including the specified filename)
        bool            make_parent_path(const std::filesystem::path&p, mode_t dirMode)
        {
            return make_path(p.parent_path());
        }



        namespace dir {


            path_vec        all_children(const path& d, const std::string_view& name)
            {
                path_vec    ret;
                for_all_children(d, 0, [&](const path& p) { 
                    if(is_similar(p.filename().c_str(), name))
                        ret << p; 
                });
                return ret;
            }

            path_vec        all_children(const std::vector<path>& dirs, const std::string_view& name)
            {
                path_vec    ret;
                for_all_children(dirs, 0, [&](const path& p) { 
                    if(is_similar(p.filename().c_str(), name))
                        ret << p; 
                });
                return ret;
            }
            
            path            first_child(const path& d, const std::string_view& name)
            {
                return for_all_children( d, 0, [&](const path&p) -> auto {
                    return is_similar(p.filename().c_str(), name) ? p : path();
                });
            }
            
            path            first_child(const std::vector<path>& d, const std::string_view& name)
            {
                return for_all_children( d, 0, [&](const path&p) -> auto {
                    return is_similar(p.filename().c_str(), name) ? p : path();
                });
            }

            
            Vector<String>  subdirectory_names(const path&d, unsigned options) 
            {
                Vector<String>  ret;
                for_all_children(d, options | NO_FILES, [&](const path&p){
                    ret << p.filename().string();
                });
                return ret;
            }
            
            Vector<String>  subdirectory_names(const std::vector<path>&dirs, unsigned options) 
            {
                Vector<String>  ret;
                for_all_children(dirs, options | NO_FILES, [&](const path&p){
                    ret << p.filename().string();
                });
                return ret;
            }


            //  Finds all chlidren (not hidden)
            StringSet       subdirectory_names_set(const path&d, unsigned options) 
            {
                StringSet   ret;
                for_all_children(d, options | NO_FILES, [&](const path&p){
                    ret << p.filename().string();
                });
                return ret;
            }
            
            StringSet       subdirectory_names_set(const std::vector<path>&dirs, unsigned options) 
            {
                StringSet   ret;
                for_all_children(dirs, options | NO_FILES, [&](const path&p){
                    ret << p.filename().string();
                });
                return ret;
            }


            //  Finds all chlidren (not hidden)
            path_vec        subdirectories(const path&d, unsigned options)
            {
                path_vec    ret;
                for_all_children(d, options | NO_FILES, [&](const path&p){
                    ret << p;
                });
                return ret;
            }
            
            path_vec        subdirectories(const std::vector<path>&dirs, unsigned options)
            {
                path_vec    ret;
                for_all_children(dirs, options | NO_FILES, [&](const path&p){
                    ret << p;
                });
                return ret;
            }

            bool            child_exists(const path&d, const std::string_view& name)
            {
                return for_all_children(d, 0, [&](const path&p) -> bool {
                    return is_similar(p.filename().c_str(), name);
                });
            }

            bool            child_exists(const std::vector<path>&dirs, const std::string_view& name)
            {
                return for_all_children(dirs, 0, [&](const path&p) -> bool {
                    return is_similar(p.filename().c_str(), name);
                });
            }
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  XML FILE
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////

        XmlFile::XmlFile()
        {
        }

        XmlFile::~XmlFile()
        {
        }

        bool    XmlFile::read(ByteArray&&buffer, const std::string& fname )
        {
            ByteArray   chars   = std::move(buffer);
            chars << '\0';
            XmlDocument     doc;
            try {
                doc.parse<0>(chars.data());
            } catch(const rapidxml::parse_error& pe){
                size_t  pt  = pe.where<char>() - buffer.data();
                yError() << "Xml parse error: " << pe.what() << " (at byte " << pt << ") : " << fname;
                return false;
            }
            
            return read(doc,fname);
        }

        bool    XmlFile::write(yq::Stream & str) const
        {
            Vector<char>    s;
            XmlDocument     doc;
            XmlNode*        x   = doc.allocate_node(rapidxml::node_pi, "xml", "version=\"1.0\" encoding=\"UTF-8\"");
            doc.append_node(x);
            write(doc);
            rapidxml::print(std::back_inserter(s), doc, 0);
            str.write(s.data(), s.size());
            return true;
        }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  XML UTILS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
}