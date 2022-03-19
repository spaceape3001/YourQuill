#include "HttpConnection.hpp"
#include "HttpContext.hpp"
#include "HttpData.hpp"
#include "HttpDataStream.hpp"
#include "HttpParser.hpp"
#include "HttpResponse.hpp"
#include "HttpServer.hpp"

#include <util/log/Logging.hpp>
#include <util/stream/Ops.hpp>
#include <util/stream/Text.hpp>
#include <util/text/Utils.hpp>

#include <asio/write.hpp>

#include <tbb/spin_rw_mutex.h>
#include <unistd.h>

namespace yq {

    namespace {
        auto      buffer_for(const HttpDataPtr dp)
        {
            return asio::buffer(dp->data(), dp->count());
        }
    }

    Stream& operator<<(Stream&s, const VersionSpec&v)
    {
        return s << v.protocol << '/' << v.major << '.' << v.minor;
    }
    
    log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&s, const VersionSpec&v)
    {
        return s << v.protocol << '/' << v.major << '.' << v.minor;
    }

    bool                    match(const VersionSpec& a, const VersionSpec& b)
    {
        return is_similar(a.protocol, b.protocol) && (a.major == b.major) && (a.minor == b.minor);
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

    Result<UriView>         parse_hostport(const std::string_view&input)
    {
        if(input.empty())
            return {};

        enum Mode {
            Host1       = 0,
            Host2,
            IPv6,
            SqClose,
            Port
        };
        
        UriView     ret;
        Mode        mode    = Host1;
        const char* z       = nullptr;
        const char* str     = nullptr;
        
        auto sstring = [&]() -> std::string_view {
            if(str && z){
                std::string_view    ret(str, z-str);
                str     = nullptr;
                return ret;
            } else
                return std::string_view();
        };
        
        for(z = input.begin(); z<input.end(); ++z){
            switch(mode){
            case Host1:
                str     = z;
                if(*z == '[')
                    mode    = IPv6;
                else
                    mode    = Host2;
                break;
            case Host2:
                if(*z == ':'){
                    ret.host    = sstring();
                    mode    = Port;
                }
                break;
            case IPv6:
                if(*z == ']')
                    mode     = SqClose;
                break;
            case SqClose:
                if(*z == ':'){
                    ret.host    = sstring();
                    mode        = Port;
                }
                break;
            case Port:
                if(!str)
                    str = z;
                break;
            }
        }
        
        bool        good    = true;
        
        switch(mode){
        case Host2:
        case SqClose:
            ret.host        = sstring();
            break;
        case Port:
            {
                auto    r   = to_integer(sstring());
                ret.port    = r.value;
                good        = good && r.good;
            }
            break;
        default:
            good    = false;
            break;
        }
        
        return { ret, good };
        
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
            
        if(s == "/")
            return { UriView{ "", "", "", "", "/", "", "", 0 }, true };

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
                        ret.major = ret.major + (uint16_t) (ch - '0');
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
                    ret.minor = (uint16_t) (ch - '0');
                    break;
                case Minor:
                    if(!is_digit(ch)){
                        mode    = Done;
                    } else {
                        ret.minor = ret.minor + (uint16_t) (ch - '0');
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

    HttpConnection::HttpConnection(HttpContext&ctx, asio::ip::tcp::socket s) : 
        m_context(ctx), m_socket(std::move(s))
    {
        m_buffer    = HttpData::make();
        m_request   = new HttpRequest;
    }
    
    HttpConnection::~HttpConnection()
    {
    }
    
    void    HttpConnection::dispatch(HttpStatus code)
    {
        Ref<HttpResponse>   r   = new HttpResponse(m_context.version);
        r -> status(code);
        send(r);
    }

    void    HttpConnection::dispatch(HttpStatus code, VersionSpec vs)
    {
        Ref<HttpResponse>   r   = new HttpResponse(limit(vs));
        r -> status(code);
        send(r);
    }

    void        HttpConnection::dispatch(Ref<HttpRequest> rq)
    {
        HttpStatus  code;
        if(!m_context.handler){
            dispatch(HttpStatus::NotImplemented);
            return;
        }
    
        if(rq->m_uri.host.empty() && !rq->m_host.empty())
            rq->m_uri.host    = rq->m_host;
        if(!rq->m_uri.port && rq->m_port)
            rq->m_uri.port    = rq->m_port;
        
        
        if(rq->m_version == VersionSpec{})
            rq->m_version   = http09();
            
        if(match(rq->m_version, http11()) && rq->m_uri.host.empty()){
            dispatch(HttpStatus::BadRequest);
            return ;
        }
    
        Ref<HttpResponse>   response = new HttpResponse(limit(rq->m_version));
        try {
            m_context.handler(*rq, *response);
            if(response->m_status == HttpStatus())
                code    = HttpStatus::InternalError;
        }
        catch(HttpStatus ex)
        {
            code    = ex;
        }
        catch(HttpStatus::enum_t ex)
        {
            code    = ex;
        }
        catch(int ex)
        {
            if((ex >= 0) && (ex < 600)){
                code    = (HttpStatus::enum_t) ex;
            } else 
                code    = HttpStatus::InternalError;
        }
        #ifdef NDEBUG
        catch(...)
        {
            code    = HttpStatus::InternalError;
        }
        #endif
        
        if((code != HttpStatus()) && isError(code)){
            dispatch(code);
        } else
            send(response);
    }

    void        HttpConnection::do_read()
    {
        if(!m_buffer->available())
            return ;
            
        auto self = shared_from_this();
        m_socket.async_read_some(asio::buffer(m_buffer->freespace(), m_buffer->available()), 
            [this, self](std::error_code ec, std::size_t length){
                if(!ec){
                    m_buffer->advance(length);
                    process();
                    if(m_rxMode != RxError)
                        do_read();
                }
            }
        );
    }
    
    VersionSpec HttpConnection::limit(VersionSpec vs) const
    {
        if((Version) vs > (Version) m_context.version)
            vs  = m_context.version;
        return vs;
    }

    std::string_view    HttpConnection::next_line()
    {
        std::string_view    line = m_buffer -> as_view(m_next);
        if(line.empty()){
            m_eod   = true;
            return std::string_view();
        }

        const char* z   = strany(line, "\r\n");
        if(!z){
            m_eod   = true;
            return std::string_view();
        }

        const char* y   = z;
        if(z && (*z == '\r'))
            ++z;
        if(z && (*z == '\n'))
            ++z;
        m_next  = z - m_buffer->data();
        m_eod   = false;
        return std::string_view(line.data(), y-line.data());
    }
    
    void        HttpConnection::process()
    {
        for(m_eod=false;!m_eod;){
            switch(m_rxMode){
            case RxMethod:
                rxMethod();
                break;
            case RxHeader:
                rxHeader();
                break;
            case RxBody:
                rxBody();
                break;
            case RxDispatch:
                rxDispatch();
                break;
            case RxDone:
            case RxError:
                return;
            }
        }
    }

    void        HttpConnection::rxBody()
    {
    yInfo() << "rxBody";
        
        dispatch(HttpStatus::LoopDetected);
    }

    void        HttpConnection::rxDispatch()
    {
    yInfo() << "rxDispatch";

        if(!m_request){
            dispatch(HttpStatus::InternalError);
            m_rxMode    = RxError;
            return;
        }
        
        VersionSpec as = limit(m_request->m_version);
        dispatch(m_request);
        m_request   = nullptr;
        if(match(as, http11())){    
            auto old    = m_buffer;
            m_buffer    = HttpData::make();
            if(m_next < old->count())
                m_buffer -> append(old->as_view(m_next));
            m_next      = 0;
            m_rxMode    = RxMethod;
        } else
            m_rxMode    = RxDone;
    }
    
    
    void        HttpConnection::rxHeader()
    {
        if(!m_request){
            dispatch(HttpStatus::InternalError);
            m_rxMode    = RxError;
            return;
        }
    
        HttpHeaderView  hv  = parse_header_line(next_line());
        if(hv.empty()){
            if(!m_eod){
                //if(m_content_length){
                    //mode        = RxBody;
                //} else {
                    m_rxMode    = RxDispatch;
                //}
            }
            return;
        }
        
        m_request -> m_headers.push_back(hv);


    yInfo() << "rxHeader ('" << hv.key << "', '" << hv.value << "')";

        if(is_similar(hv.key, "host")){
            auto t  = parse_hostport(hv.value);
            if(t.good){
                m_request -> m_host = t.value.host;
                m_request -> m_port = t.value.port;
            }
        } else if(is_similar(hv.key, "Content-Length")){
            auto t = to_integer(hv.value);
            if(t.good)
                m_request -> m_content_length   = t.value;
        }
    }
    
    void        HttpConnection::rxMethod()
    {
        std::string_view    v   = next_line();
        if(v.empty())
            return;

        if(!m_request){
            m_request   = new HttpRequest;
            m_request -> m_buffers.push_back(m_buffer);
        }

        MethodUriVersion  muri      = parse_method_uri(v);
        auto m = HttpOp::value_for(muri.method);
        if(!m.good){
            dispatch(HttpStatus::MethodNotAllowed);
            m_rxMode    = RxError;
            return ;
        }
        
        m_request -> m_method   = m.value;
        if(muri.uri.empty()){
            dispatch(HttpStatus::MissingURI);
            m_rxMode    = RxError;
            return;
        }
    
        Result<UriView>     uri = parse_uri(muri.uri);
        if(!uri.good){
            dispatch(HttpStatus::BadURI);
            m_rxMode    = RxError;
            return;
        }
        
        m_request -> m_uri = uri.value;
        if(muri.version.empty()){
            m_request -> m_version = http09();
            m_rxMode    = RxDispatch;
            return ;
        }

        m_request->m_version = parse_version(muri.version);
        if(!is_similar(m_request->m_version.protocol, "http")){
            dispatch(HttpStatus::BadHTTPVersion);
            m_rxMode    = RxError;
            return ;
        }
        
        if( (Version) http09() == (Version) m_request->m_version){
            m_rxMode    = RxDispatch;
        } else
            m_rxMode    = RxHeader;
    }


    void    HttpConnection::send(Ref<HttpResponse> r)
    {
yInfo() << "Sending...";
        std::string_view    message;
        if(isSuccessful(r -> m_status) && !r->m_content.empty()){
            size_t      count = 0;
            for(auto& p : r->m_content)
                count += p->count();
            HttpDataStream(r->m_reply) << "Content-Length: " << count << "\r\n";
        } else if(isError(r->m_status)){
            message     = m_context.messages[r->m_status];
            if(!message.empty()){
                HttpDataStream(r->m_reply) 
                    << "Content-Type: text/html\r\n" 
                    << "Content-Length: " << message.size() << "\r\n";
            }
        }

        std::vector<asio::const_buffer>   buffers;
        buffers.push_back(buffer_for(r -> m_reply));
        buffers.push_back(asio::buffer("\r\n"));
        
        if(isSuccessful(r->m_status) && !r->m_content.empty()){
            for(auto& p : r->m_content)
                buffers.push_back(buffer_for(p));
            buffers.push_back(asio::buffer("\r\n"));
        } else if(isError(r->m_status) && !message.empty()){
            buffers.push_back(asio::buffer(message));
            buffers.push_back(asio::buffer("\r\n"));
        }
        
        yInfo() << "Reply is:\n" << r->m_reply->as_view();
        
        auto self = shared_from_this();
        async_write(m_socket, buffers, [r, self](std::error_code, size_t){} /* executor is to make sure response & buffers stay alive */ );
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    HttpContext::HttpContext(asio::io_context& ctx) : io_ctx(ctx)
    {
        for(HttpStatus hs : HttpStatus::all_values()){
            stream::Text    txt(messages[hs]);
            
            const char*     cat = "MYSTERY";
            if(isInformational(hs))
                cat = "INFORMATIONAL";
            else if(isSuccessful(hs))
                cat = "SUCCESS";
            else if(isRedirect(hs))
                cat = "REDIRECT";
            else if(isClientError(hs))
                cat = "CLIENT ERROR";
            else if(isServerError(hs))
                cat = "SERVER ERROR";
            txt << "<html><head><title>" << cat << "</title></head><body><h1>" 
                << cat << " (" << hs.value() << "): " << statusMessage(hs) << "</h1></body></html>";
        }
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

    HttpData::HttpData() : m_ref{0}, m_count{0}
    {
    }
    
    HttpData::~HttpData()
    {
    }

    void    HttpData::advance(size_t n) 
    { 
        m_count = std::min<uint32_t>(m_count+n, size());
    }

    size_t  HttpData::append(const char*z, size_t cb)
    {
        cb  = std::min(cb, available());
        memcpy(data()+m_count, z, cb);
        m_count += cb;
        return cb;
    }
    
    size_t  HttpData::append(const std::string_view&s)
    {
        return append(s.data(), s.size());
    }

    std::string_view        HttpData::as_view() const 
    { 
        return std::string_view( data(), m_count ); 
    }
    
    std::string_view        HttpData::as_view(uint32_t n) const 
    { 
        n = std::min(n, m_count);
        return std::string_view( data() + n, m_count - n ); 
    }

    size_t            HttpData::available() const
    {
        return size() - m_count;
    }

    std::string_view  HttpData::copy(const std::string_view& sv)
    {
        size_t  rem = available();
        size_t  cnt = std::min(sv.size(), rem);
        if(!cnt)
            return std::string_view();
        memcpy(data()+m_count, sv.data(), cnt);
        std::string_view    ret(data()+m_count, cnt);
        m_count += cnt;
        return ret;
    }
    
    void    HttpData::count(uint32_t i)
    {
        m_count = std::min(i, (uint32_t) pool().size);
    }
    
    void    HttpData::decRef() const
    {
        if(!--m_ref){
            //  temporary
            delete this;
            //  recycle
        }
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    HttpDataStream::HttpDataStream(std::vector<HttpDataPtr>& destination) : 
        HttpDataStream( 
            [&destination](HttpDataPtr x){ 
                destination.push_back(x); 
            }
        )
    {
    }
    
    HttpDataStream::HttpDataStream(DestinationX x) : m_destination(x)
    {
    }
    
    HttpDataStream::HttpDataStream(HttpDataPtr x) : m_buffer(x)
    {
    }

    HttpDataStream::~HttpDataStream()
    {
        flush();
    }

    HttpDataPtr&    HttpDataStream::buffer()
    {
        if(!m_buffer)
            m_buffer    = HttpData::make();
        return m_buffer;
    }

    void HttpDataStream::close() 
    {
        //  Do nothing, it's always open
    }
    
    void HttpDataStream::flush()
    {
        if(m_destination && m_buffer && m_buffer -> m_count){
            m_destination(m_buffer);
            m_buffer = nullptr;
        }
    }
    
    bool HttpDataStream::is_full() const
    {
        return (!m_destination) && m_buffer && !m_buffer->available();
    }

    bool HttpDataStream::is_open() const 
    { 
        return true; 
    }
    
    bool HttpDataStream::write(const char* z, size_t cb) 
    {
        if(!z)  
            return false;

        size_t  n = 0;
        while( cb && ((n = buffer()->append(z, cb)) != cb)){
            z  += n;
            cb -= n;
            if(!m_destination)
                break;
            m_destination(m_buffer);
            m_buffer    = HttpData::make();
        }
        
        return true;
    }
    
    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    
    HttpResponse::HttpResponse(VersionSpec vs) : m_version(vs)
    {
        m_reply     = HttpData::make();
    }
    
    HttpResponse::~HttpResponse()
    {
    }
    

    HttpResponse::ContentVec&     HttpResponse::content(ContentType ct) 
    {
        if(m_status == HttpStatus())
            status(HttpStatus::Success);
        if(ct != ContentType())
            HttpDataStream(m_reply) << "Content-Type: " << mimeType(ct) << "\r\n";
        return m_content;
    }

    void            HttpResponse::content(const std::string_view&sv)
    {
        HttpDataStream(content()) << sv;
    }


    //! Sets intended status & version
    void            HttpResponse::status(HttpStatus s)
    {
        [[unlikely]] if(m_status != HttpStatus()){
            m_status    = HttpStatus::InternalError;
        } else {
            m_status    = s;
            HttpDataStream(m_reply) << m_version << ' ' << m_status.value() <<  ' ' << statusMessage(s) << "\r\n";
        }
    }


    //  set INTENDED status & version first BEFORE a header
    //  Errors will override....
    void    HttpResponse::header(const std::string_view&k, const std::string_view&v)
    {
        [[unlikely]] if(m_status != HttpStatus()){
            m_status    = HttpStatus::InternalError;
            return ;
        } else {
            HttpDataStream(m_reply)  << k << ": " << v << "\r\n";
        }
    }
    
    void    HttpResponse::redirect(const std::string_view&sv, bool permanent)
    {
        [[unlikely]] if(m_status != HttpStatus()){
            if(!isRedirect(m_status)){
                m_status    = HttpStatus::InternalError;
                return ;
            }
        } else {
            status( permanent ? HttpStatus::MovedPermanently : HttpStatus::TemporaryRedirect );
        }
        
        HttpDataStream(m_reply) << "Location: " << sv << "\r\n";
    }


    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    HttpServer::HttpServer(HttpContext&ctx) : 
        m_context(ctx),
        m_acceptor(ctx.io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), ctx.port)),
        m_socket(ctx.io_ctx)
    {
    }
    
    HttpServer::~HttpServer()
    {
    }
    
    void        HttpServer::start()
    {
        m_acceptor.async_accept(m_socket, [this](std::error_code ec){
            if(!ec && m_context.handler)
                std::make_shared<HttpConnection>(m_context, std::move(m_socket)) -> start();
            start();
        });
    }
}
