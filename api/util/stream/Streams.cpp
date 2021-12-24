#include "Bytes.hpp"
#include "File.hpp"
#include "LineCol.hpp"
#include "OSFile.hpp"
#include "Ops.hpp"


#include <util/ByteArray.hpp>
#include <util/FileUtils.hpp>

#include <cstring>
#include <charconv>

#include <fcntl.h>
#include <unistd.h>

#include <QString>

namespace yq {

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  BYTE STREAM
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace stream {
        bool Bytes::write(const char* buf, size_t cb)
        {
            m_bytes.append(buf, cb);
            return true;
        }
    }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  C++ FILE STREAM
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace stream {
        File::File()
        {
        }

        File::~File()
        {
            close();
        }

        void        File::close()
        {
            if(m_file.is_open()){
                m_path.clear();
                m_file.flush();
                m_file.close();
            }
        }

        void    File::flush() 
        {
            if(m_file.is_open())
                m_file.flush();
        }

        bool        File::is_open() const
        {
            return m_file.is_open();
        }

        bool        File::open(const std::filesystem::path& pth)
        {
            if(m_file.is_open())
                return false;
            m_file.open(pth, std::ios_base::binary);
            if(m_file){
                m_path      = pth;
                return true;
            } else
                return false;
        }

        bool    File::write(const char*z, size_t cb) 
        {
            if(m_file.is_open()){
                m_file.write(z, cb);
                return true;
            } else
                return false;
        }

    }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  LINE COUNTING STREAM
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace stream {

        LineCol::LineCol(Stream& out) : m_stream(out), m_line(0), m_column(0) 
        {
        }
        
        bool    LineCol::is_open() const
        {
            return m_stream.is_open();
        }

        bool    LineCol::write(const char* z, size_t cb)
        {
            for(size_t i=0;i<cb;++i){
                if(z[i] == '\n'){
                    ++m_line;
                    m_column    = 0;
                } else
                    ++m_column;
            }
            return m_stream.write(z, cb);
        }
    }
    



    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  NULL STREAM
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        namespace {
            class NullStream : public Stream {
            public:
                bool    is_open() const override { return false; }
                bool    write(const char*, size_t) { return true; }
            };
        }


        Stream&  Stream::null()
        {
            static NullStream   ret;
            return ret;
        }


    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  OS FILE STREAM
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    namespace stream {

        OSFile::OSFile()
        {
        }

        OSFile::~OSFile()
        {
            if(is_open()){
                do_flush();
                do_close();
            }
        }

        void    OSFile::close()
        {
            if(!is_open())
                return ;
            do_flush();
            do_close();
        }

        void            OSFile::do_close()
        {
            ::close(m_fd);
            m_fd        = -1;
            
            if(!(m_options & OVERWRITE)){
                if(!(m_options & NO_BACKUP))
                    file_backup(m_path.c_str());
                rename(m_tmp.c_str(), m_path.c_str());
            }
        }

        void            OSFile::do_flush()
        {
            syncfs(m_fd);
        }

        void    OSFile::flush() 
        {
            if(!is_open())
                return ;
            do_flush();
        }

        bool            OSFile::open(const std::filesystem::path& pth, unsigned int options)
        {
            if(is_open())
                return false;
            
            std::filesystem::path       p;
            int                         flags   = O_CREAT | O_WRONLY;
            
            m_path      = pth;
            m_options   = options;
            
            if(options & OVERWRITE){
                p   = pth;                    //  no chance of changing your mind?
                if(!(options & NO_BACKUP))   // move old into backup
                    file_backup(pth.c_str());
            } else {
                m_tmp   = m_path.concat("*.tmp");
            }
            
            m_fd    = ::open(p.c_str(), flags, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
            
            return m_fd != -1;
        }

        bool    OSFile::write(const char*z, size_t cb)
        {
            if(!is_open())
                return false;
            if(!z)
                return false;
            if(!cb)     // what does it mean to write empty?  Considering it a success
                return true;
            
            do {
                ssize_t     cnt =  ::write(m_fd, z, cb);
                if(cnt <= 0)   //  error, no more
                    return false;
                    
                z  += cnt;
                cb -= cnt;
            } while(cb);
            return true;
        }
    }

    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  STREAM OPERATORS
    //  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        static constexpr const size_t   kStdBuf = 63;

        Stream&     operator<<(Stream&str, const char* v)
        {
            if(v)
                str.write(v, strlen(v));
            return str;
        }

        Stream&     operator<<(Stream&str, const std::string&v)
        {
            str.write(v.data(), v.size());
            return str;
        }

        Stream&     operator<<(Stream&str, const std::string_view&v)
        {
            str.write(v.data(), v.size());
            return str;
        }

        Stream&     operator<<(Stream&str, const QString&v)
        {
            return str << v.toStdString();
        }

        Stream&     operator<<(Stream&str,  const QByteArray&v)
        {
            str.write(v.constData(), v.size());
            return str;
        }

        Stream&     operator<<(Stream& str, char v)
        {
            str.write(&v, 1);
            return str;
        }

        Stream&     operator<<(Stream&str, float v)
        {
            //  std::to_chars would be GREAT, if GCC implemented it.....
            char    buf[kStdBuf+1];
            int n  = snprintf(buf, kStdBuf, "%g", v);
            str.write(buf, n);
            return str;
        }

        Stream&     operator<<(Stream&str, double v)
        {
            //  std::to_chars would be GREAT, if GCC implemented it.....
            char    buf[kStdBuf+1];
            int n  = snprintf(buf, kStdBuf, "%lg", v);
            str.write(buf, n);
            return str;
        }

        Stream&     operator<<(Stream&str, bool v)
        {
            static constexpr const std::string_view     kFalse  = "false";
            static constexpr const std::string_view     kTrue   = "true";
            return str << (v?kTrue:kFalse);
        }


        Stream&     operator<<(Stream&str, int8_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }

        Stream&     operator<<(Stream&str, int16_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }

        Stream&     operator<<(Stream&str, int32_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }

        Stream&     operator<<(Stream&str, int64_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }


        Stream&     operator<<(Stream&str, uint8_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }

        Stream&     operator<<(Stream&str, uint16_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }

        Stream&     operator<<(Stream&str, uint32_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }

        Stream&     operator<<(Stream&str, uint64_t v)
        {
            char    buf[kStdBuf+1];
            auto [p,ec] = std::to_chars(buf, buf+kStdBuf, v);
            str.write(buf, (p-buf));
            return str;
        }


        Stream&     operator<<(Stream&str, const std::filesystem::path&v)
        {
            return str << v.native();
        }

}
