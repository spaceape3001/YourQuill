//#include "HttpConnection.hpp"
#include "HttpParser.hpp"
#include "HttpData.hpp"

#include <util/text/Utils.hpp>

#include <tbb/spin_rw_mutex.h>

namespace yq {

    
    bool                    match(const VersionSpec& a, const VersionSpec& b)
    {
        return is_similar(a.protocol, b.protocol) && (a.v_major == b.v_major) && (a.v_minor == b.v_minor);
    }
    
    HttpHeaderView          parse_header_line(const std::string_view&s)
    {
        HttpHeaderView  ret;
        const char* c   = strnchr(s, ':');
        if(c){
            ret.key     = trimmed(std::string_view(s.data(), c));
            ret.value   = trimmed(std::string_view(c+1, s.end()));
        }
        return ret;
    }

    MethodUriVersion        parse_method_uri(const std::string_view& input)
    {
        const char* z       = nullptr;
        const char* str     = nullptr;
        
        enum Mode {
            Start    = 0,
            Method,
            MSpace,
            Uri,
            USpace,
            Version,
            Done
        };
        
        Mode                mode    = Start;
        MethodUriVersion    ret;

        for(z = input.begin(); z<input.end(); ++z){
            switch(mode){
            case Start:
                if(!is_space(*z)){
                    str     = z;
                    mode    = Method;
                }
                break;
            case Method:
                if(is_space(*z)){
                    ret.method  = std::string_view(str, z);
                    mode    = MSpace;
                }
                break;
            case MSpace:
                if(is_space(*z))
                    break;
                str     = z;
                mode    = Uri;
                break;
            case Uri:
                if(is_space(*z)){
                    ret.uri = std::string_view(str, z);
                    mode    = USpace;
                }
                break;
            case USpace:
                if(is_space(*z))
                    break;
                str = z;
                mode    = Version;
                break;
            case Version:
                if(is_space(*z)){
                    ret.version = std::string_view(str, z);
                    mode        = Done;
                }
                break;
            default:
                break;
            }
        }
        
        switch(mode){
        case Method:
            ret.method  = std::string_view(str, z);
            break;
        case Uri:
            ret.uri = std::string_view(str, z);
            break;
        case Version:
            ret.version  = std::string_view(str, z);
            break;
        default:
            break;
        }
        
        return ret;
    }
    
    UriView                 parse_uri(const std::string_view& s)
    {
        UriView     ret;
            //  TODO
        return ret;
    }
    

    VersionSpec  parse_version(const std::string_view& s)
    {
        VersionSpec ret;
        
        enum Mode {
            Major = 0,
            Dot,
            Minor,
            Done
        };
        
        const char* y   = strnchr(s, '/');
        if(!y){
            ret.protocol    = s;
        } else {
            ret.protocol    = std::string_view(s.data(), y);
            ++y;
            Mode        mode    = Major;
            for(const char* z = y; z<s.end(); ++z){
                char ch = *z;
                switch(mode){
                case Major:
                    if(is_digit(ch)){
                        ret.v_major = ret.v_major + (uint16_t) (ch - '0');
                    } else if(ch == '.'){
                        mode    = Dot;
                    } else
                        mode    = Done;
                    break;
                case Dot:
                    if(!is_digit(ch)){
                        mode    = Done;
                        break;
                    } 
                    mode    = Minor;
                    ret.v_minor = (uint16_t) (ch - '0');
                    break;
                case Minor:
                    if(!is_digit(ch)){
                        mode    = Done;
                    } else {
                        ret.v_minor = ret.v_minor + (uint16_t) (ch - '0');
                    }
                    break;
                default:
                    break;
                }
            }
        }
        
        return ret;
    }

    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    struct HttpData::Pool {
        size_t                          size    = 0ULL;
        size_t                          total   = 0ULL;
        std::vector<Ref<HttpData>>      available;
        tbb::spin_rw_mutex              mutex;
        
        Pool(){}
        ~Pool(){}
        
        bool    init(size_t n)
        {
            size_t  mask    = (1ULL << kClean) - 1;
            bool    inc     = static_cast<bool>(n & mask);
            n  = (n >> kClean) << kClean;
            if(inc)
                ++n;
            total           = n;
            size            = n - sizeof(HttpData);
            return true;
        }
        
        HttpData*       create() 
        {
            HttpData*   p    = (HttpData*) new char[total];
            new(p) HttpData();
            return  p;
        }
        
    };
    
    HttpData::Pool&  HttpData::pool()
    {
        static Pool     s_ret;
        return s_ret;
    }
    
    
    bool    HttpData::start_pool(uint32_t n)
    {
        static bool    f = pool().init(n);
        return f;
    }

    uint32_t    HttpData::size()
    {
        return pool().size;
    }

    Ref<HttpData>    HttpData::make()
    {
        // temporary
        return pool().create();
    }

    HttpData::HttpData() : m_count{0}
    {
    }
    
    HttpData::~HttpData()
    {
    }

    void    HttpData::decRef() const
    {
        if(!--m_count){
            //  temporary
            delete this;
            //  recycle
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    bool    HttpHeaderView::empty() const
    {
        return key.empty() && value.empty();
    }
}
