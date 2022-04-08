////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "yquill.hpp"
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

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

using namespace yq;

//! Version of this server, we'll push it up later
constexpr const VersionSpec     kServerHttpVersion     = http10();

namespace asio {
    const_buffer    buffer(const HttpDataPtr& dp)
    {
        return buffer(dp->data(), dp->count());
    }
}

namespace {

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
                text << kServerHttpVersion.protocol << '/' << kServerHttpVersion.major << '.' << kServerHttpVersion.minor << ' ' << hs.value() << ' ' << sq.source << "\r\n";
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
        if((Version) vs > (Version) kServerHttpVersion)
            return kServerHttpVersion;
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

    
    class Connection : public RefCount {
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
        Ref<WebContext>         m_current;
        HttpDataPtr             m_buffer;
        RxMode                  m_rxMode    = RxMethod;
        size_t                  m_next  = 0;
        size_t                  m_counter   = 0;
        bool                    m_eod   = false;

        Connection(asio::io_context& ctx, asio::ip::tcp::socket&& s) : m_context(ctx), m_socket(std::move(s)) 
        {
            m_buffer                = HttpData::make();
            m_current               = new WebContext(ctx);
        }
        
        void    do_read()
        {
            if(!m_buffer->available())
                return ;
                
            Ref<Connection>     me(this);
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
        
        void    dispatch(Ref<WebContext> ctx)
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
            Ref<WebContext> r   = new WebContext(m_context);
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
        
            HttpHeaderView  hv  = parse_header_line(next_line());
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
            
            m_current -> rx_headers.push_back(hv);

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
            }
        }
    
        void                rxMethod()
        {
            std::string_view    v   = next_line();
            if(v.empty())
                return;

            if(!m_current){
                m_current   = new WebContext(m_context);
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
        
        
        void        send(Ref<WebContext>ctx)
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
            
            Ref<Connection> self = this;
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
    
    class Server {
    public:
    
        Server(asio::io_context& ctx) : 
            m_context(ctx), 
            m_acceptor(ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), wksp::port())), 
            m_socket(ctx)
        {
        }
        
        void    start()
        {
            m_acceptor.async_accept(m_socket, [this](std::error_code ec){
                if(!ec){
                    ( new Connection(m_context, std::move(m_socket))) -> do_read();
                }
                start();
            });
        }
        
        asio::io_context&           m_context;
        asio::ip::tcp::acceptor     m_acceptor;
        asio::ip::tcp::socket       m_socket;
    };
    
    void        do_server()
    {
        thread::id();
        yInfo() << "Launching Quill Server for '" << wksp::name() << "' on Port " << wksp::port() << ".";

        asio::error_code        ec;
        asio::io_context        context;
        Server  server(context);
        server.start();
        context.run();
        
        yInfo() << "Exiting the server.";
        
        if(gQuit == Quit::No)
            gQuit       = Quit::Stop;
    }
}


void        run_server(yq::Vector<std::thread>&  threads)
{
    genStatus();
    threads << std::thread(do_server);
}

