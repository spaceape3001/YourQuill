////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yquill.hpp"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <yq/algo/Random.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/stream/Text.hpp>
#include <yq/text/Utils.hpp>
#include <yq/web/HttpDataStream.hpp>
#include <yq/web/HttpParser.hpp>
#include <yq/web/WebContext.hpp>
#include <yq/web/WebPage.hpp>
#include <yq/wksp/Workspace.hpp>

#include <fstream>
#include <syncstream>
#include <tbb/spin_rw_mutex.h>

using namespace yq;

namespace asio {
    const_buffer    buffer(const HttpDataPtr& dp)
    {
        return buffer(dp->data(), dp->count());
    }
}



struct Session : public RefCount {
    SessionData     master;
    std::string     ssid;
    std::string     magic, oldMagic;
    std::string     cookie;
    time_t          created     = 0;
    time_t          accessed    = 0;
    time_t          magicked    = 0;
    mutable tbb::spin_rw_mutex  mutex;
    
    Session();
    ~Session();
};

struct Context : public WebContext, public RefCount {
    Ref<Session>     ss;
    
    std::string_view    cookie_ssid, cookie_magic;
    
    Context(asio::io_context& _io_ctx);
    
    virtual void    set_username(std::string_view sv) override;
    virtual void    set_auto_edit(bool f) override;
    virtual void    set_columns(int v) override;
    virtual void    set_def_root(const Root*rt) override;
    virtual void    set_logged_in(bool v) override;
    virtual void    set_inspect_submit(bool v) override;
    virtual void    set_icon_size(SizeDesc v) override;

    enum Quality {
        Flunk   = 0,    // no qualification
        Expired,        // Expired
        Renew,          // had to renew the cookie
        Pass            // passed
    };

    
    Quality         check_session();
    Quality         config_session();
    void            send_cookie();
};


namespace {

    struct {
        //! Time session's considered "stale"
        time_t              autoLogout      = 900;
        
        //! Scan every five seconds for "stale" sessions
        time_t              staleScan       = 5;
        
        //! Require a new login every.... so often
        time_t              forceLogin      = 3600;
        
        //! Renew the cookie (&magic) this time-interval
        time_t              renewCookie     = 300;
        
        //! When to purge sessions
        time_t              idlePrune       = 450;
        
        //! How often to prune the table
        time_t              pruneInterval   = 60;
        
        //! Old magic is tolerated for this long
        time_t              oldMagic        = 5;
        
        std::string         cookieName      = "yq";

        VersionSpec         version         = http10();
        unsigned int        sizeSSID        = 12;
        unsigned int        sizeMAGIC       = 32;
        
    }  config;

    struct {
        tbb::spin_rw_mutex                      mutex;
        Map<std::string_view, Ref<Session>>     lut;
    }   sessions;

    struct StatusQ {
        std::string_view    source;
        std::string         html_data, reply_data;     
        asio::const_buffer  html_asio, reply_asio;
    };

    using StatusMessages    = EnumMap<HttpStatus, StatusQ>;
    StatusMessages          gStatusMessages;

    void                genStatus()
    {
        for(HttpStatus hs : HttpStatus::all_values()){
            StatusQ&    sq  = gStatusMessages[hs];
            sq.source       = statusMessage(hs);
            
            {
                stream::Text    text(sq.reply_data);
                text << config.version.protocol << '/' << config.version.major << '.' << config.version.minor << ' ' << hs.value() << ' ' << sq.source << "\r\n";
            }

            // check for error messages (shared & workspace)... if not, the default is....
            {
                stream::Text    text(sq.html_data);
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
                text << "<html><head><title>" << cat << "</title></head><body><h1>" 
                    << cat << " (" << hs.value() << "): " << sq.source << "</h1></body></html>";
            }
            
            sq.reply_asio   = asio::buffer(sq.reply_data);
            sq.html_asio    = asio::buffer(sq.html_data);
        }
    }


    VersionSpec         limit(VersionSpec vs)
    {
        if((Version) vs > (Version) config.version)
            return config.version;
        return vs;
    }

    std::pair<const WebPage*, std::string_view>     find_page(HttpOp m, std::string_view path)
    {
        const WebPage*  p   = web::page(m, path);
        if(p)
            return { p, std::string_view() };

        // yInfo() << path << " not found, trying directory";

        size_t  n   = path.find_last_of('/');
        if(n == std::string_view::npos)
            return {};
        if(!n)
            return {};
        p   = web::directory(m, path.substr(0, n));
        if(p)
            return { p, path.substr(n+1) };

        // yInfo() << path.substr(0,n) << " not found, trying glob";
        
        for(; (n != std::string_view::npos) && n; n = path.find_last_of('/', n-1)){
            p       = web::glob(m, path.substr(0, n));
            if(p)
                return { p, path.substr(n+1) };
        }
        
        return {};
    }

    std::filesystem::path     access_log_file()
    {
        std::string     file    = "access-";
        file += wksp::start_file();
        file += '-';
        file += to_string(thread::id());
        file += ".log";
        return wksp::log_dir() / file;
    }

    std::string     randomWebString(unsigned int len)
    {
        static constexpr const std::string_view kChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"sv;
        static constexpr const size_t N = kChars.size();
        std::string     ret;
        ret.resize(len);
        for(size_t i=0;i<len;++i)
            ret[i]  = kChars[ (size_t)( urand() * N)];
        return ret;
    }
    
    void    purge_sessions()
    {
        time_t      now;
        time(&now);
        time_t      minAge      = now - config.idlePrune;
        std::vector<Ref<Session>>    prune;
        
        {
            tbb::spin_rw_mutex::scoped_lock _lock(sessions.mutex, true);
            prune.reserve(sessions.lut.size());
            for(auto i = sessions.lut.begin(), last=sessions.lut.end(); i!=last; ++i){
                if(!i->second){
                    i   = sessions.lut.erase(i);
                } else if(i->second->accessed < minAge){
                    prune.push_back(std::move(i->second));
                    i   = sessions.lut.erase(i);
                } else
                    ++i;
            }
        }
    }
    
    std::string make_cookie(std::string_view ssid, std::string_view magic)
    {
        std::string     ret;
        stream::Text    text(ret);
        text << config.cookieName << '=' << ssid << '+' << magic << "; SameSite=Strict; Max-Age=" << config.renewCookie << "; Version=1";
        return ret;
    }
}

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    Context::Context(asio::io_context& _io_ctx) : WebContext(_io_ctx)
    {
    }

    Context::Quality Context::check_session()
    {
        {
            tbb::spin_rw_mutex::scoped_lock  _lock(ss->mutex, false);
            if(is_similar(ss->magic, cookie_magic)){
                session         = ss->master;
                ss->accessed    = time;
                if(time < ss->magicked + config.renewCookie)
                    return Pass;
                
                //  Good, but needs renewal
                
            } else if(is_similar(ss->oldMagic, cookie_magic)){
                if(time + config.oldMagic < ss->magicked){
                    session = ss->master;
                    ss -> accessed  = time;
                    return Pass;
                }
                return Expired;
            } else
                return Flunk;
        }
    
        //  At this point, the cookie needs to be renewed
        std::string     newMagic    = randomWebString(config.sizeMAGIC);
        std::string     newCookie   = make_cookie(ss->ssid, newMagic);
        std::string     oldOld;
        
        {
            tbb::spin_rw_mutex::scoped_lock _lock(ss->mutex, true);
            if(time > ss->magicked){    // task could've been duplicated outside of the mutex lock
                std::swap(oldOld, ss->oldMagic);
                std::swap(ss->oldMagic, ss->magic);
                std::swap(ss->magic, newMagic);
                ss->magicked = time;
            }
        }
        
        return Renew;
    }
    
    Context::Quality    Context::config_session()
    {
        if(!cookie_ssid.empty() && !cookie_magic.empty()){
                // lookup the session
            {
                tbb::spin_rw_mutex::scoped_lock _lock(sessions.mutex);
                ss      = sessions.lut.get(cookie_ssid);
            }
            
            if(ss){
                switch(check_session()){
                case Renew:
                    send_cookie();
                    //  FALL THROUGH
                case Pass:
                    return Pass;
                case Expired:
                    return Expired;
                default:
                    break;
                }
            }
        }
        
        ss              = new Session;
        session         = ss -> master;
        ss->accessed    = time;
        send_cookie();
        return Pass;
    }
    

    void    Context::send_cookie()
    {
        if(ss)
            tx_header("Set-Cookie", ss->cookie);
    }
    
    void    Context::set_auto_edit(bool f) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.auto_edit    = f;
            }
            session.auto_edit   = f;
        }
    }
    
    void    Context::set_columns(int v) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.columns    = v;
            }
            session.columns   = v;
        }
    }
    
    void    Context::set_def_root(const Root*rt) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.def_root    = rt;
            }
            session.def_root   = rt;
        }
    }
    
    void    Context::set_icon_size(SizeDesc v) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.icon_size    = v;
            }
            session.icon_size   = v;
        }
    }

    void    Context::set_inspect_submit(bool v) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.inspect_submit    = v;
            }
            session.inspect_submit   = v;
        }
    }
    
    void    Context::set_logged_in(bool v) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.logged_in    = v;
            }
            session.logged_in   = v;
        }
    }
    
    void    Context::set_username(std::string_view sv) 
    {
        if(ss){
            {
                tbb::spin_rw_mutex::scoped_lock     _lock(ss->mutex, true);
                ss->master.username = sv;
            }
            session.username    = sv;
        }
    }
    



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


Session::Session()
{
    time(&created);
    magicked        = created;  
    accessed        = created;  // must do this early unless we want a purge to happen before we have a chance....

    {
        tbb::spin_rw_mutex::scoped_lock _lock(sessions.mutex, true);
        do {
            ssid        = randomWebString(config.sizeSSID);
        } while(!sessions.lut.insert(ssid, this));
    }
    
    master.ssid     = ssid;
    magic           = randomWebString(config.sizeMAGIC);
    cookie          = make_cookie(ssid, magic);
}

Session::~Session()
{
}



//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


class HttpConnection : public RefCount {
public:
    enum RxMode {
        RxMethod    = 0,
        RxHeader,
        RxBody,
        RxDispatch,
        RxDone,
        RxError
    };
    
    asio::io_context&       m_context;
    asio::ip::tcp::socket   m_socket;
    Ref<Context>            m_current;
    HttpDataPtr             m_buffer;
    RxMode                  m_rxMode    = RxMethod;
    size_t                  m_next  = 0;
    size_t                  m_counter   = 0;
    bool                    m_eod   = false;

    HttpConnection(asio::io_context& ctx, asio::ip::tcp::socket&& s) : m_context(ctx), m_socket(std::move(s)) 
    {
        m_buffer                = HttpData::make();
        m_current               = new Context(ctx);
    }
    
    void    do_read()
    {
        if(!m_buffer->available())
            return ;
            
        Ref<HttpConnection>     me(this);
        m_socket.async_read_some(asio::buffer(m_buffer->freespace(), m_buffer->available()), 
            [this, me](std::error_code ec, std::size_t length){
                if(!ec){
                    m_buffer->advance(length);
                    process();
                    if(m_rxMode != RxError)
                        do_read();
                }
            }
        );
    }
    
    void    dispatch(Ref<Context> ctx)
    {
        static thread_local std::ofstream access_log(access_log_file(), std::ios_base::out | std::ios_base::app);

        HttpStatus  code;
        if(ctx->url.host.empty() && !ctx->host.empty())
            ctx->url.host    = ctx->host;
        ctx -> host = ctx -> url.host;
        if(!ctx->url.port && ctx->port)
            ctx->url.port    = ctx->port;
        ctx -> port = ctx -> url.port;
    
        if(ctx->version == VersionSpec{})
            ctx->version   = http09();
            
        if(is_similar(ctx->version, http11()) && ctx->url.host.empty()){
            dispatch(HttpStatus::BadRequest);
            return ;
        }

        time(&ctx->time);
        gmtime_r(&ctx->time, &ctx->timeparts);
        strftime(ctx->timestamp, sizeof(ctx->timestamp), "%F %T UTC", &ctx->timeparts);

        do {
            if(ctx->config_session() == Context::Expired){
                ctx -> status = HttpStatus::PageExpired;
                break;
            }

            auto pg = find_page(ctx->method, ctx->url.path);
            if(!pg.first){
                ctx->status = HttpStatus::NotFound;
                break;
            }
            bool    isLocal = ctx->remote_addr.is_loopback();
            if(isLocal)
                ctx->flags |= WebContext::LOCAL;

            if(pg.first -> local_only() && !isLocal){
                ctx->status = HttpStatus::Forbidden;
                break;
            }
            
            try {
                ctx->page   = pg.first;
                ctx->truncated_path = pg.second;
                pg.first -> handle(*ctx);
                if(ctx -> status == HttpStatus())
                    ctx -> status   = HttpStatus::Success;
            }
            catch(HttpStatus s)
            {
                ctx->status  = s;
            }
            catch(HttpStatus::enum_t s)
            {
                ctx->status = s;
            }
            catch(int ex)
            {
                if((ex >= 0) && (ex < 600)){
                    ctx->status = (HttpStatus::enum_t) ex;
                } else 
                    ctx->status = HttpStatus::InternalError;
            }
            #ifdef NDEBUG
            catch(...)
            {
                code    = HttpStatus::InternalError;
            }
            #endif
        } while(false);
        
        if(isError(ctx->status)){
            HttpStatus code = ctx->status;
            ctx -> tx_reset();
            ctx -> status   = code;
        }
        send(ctx);
        
        // log it.
        access_log << ctx->remote_addr.to_string() 
            << " - [" << ctx->timestamp  << "] \"" << ctx->method.key() << " " << ctx->url.path << " " << ctx->version
            << "\" " << ctx->status.value() << " " << ctx->tx_content_size() << "\n";
        access_log.flush();
        
        #ifndef NDEBUG
        yInfo() << ctx->remote_addr.to_string() 
            << " - [" << ctx->timestamp  << "] \"" << ctx->method << " " << ctx->url.path << " " << ctx->version
            << "\" " << ctx->status.value() << " " << ctx->tx_content_size();
        #endif
    }
    
    void    dispatch(HttpStatus code)
    {
        Ref<Context> r   = new Context(m_context);
        r -> status = code;
        send(r);
    }

    // Gets the next line out of the buffer
    std::string_view    next_line()
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

    void    process()
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

    void                rxBody()
    {
        if(!m_current){
            dispatch(HttpStatus::InternalError);
            m_rxMode    = RxError;
            return;
        }
        
    }

    void                rxCookie(std::string_view cookie_data)
    {
        vsplit(cookie_data, ';', [&](std::string_view s){
            size_t i  = s.find('=');
            if(i == std::string_view::npos)
                return ;
            
            size_t j    = s.find('+', i+1);
            if(i == std::string_view::npos)
                return ;
            
            m_current -> cookie_ssid  = std::string_view( s.data()+i+1, s.data()+j);
            m_current -> cookie_magic = s.substr(j+1);
        });
    }
    
    
    void                rxDispatch()
    {
        if(!m_current){
            dispatch(HttpStatus::InternalError);
            m_rxMode    = RxError;
            return;
        }

        auto ep = m_socket.remote_endpoint();
        m_current -> remote_addr    = ep.address();
        m_current -> remote_port    = ep.port();
        
        VersionSpec as = limit(m_current->version);
        dispatch(m_current);
        m_current   = nullptr;
        
        if(is_similar(as, http11())){    
            auto old    = m_buffer;
            m_buffer    = HttpData::make();
            if(m_next < old->count())
                m_buffer -> append(old->as_view(m_next));
            m_next      = 0;
            m_rxMode    = RxMethod;
        } else
            m_rxMode    = RxDone;
    }
    
    void                rxHeader()
    {
        if(!m_current){
            dispatch(HttpStatus::InternalError);
            m_rxMode    = RxError;
            return;
        }
    
        KVView  hv  = parse_header_line(next_line());
        if(hv.empty()){
            if(!m_eod){
                //if(m_current -> rx_content_length){
                    //m_rxMode    = RxBody;
                    //m_counter   = 0;
                //} else {
                    m_rxMode    = RxDispatch;
                //}
            }
            return;
        }
        
        m_current -> rx_headers.insert( hv.key, hv.value );

        if(is_similar(hv.key, "host")){
            auto t  = as_host_port(hv.value);
            if(t.good){
                m_current -> host = t.value.host;
                m_current -> port = t.value.port;
            }
        } else if(is_similar(hv.key, "Content-Length")){
            auto t = to_integer(hv.value);
            if(t.good)
                m_current -> rx_content_length   = t.value;
        } else if(is_similar(hv.key, "Content-Type")){
            m_current -> rx_content_type         = ContentType(hv.value);
        } else if(is_similar(hv.key, "Cookie")){
            rxCookie(hv.value);
        }
    }

    void                rxMethod()
    {
        std::string_view    v   = next_line();
        if(v.empty())
            return;

        if(!m_current){
            m_current   = new Context(m_context);
            m_current -> rx_buffers.push_back(m_buffer);
        }

        MethodUriVersion  muri      = parse_method_uri(v);
        auto m = HttpOp::value_for(muri.method);
        if(!m.good){
            dispatch(HttpStatus::MethodNotAllowed);
            m_rxMode    = RxError;
            return ;
        }
        
        m_current -> method   = m.value;
        if(muri.uri.empty()){
            dispatch(HttpStatus::MissingURI);
            m_rxMode    = RxError;
            return;
        }
    
        url_view_r     uri = to_url(muri.uri);
        if(!uri.good){
            dispatch(HttpStatus::BadURI);
            m_rxMode    = RxError;
            return;
        }
        
        m_current -> url = uri.value;
        if(muri.version.empty()){
            m_current -> version = http09();
            m_rxMode    = RxDispatch;
            return ;
        }

        m_current->version = to_version_spec(muri.version);
        if(!is_similar(m_current->version.protocol, "http")){
            dispatch(HttpStatus::BadHTTPVersion);
            m_rxMode    = RxError;
            return ;
        }
        
        if( (Version) http09() == (Version) m_current->version){
            m_rxMode    = RxDispatch;
        } else
            m_rxMode    = RxHeader;
    }
    
    
    void        send(Ref<Context>ctx)
    {
        const StatusQ& sq    = gStatusMessages[ctx->status];
        
        std::vector<asio::const_buffer>  buffers;
        buffers.push_back(sq.reply_asio);
        bool    has_content = false;
    
        if(isSuccessful(ctx->status)){
            size_t  sz  = ctx->tx_content_size();
            if(sz){
                has_content = true;
                ctx -> tx_header("Content-Length", to_string(sz));
            }
        } else if(isError(ctx->status)){
            // reset the content buffer
            if(ctx -> tx_header_buffer)
                ctx -> tx_header_buffer -> count(0);
            if(sq.html_data.size()){
                ctx -> tx_header("Content-Type", "text/html");
                ctx -> tx_header("Content-Length", to_string(sq.html_data.size()));
            }
        }
        
        if(ctx -> tx_header_buffer)
            buffers.push_back(asio::buffer(ctx -> tx_header_buffer));
        buffers.push_back(asio::buffer("\r\n"sv));
        
        if(isSuccessful(ctx->status) && has_content){
            for(auto& p : ctx->tx_content){
                if(p)
                    buffers.push_back(asio::buffer(p));
            }
            buffers.push_back(asio::buffer("\r\n"sv));
        } else if(isError(ctx->status) && sq.html_data.size()){
            buffers.push_back(sq.html_asio);
            buffers.push_back(asio::buffer("\r\n"sv));
        }
        
        Ref<HttpConnection> self = this;
        async_write(m_socket, buffers, [ctx, this, self](std::error_code ec, size_t){
            if (!ec)
            {
              // Initiate graceful connection closure.
              asio::error_code ignored_ec;
              m_socket.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
            }
        } /* executor is to make sure response & buffers stay alive */ );
    }

};

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

class HttpServer {
public:

    HttpServer(asio::io_context& ctx) : 
        m_context(ctx), 
        m_acceptor(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), wksp::port())), 
        m_socket(ctx)
    {
    }
    
    void    start()
    {
        m_acceptor.async_accept(m_socket, [this](std::error_code ec){
            if(!ec){
                ( new HttpConnection(m_context, std::move(m_socket))) -> do_read();
            }
            start();
        });
    }
    
    asio::io_context&           m_context;
    asio::ip::tcp::acceptor     m_acceptor;
    asio::ip::tcp::socket       m_socket;
};

//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
//  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

void        do_server()
{
    thread::id();
    yInfo() << "Launching Quill HttpServer for '" << wksp::name() << "' on Port " << wksp::port() << ".";

    asio::error_code        ec;
    asio::io_context        context;
    HttpServer  server(context);
    server.start();
    context.run();
    
    yInfo() << "Exiting the server.";
    
    if(gQuit == Quit::No)
        gQuit       = Quit::Stop;
}


void        run_server(yq::Vector<std::thread>&  threads)
{
    genStatus();
    threads << std::thread(do_server);
}

