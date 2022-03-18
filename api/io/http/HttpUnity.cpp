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
    
    Result<UriView>                 parse_uri(const std::string_view& s)
    {
        //  URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
        //  authority = [userinfo "@"] host [":" port]
        
        if(s.empty())
            return {};

        enum Mode {
            Start       = 0,
            Scheme,
            Colon,
            Slash1,
            Slash2,
            UserOrHost,
            PwdOrPort,
            Host1,
            Host2,
            SqOpen,
            IPv6,
            SqClose,
            Port,
            PSlash,
            Path,
            Query,
            Fragment,
            Error
        };
        
        UriView     ret;
        
        const char* z   = nullptr;
        const char* str = nullptr;
        Mode        mode    = Start;
        std::string_view    userhost;
        
        auto sstring = [&](int n=0) -> std::string_view {
            if(str && z){
                std::string_view    ret(str, z-str+n);
                str     = nullptr;
                return ret;
            } else
                return std::string_view();
        };
        
        for(z=s.begin();z!=s.end();++z){
            switch(mode){
            case Start:
                if(*z == '.'){
                    mode    = Path;
                    str     = z;
                } else if(*z == '/'){
                    mode    = Slash1;
                } else if(is_alpha(*z)){
                    mode    = Scheme;
                    str     = z;
                } else 
                    mode    = Error;
                break;
            case Scheme:
                if(*z == ':'){
                    ret.scheme  = sstring();
                    mode    = Colon;
                } else if(!(is_alnum(*z) || (*z == '+') || (*z == '.') || (*z == '-')))
                    mode    = Error;
                break;
            case Colon:
                if(*z == '/'){
                    mode    = Slash1;
                } else {
                    str     = z;
                    mode    = Path;
                }
                break;
            case Slash1:
                if(*z == '/'){
                    mode    = Slash2;
                } else {
                    str     = z-1;
                    mode    = Path;
                }
                break;
            case Slash2:
                if(*z == '/'){
                    mode    = Path;
                    str     = z;
                } else if(*z == '['){
                    mode    = SqOpen;
                    str     = z;
                } else if(*z == '@'){
                    mode    = Error;
                } else if(*z == ':'){
                    mode    = Error;
                } else {
                    mode    = UserOrHost;
                    str     = z;
                }
                break;
            case UserOrHost:
                if(*z == '@'){
                    ret.user    = sstring();
                    mode        = Host1;
                } else if(*z == ':'){
                    mode        = PwdOrPort;
                    userhost    = sstring();
                    str         = z+1;
                } else if(*z == '/'){
                    //  it's a host....
                    ret.host    = sstring();
                    mode        = Path;
                    str         = z;
                }
                break;
            case PwdOrPort:
                if(*z == '@'){
                    // it was a user...  (with password)
                    ret.user    = userhost;
                    ret.pwd     = sstring();
                    mode        = Host1;
                } else if(*z == '/'){
                    // it was a host:port
                    ret.host    = userhost;
                    ret.port    = to_integer(sstring()).value;
                    mode        = Path;
                    str         = z;
                }
                break;
            case Host1:
                str         = z;
                if(*z == '['){
                    mode    = SqOpen;
                } else if(*z == ':'){
                    mode    = Error;
                } else {
                    mode    = Host2;
                }
                break;
            case Host2:
                if(*z == ':'){
                    ret.host    = sstring();
                    mode        = Port;
                    str         = z + 1;
                } else if(*z == '/'){
                    ret.host    = sstring();
                    mode        = Path;
                    str         = z;
                } 
                break;
            case SqOpen:
                if( *z == ']'){
                    mode        = Error;
                } else 
                    mode        = IPv6;
                break;
            case IPv6:
                if(*z == ']')
                    mode         = SqClose;
                break;
            case SqClose:
                ret.host        = sstring(1);
                if(*z == ':'){
                    mode        = Port;
                    str         = z+1;
                } else if(*z == '/'){
                    mode        = Path;
                    str         = z;
                } else
                    mode        = Error;
                break;
            case Port:
                if(*z == '/'){
                    ret.port    = to_integer(sstring()).value;
                    mode        = Path;
                    str         = z;
                } else if(!is_digit(*z)){
                    mode        = Error;
                }
                break;
            case Path:
                if(*z == '?'){
                    ret.path    = sstring();
                    mode        = Query;
                    str         = z+1;
                } else if(*z == '#'){
                    ret.path    = sstring();
                    mode        = Fragment;
                    str         = z+1;
                }
                break;
            case Query:
                if(*z == '#'){
                    ret.query  = sstring();
                    mode       = Fragment;
                    str         = z+1;
                }
                break;
            default:
                break;
            }
            
            if(mode == Error)
                return { ret, false};
        }
        
        switch(mode){
        case Scheme:
        case Colon:
        case Slash1:
        case Slash2:
            mode        = Error;
            break;
        case UserOrHost:
            ret.host    = sstring();
            break;
        case PwdOrPort:
            ret.host    = userhost;
            ret.port    = to_integer(sstring()).value;
            break;
        case Path:
            ret.path    = sstring();
            break;
        case Query:
            ret.query   = sstring();
            break;
        case Fragment:
            ret.fragment    = sstring();
            break;
        default:
            break;
        }
        
        
        return { ret, mode != Error };
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
