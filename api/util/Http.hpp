////////////////////////////////////////////////////////////////////////////////
//
//  YOUR QUILL
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Enum.hpp"

YQ_ENUM(HttpOp, ,
    Unknown = 0,
    Get,
    Head,
    Post,
    Put,
    Delete,
    Trace,
    Options,
    Connect,
    Patch
)


static constexpr const HttpOp::enum_t hConnect                  = HttpOp::Connect;
static constexpr const HttpOp::enum_t hDelete                   = HttpOp::Delete;
static constexpr const HttpOp::enum_t hGet                      = HttpOp::Get;
static constexpr const HttpOp::enum_t hHead                     = HttpOp::Head;
static constexpr const HttpOp::enum_t hOptions                  = HttpOp::Options;
static constexpr const HttpOp::enum_t hPatch                    = HttpOp::Patch;
static constexpr const HttpOp::enum_t hPost                     = HttpOp::Post;
static constexpr const HttpOp::enum_t hPut                      = HttpOp::Put;
static constexpr const HttpOp::enum_t hTrace                    = HttpOp::Trace;

YQ_ENUM(HttpStatus, ,
    None                        = 0,
    
    Continue                    = 100,
    SwitchingProtocols          = 101,
    Processing                  = 102,
    EarlyHints                  = 103,

    Okay                        = 200,
    Success                     = 200,
    Created                     = 201,
    Accepted                    = 202,
    NonAuthoritative            = 203,
    NoContent                   = 204,
    ResetContent                = 205,
    PartialContent              = 206,
    MultiStatus                 = 207,
    AlreadyReported             = 208,
    ThisIsFine                  = 218,
    IMUsed                      = 226,

    MultipleChoice              = 300,
    MovedPermanently            = 301,
    Found                       = 302,
    SeeOther                    = 303,
    NotModified                 = 304,
    UseProxy                    = 305,
    TemporaryRedirect           = 307,
    PermanentRedirect           = 308,
    
    BadRequest                  = 400,
    Unauthorized                = 401,
    PaymentRequired             = 402,
    Forbidden                   = 403,
    NotFound                    = 404,
    MethodNotAllowed            = 405,
    NotAcceptable               = 406,
    ProxyAuthenticationReq      = 407,
    RequestTimeout              = 408,
    Conflict                    = 409,
    Gone                        = 410,
    LengthRequired              = 411,
    PreconditionFailed          = 412,
    PayloadTooLarge             = 413,
    UriTooLong                  = 414,
    UnsupportedMedia            = 415,
    BadRange                    = 416,
    ExpectationFailed           = 417,
    Teapot                      = 418,
    PageExpired                 = 419,
    EnhanceYourCalm             = 420,
    MisdirectedRequest          = 421,
    UnprocessableEntity         = 422,
    Locked                      = 423,
    FailedDependency            = 424,
    TooEarly                    = 425,
    UpgradeRequired             = 426,
    PreconditionRequired        = 428,
    TooManyRequests             = 429,
    RequestHeaderTooLarge       = 431,
    LoginTimeout                = 440,
    NoResponse                  = 444,
    RetryWith                   = 449,
    ParentalControl             = 450,
    Censored                    = 451,
    ClientClosedRequest         = 499,

    InternalError               = 500,
    NotImplemented              = 501,
    BadGateway                  = 502,
    ServiceUnavailable          = 503,
    GatewayTimeout              = 504,
    HTTPVersionNotSupported     = 505,
    VariantAlsoNegotiates       = 506,
    InsufficientStorage         = 507,
    LoopDetected                = 508,
    BandwidthLimitExceeded      = 509,
    NotExtended                 = 510,
    NetworkAuthenticationReq    = 511,
    InvalidSSLCertificate       = 526,
    
    CurlError                   = 597,
    NetworkReadTimeout          = 598,
    NetworkConnectTimeout       = 599
)

static constexpr const HttpStatus::enum_t   hNotFound           = HttpStatus::NotFound;
static constexpr const HttpStatus::enum_t   hBadRequest         = HttpStatus::BadRequest;

YQ_ENUM(ContentType, ,
    unknown                     = 0,
    binary,
    bmp,
    css,
    csv,
    gif,
    html,
    javascript,
    jpeg,
    json,
    latex,
    markdown,
    mp3,
    mp4,
    mpeg,
    ogg,
    pdf,
    png,
    rtf,
    svg,
    text,
    tiff,
    wave,
    xml
)

const char* statusMessage(HttpStatus);
const char* statusMessage(unsigned short);

bool        isRedirect(HttpStatus);
bool        isRedirect(unsigned short);

bool        isError(HttpStatus);
bool        isError(unsigned short);

bool        isClientError(HttpStatus);
bool        isClientError(unsigned short);

bool        isServerError(HttpStatus);
bool        isServerError(unsigned short);

bool        isSuccessful(HttpStatus);
bool        isSuccessful(unsigned short);

bool        isInformational(HttpStatus);
bool        isInformational(unsigned short);

const char* mimeType(ContentType);
ContentType mimeTypeForExt(const String&);

