////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#include "Http.hpp"

#include <util/app/DelayInit.hpp>
#include <util/collection/EnumMap.hpp>
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
                { ContentType::mp3,             "text/markdown"             },
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

}
