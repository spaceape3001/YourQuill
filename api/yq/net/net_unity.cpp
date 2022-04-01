////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Http.hpp"
#include "Url.hpp"
#include "VersionSpec.hpp"

#include <yq/app/DelayInit.hpp>
#include <yq/collection/EnumMap.hpp>
#include <yq/log/Logging.hpp>
#include <yq/stream/Ops.hpp>
#include <yq/text/Utils.hpp>
#include <stdio.h>

namespace yq {

    bool        isClientError(unsigned short u)
    {
        return (u>=400) && (u<500);
    }

    bool        isClientError(HttpStatus u)
    {
        return isClientError((unsigned short) u.value());
    }

    bool        isError(unsigned short u)
    {
        return (u>=400) && (u<600);
    }

    bool        isError(HttpStatus u)
    {
        return isError((unsigned short) u.value());
    }

    bool        isInformational(unsigned short u)
    {
        return (u>=100) && (u<200);
    }

    bool        isInformational(HttpStatus u)
    {
        return isInformational((unsigned short) u);
    }

    bool        isRedirect(unsigned short u)
    {
        return (u>=300) && (u<400);
    }

    bool        isRedirect(HttpStatus u)
    {
        return isRedirect((unsigned short) u);
    }


    bool        isServerError(unsigned short u)
    {
        return (u>=500) && (u<600);
    }

    bool        isServerError(HttpStatus u)
    {
        return isServerError((unsigned short) u);
    }

    bool        isSuccessful(unsigned short u)
    {
        return (u>=200) && (u<300);
    }

    bool        isSuccessful(HttpStatus u)
    {
        return isSuccessful((unsigned short) u);
    }


    namespace {
    
        //std::string makeMessage(const char* cat, unsigned short value)
        //{
            //char        buffer[256];
            //sprintf(buffer, "%s %d", cat, (int) value);
            //return std::string(buffer);
        //}
        
        using HttpStatusMap	= EnumMap<HttpStatus,std::string_view>;
        
        HttpStatusMap 	makeStatusMessage()
        {
            static const struct {
                HttpStatus::enum_t  code;
                std::string_view    message;
            } kCodes[] = {
                { HttpStatus::None,                     	"Invalid"                           },

                //  100 Codes (information)
                { HttpStatus::Continue,                     "Continue"                          },
                { HttpStatus::SwitchingProtocols,           "Switching Protcols"                },
                { HttpStatus::Processing,                   "Processing...."                    },
                { HttpStatus::EarlyHints,                   "Psst, Early Hints."                },
                
                //  200 codes (success)
                { HttpStatus::Okay,                         "OK"                                },
                { HttpStatus::Created,                      "Created"                           },
                { HttpStatus::Accepted,                     "Accepted"                          },
                { HttpStatus::NonAuthoritative,             "Non-Authroitative Information"     },
                { HttpStatus::NoContent,                    "No content"                        },
                { HttpStatus::ResetContent,                 "Reset Content"                     },
                { HttpStatus::PartialContent,               "Partial Content"                   },
                { HttpStatus::MultiStatus,                  "Multi Status"                      },
                { HttpStatus::AlreadyReported,              "Already Reported"                  },
                { HttpStatus::ThisIsFine,                   "This is fine."                     },
                { HttpStatus::IMUsed,                       "IM Used"                           },

                //  300 codes
                { HttpStatus::MultipleChoice,               "Multiple Choice Redirect"          },
                { HttpStatus::MovedPermanently,             "Moved Permanently"                 },
                { HttpStatus::Found,                        "Found"                             },
                { HttpStatus::SeeOther,                     "See Other"                         },
                { HttpStatus::NotModified,                  "Not Modified"                      },
                { HttpStatus::UseProxy,                     "Use Proxy"                         },
                { HttpStatus::TemporaryRedirect,            "Temporary Redirect"                },
                { HttpStatus::PermanentRedirect,            "Permanent Redirect"                },
            
                { HttpStatus::BadRequest,                   "Bad Request"                       },
                { HttpStatus::Unauthorized,                 "Unauthorized"                      },
                { HttpStatus::PaymentRequired,              "Payment Required"                  },
                { HttpStatus::Forbidden,                    "Forbidden"                         },
                { HttpStatus::NotFound,                     "Not Found"                         },
                { HttpStatus::MethodNotAllowed,             "Method Not Allowed"                },
                { HttpStatus::NotAcceptable,                "Not Acceptable"                    },
                { HttpStatus::ProxyAuthenticationReq,       "Proxy Authentication Required"     },
                { HttpStatus::RequestTimeout,               "RequestTimeout"                    },
                { HttpStatus::Conflict,                     "Conflict"                          },
                { HttpStatus::Gone,                         "Gone"                              },
                { HttpStatus::LengthRequired,               "Length Required"                   },
                { HttpStatus::PreconditionFailed,           "Precondition Failed"               },
                { HttpStatus::PayloadTooLarge,              "Payload Too Large"                 },
                { HttpStatus::UriTooLong,                   "Uri Too Long"                      },
                { HttpStatus::UnsupportedMedia,             "Unsupported Media"                 },
                { HttpStatus::BadRange,                     "Bad Range"                         },
                { HttpStatus::ExpectationFailed,            "Expectation Failed"                },
                { HttpStatus::Teapot,                       "I'm a Teapot"                      },
                { HttpStatus::PageExpired,                  "Page Expired"                      },
                { HttpStatus::EnhanceYourCalm,              "Enhance Your Calm"                 },
                { HttpStatus::MisdirectedRequest,           "Misdirected Request"               },
                { HttpStatus::UnprocessableEntity,          "Unprocessable Entity"              },
                { HttpStatus::Locked,                       "Locked"                            },
                { HttpStatus::FailedDependency,             "Failed Dependency"                 },
                { HttpStatus::TooEarly,                     "Too Early"                         },
                { HttpStatus::UpgradeRequired,              "Upgrade Required"                  },
                { HttpStatus::PreconditionRequired,         "Precondition Required"             },
                { HttpStatus::TooManyRequests,              "Too Many Requests"                 },
                { HttpStatus::RequestHeaderTooLarge,        "Request Header Too Large"          },
                { HttpStatus::LoginTimeout,                 "Login Timeout"                     },
                { HttpStatus::NoResponse,                   "No Response"                       },
                { HttpStatus::RetryWith,                    "Retry With"                        },
                { HttpStatus::ParentalControl,              "Parental Control"                  },
                { HttpStatus::Censored,                     "Censored"                          },
                { HttpStatus::BadHTTPVersion,               "Incompatible HTTP Version"         },
                { HttpStatus::MissingURI,                   "Missing URI"                       },
                { HttpStatus::BadURI,                       "Malformed URI"                     }, 
                { HttpStatus::ClientClosedRequest,          "Client Closed Request"             },

                { HttpStatus::InternalError,                "Internal Error"                    },
                { HttpStatus::NotImplemented,               "Not Implemented"                   },
                { HttpStatus::BadGateway,                   "Bad Gateway"                       },
                { HttpStatus::ServiceUnavailable,           "Service Unavailable"               },
                { HttpStatus::GatewayTimeout,               "Gateway Timeout"                   },
                { HttpStatus::HTTPVersionNotSupported,      "Http Version Not Supported"        },
                { HttpStatus::VariantAlsoNegotiates,        "Variant Also Negotiates"           },
                { HttpStatus::InsufficientStorage,          "Insufficient Storage"              },
                { HttpStatus::LoopDetected,                 "Loop Detected"                     },
                { HttpStatus::BandwidthLimitExceeded,       "Bandwidth Limit Exceeded"          },
                { HttpStatus::NotExtended,                  "Not Extended"                      },
                { HttpStatus::NetworkAuthenticationReq,     "Network Authentication Required"   },
                { HttpStatus::InvalidSSLCertificate,        "Invalid SSL Certificate"           },
                { HttpStatus::CurlError,                    "Error in CURL library"             },
                { HttpStatus::NetworkReadTimeout,           "Network Read Timeout"              },
                { HttpStatus::NetworkConnectTimeout,        "Network Connect Timeout"           }
            };


            HttpStatusMap      ret;
            for(auto& k : kCodes)
                ret[k.code] 	= k.message;
            return ret;
        }
        
        const HttpStatusMap& statusMessages()
        {
            static const HttpStatusMap ret = makeStatusMessage();
            return ret;
        }
        
        using ContentTypeMap	= EnumMap<ContentType, std::string_view>;
        
        ContentTypeMap makeContentType()
        {
            static const struct {
                ContentType::enum_t     code;
                std::string_view        mime;
            } kCodes[] = {
                { ContentType::unknown,         "application/octet-stream"  },
                { ContentType::binary,          "application/octet-stream"  },
                { ContentType::bmp,             "image/bmp"                 },
                { ContentType::css,             "text/css"                  },
                { ContentType::csv,             "text/csv"                  },
                { ContentType::gif,             "text/gif"                  },
                { ContentType::html,            "text/HTML"                 },
                { ContentType::javascript,      "application/javascript"    },
                { ContentType::jpeg,            "image/jpeg"                },
                { ContentType::json,            "application/json"          },
                { ContentType::latex,           "application/x-latex"       },
                { ContentType::markdown,        "text/markdown"             },
                { ContentType::mp3,             "audio/mpeg3"               },
                { ContentType::mp4,             "video/mp4"                 },
                { ContentType::mpeg,            "video/mpeg"                },
                { ContentType::ogg,             "audio/ogg"                 },
                { ContentType::pdf,             "application/pdf"           },
                { ContentType::png,             "image/png"                 },
                { ContentType::rtf,             "text/richtext"             },
                { ContentType::svg,             "image/svg+xml"             },
                { ContentType::text,            "text/plain"                },
                { ContentType::tiff,            "image/tiff"                },
                { ContentType::wave,            "audio/wav"                 },
                { ContentType::xml,             "text/xml"                  }
            };
            
            ContentTypeMap ret;
            for(auto& k : kCodes)
                ret[k.code] = k.mime;
            return ret;
        }
        
        const ContentTypeMap&	contentTypes()
        {
            static const ContentTypeMap	ret = makeContentType();
            return ret;
        }
        
        using ContentExtMap = Map<std::string_view,ContentType,IgCase>;
        
        ContentExtMap   makeExtensionMap()
        {
            static const struct {
                std::string_view        ext;
                ContentType::enum_t     code;
            } kExts[] = {
                { "bmp",    ContentType::bmp        },
                { "css",    ContentType::css        },
                { "csv",    ContentType::csv        },
                { "gif",    ContentType::gif        },
                { "htm",    ContentType::html       },
                { "html",   ContentType::html       },
                { "js",     ContentType::javascript },
                { "jpe",    ContentType::jpeg       },
                { "jpeg",   ContentType::jpeg       },
                { "jpg",    ContentType::jpeg       },
                { "json",   ContentType::json       },
                { "latex",  ContentType::latex      },
                { "md",     ContentType::markdown   },
                { "mp3",    ContentType::mp3        },
                { "mp4",    ContentType::mp4        },
                { "ogg",    ContentType::ogg        },
                { "pdf",    ContentType::pdf        },
                { "png",    ContentType::png        },
                { "rtf",    ContentType::rtf        },
                { "svg",    ContentType::svg        },
                { "tex",    ContentType::latex      },
                { "txt",    ContentType::text       },
                { "tif",    ContentType::tiff       },
                { "tiff",   ContentType::tiff       },
                { "wav",    ContentType::wave       },
                { "xml",    ContentType::xml        }
            };
            
            ContentExtMap ret;
            for(auto& k : kExts)
                ret[k.ext]  = k.code;
            return ret;
        }
        
        const ContentExtMap&    extToTypeMap()
        {
            static const ContentExtMap s_ret = makeExtensionMap();
            return s_ret;
        }
        
    }

    std::string_view    statusMessage(unsigned short code)
    {
        const auto & r	= statusMessages();
        if(!r.valid((int) code))
            return std::string_view();
        return r[code];
    }

    std::string_view    statusMessage(HttpStatus code)
    {
        return statusMessages()[code];
    }

    std::string_view    mimeType(ContentType ct)
    {
        return contentTypes()[ct];
    }

    ContentType mimeTypeForExt(const std::string_view& ext)
    {
        const auto& r = extToTypeMap();
        return r.get(ext);
    }


    YQ_INVOKE(
        statusMessages();
        contentTypes();
        extToTypeMap();
    )

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    //Stream& operator<<(Stream&s, const VersionSpec&v)
    //{
        //return s << v.protocol << '/' << v.major << '.' << v.minor;
    //}
    
    //log4cpp::CategoryStream& operator<<(log4cpp::CategoryStream&s, const VersionSpec&v)
    //{
        //return s << v.protocol << '/' << v.major << '.' << v.minor;
    //}

    bool       is_similar(const VersionSpec& a, const VersionSpec& b)
    {
        return is_similar(a.protocol, b.protocol) && (a.major == b.major) && (a.minor == b.minor);
    }
    
    VersionSpec         to_version_spec(const char* s0, size_t cb)
    {
        VersionSpec     ret;
        if(!s0 || !cb)
            return ret;
        
        enum Mode {
            Major = 0,
            Dot,
            Minor,
            Done
        };
        
        const char* y   = strnchr(s0, cb, '/');
        if(!y){
            ret.protocol    = std::string_view(s0, cb);
        } else {
            ret.protocol    = std::string_view(s0, y);
            ++y;
            Mode        mode    = Major;
            for(const char* z = y; z<(s0+cb); ++z){
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

    VersionSpec             to_version_spec(const std::string_view&s)
    {
        return to_version_spec(s.data(), s.size());
    }

    //  ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    url_view_r      as_host_port(const char*sz, size_t cb)
    {
        if(!sz || !cb)
            return {};

        enum Mode {
            Host1       = 0,
            Host2,
            IPv6,
            SqClose,
            Port
        };
        
        UrlView     ret;
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
        
        for(z = sz; z < sz+cb; ++z){
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

    url_view_r      as_host_port(const std::string_view&s)
    {
        return as_host_port(s.data(), s.size());
    }

    Url             copy(const UrlView& v)
    {
        return Url{ copy(v.scheme), copy(v.user), copy(v.pwd), copy(v.host), copy(v.path), copy(v.query), copy(v.fragment), v.port};
    }

    bool            is_valid(const UrlView&v)
    {
        if(v.host.empty()){
            if(!v.user.empty())
                return false;
            if(!v.pwd.empty())
                return false;
            if(!v.port)
                return false;
        } else {
            if(v.user.empty() && !v.pwd.empty())
                return false;
        }
        
        return true;
    }
    
    Url             to_url(const std::filesystem::path& pth)
    {
        return Url{ "file", std::string(), std::string(), std::string(), pth.string(), std::string(), std::string(), 0 };
    }
    
    url_view_r      to_url(const char* sz, size_t cb)
    {
        //  URI = scheme ":" ["//" authority] path ["?" query] ["#" fragment]
        //  authority = [userinfo "@"] host [":" port]
        
        if(!sz || !cb)
            return {};
            
        if((cb == 1) && (*sz == '/'))
            return { UrlView{ std::string_view(), std::string_view(), std::string_view(), std::string_view(), "/", std::string_view(), std::string_view(), 0 }, true };

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
        
        UrlView     ret;
        
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
        
        for(z=sz;z!=sz+cb;++z){
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
    
    url_view_r      to_url(const char*z)
    {   
        return to_url(z, strlen(z));
    }

    url_view_r      to_url(const std::string_view&s)
    {
        return to_url(s.data(), s.size());
    }

    //template <typename S>
    //void            stream(S& s, const UrlView& v)
    //{
        //if(!v.scheme.empty())
            //s << v.scheme << ':';
        
        //if(!v.host.empty()){
            //s << "//";
            //if(!v.user.empty()){
                //s << v.user;
                //if(!v.pwd.empty())
                    //s << ':' << v.pwd;
                //s << '@';
            //}
            //s << v.host;
            //if(v.port)
                //s << ':' << v.port;
            
            //if(v.path.empty()){
                //s << '/';
            //} else{
                //if(v.path[0] != '/')
                    //s << '/';
                //s << v.path;
            //}
        //} else {
            //if(v.scheme.empty()){
                //s << v.path;
            //} else {
                //if(v.path[0] == '/')
                    //s << "//";
                //s << v.path;
            //}
        //}
        
        //if(!v.query.empty())
            //s << '?' << v.query;
        //if(!v.fragment.empty())
            //s << '#' << v.fragment;
    //}


    //Stream&         operator<<(Stream&s, const UrlView&v)
    //{
        //stream(s, v);
        //return s;
    //}
    
    //Stream&         operator<<(Stream&s, const Url&v)
    //{
        //return s << (UrlView) v;
    //}

    //std::ostream&   operator<<(std::ostream&s, const UrlView& v)
    //{
        //stream(s, v);
        //return s;
    //}
    
    //std::ostream&   operator<<(std::ostream&s, const Url& v)
    //{
        //return s << (UrlView) v;
    //}
    

}
